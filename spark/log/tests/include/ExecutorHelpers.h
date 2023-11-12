#pragma once

#include "spark/log/Level.h"

#include "spark/base/Platforms.h"
#include "spark/path/Paths.h"

#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

#include <array>
#include <format>
#include <fstream>
#include <optional>
#include <regex>
#include <string>
#include <tuple>

namespace spark::log::testing
{
    /**
     * \brief Executes the spark_log_executor with the specified level and message in a shell and gets the std output of it
     * \param level The log level to execute
     * \param message The message to log
     * \return A string containing the stdout of the logger executable
     */
    inline std::string execute_logger(Level level, std::string_view message)
    {
        std::array<char, 128> buffer = {};
        std::string result;

#ifdef SPARK_OS_WINDOWS
        const auto& popen = _popen;
        const auto& pclose = _pclose;
        const std::string cmd = std::format("{0}\\spark_log_executor.exe", spark::path::executable_path().string());
#else
        const std::string cmd = std::format("{0}/spark_log_executor", spark::path::executable_path().string());
#endif

        const char* level_str = spdlog::level::level_string_views[static_cast<int>(level)].data();
        const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(std::format("{0} {1} \"{2}\"", cmd, level_str, message).c_str(), "r"), pclose);
        if (!pipe)
            throw std::runtime_error("pipe open failed !");

        while (std::fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
            result += buffer.data();

        result.replace(result.find_last_of('\n'), 1, "\0");
        return result;
    }

    /**
     * \brief Gets the last line in the log file and cleans the file
     * \return The last log line as string without any `\n` inside
     */
    [[nodiscard]] inline std::string last_logged_in_file()
    {
        std::string file_content;

        // Read all file into string, retry 3 times (wait for the os to write to the file)
        int i = 0;
        do
        {
            if (i != 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

            std::ifstream log_file;
            log_file.open("spark.log", std::ios::in);
            EXPECT_TRUE(log_file.is_open());
            if (!log_file.is_open())
                return {};

            file_content = std::string((std::istreambuf_iterator<char>(log_file)), std::istreambuf_iterator<char>());
            i++;
        } while (file_content.empty() || i < 3);

        // Remove trailing \n's
        while (file_content.back() == '\n')
            file_content.pop_back();

        // Clear the file, so the next test will be working as intended and not get the line from the previous log line
        std::fstream clear_file("spark.log", std::ios::trunc);
        clear_file.close();

        const auto last_endline = file_content.find_last_of('\n');
        if (last_endline == std::string::npos)
            return file_content;
        return file_content.substr(last_endline + 1);
    }
}
