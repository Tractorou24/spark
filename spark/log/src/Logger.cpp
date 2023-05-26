#include "spark/log/Logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace spark::log
{
    std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_clientLogger;

    void Logger::Init()
    {
        std::vector<spdlog::sink_ptr> log_sinks;
        log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("spark.log", true));

        log_sinks[0]->set_pattern("%^[%T] [%l] %n: %v%$");
        log_sinks[1]->set_pattern("[%T] [%l] %n: %v");

        s_coreLogger = std::make_shared<spdlog::logger>("SPARK", log_sinks.begin(), log_sinks.end());
        spdlog::register_logger(s_coreLogger);
        s_coreLogger->set_level(spdlog::level::trace);
        s_coreLogger->flush_on(spdlog::level::trace);

        s_clientLogger = std::make_shared<spdlog::logger>("APP", log_sinks.begin(), log_sinks.end());
        spdlog::register_logger(s_clientLogger);
        s_clientLogger->set_level(spdlog::level::trace);
        s_clientLogger->flush_on(spdlog::level::trace);
    }
}
