#include "spark/log/Logger.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace spark::log
{
    void log(Level level, const std::string& message)
    {
        static auto logger = []() -> std::shared_ptr<spdlog::logger>
        {
            auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            color_sink->set_pattern("%^[%T] [%l] %n: %v%$");

            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("spark.log", true);
            file_sink->set_pattern("[%T] [%l] %n: %v");

            auto core_logger = std::make_shared<spdlog::logger>("SPARK", spdlog::sinks_init_list{color_sink, file_sink});
            spdlog::register_logger(core_logger);
            core_logger->set_level(spdlog::level::trace);
            core_logger->flush_on(spdlog::level::trace);

            return core_logger;
        }();

        logger->log(static_cast<spdlog::level::level_enum>(level), message);
    }
}
