#include "ExecutorHelpers.h"

#include "gtest/gtest.h"

namespace spark::log::testing
{
    constexpr std::array<std::pair<Level, std::string_view>, 7> TEST_VALUES = {
        {
            {Level::Trace, "a trace message"},
            {Level::Debug, "a debug message"},
            {Level::Info, "an info message"},
            {Level::Warning, "a warning message"},
            {Level::Error, "an error message"},
            {Level::Critical, "a critical message"}
        }
    };

    class LoggerShould : public ::testing::TestWithParam<std::pair<Level, std::string_view>> {};

    TEST_P(LoggerShould, writeInConsole)
    {
        // When logging something, then a line is written to console
        EXPECT_FALSE(execute_logger(GetParam().first, GetParam().second).empty());
    }

    TEST_P(LoggerShould, writeInFile)
    {
        // When logging something
        execute_logger(GetParam().first, GetParam().second);

        // Then, a line is written into the log file
        const auto line = last_logged_in_file();
        EXPECT_FALSE(line.empty());
    }

    INSTANTIATE_TEST_SUITE_P(, LoggerShould, ::testing::ValuesIn(TEST_VALUES));
}
