#pragma once

#include "spark/log/Export.h"

#include "spdlog/spdlog.h"

#include <memory>

namespace spark::log
{
    class SPARK_LOG_EXPORT Logger
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> CoreLogger() { return s_coreLogger; }
        static std::shared_ptr<spdlog::logger> ClientLogger() { return s_clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}

// Core log macros
#define SPARK_CORE_TRACE(...)    ::spark::log::Logger::CoreLogger()->trace(__VA_ARGS__)
#define SPARK_CORE_INFO(...)     ::spark::log::Logger::CoreLogger()->info(__VA_ARGS__)
#define SPARK_CORE_WARN(...)     ::spark::log::Logger::CoreLogger()->warn(__VA_ARGS__)
#define SPARK_CORE_ERROR(...)    ::spark::log::Logger::CoreLogger()->error(__VA_ARGS__)
#define SPARK_CORE_CRITICAL(...) ::spark::log::Logger::CoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SPARK_TRACE(...)         ::spark::log::Logger::ClientLogger()->trace(__VA_ARGS__)
#define SPARK_INFO(...)          ::spark::log::Logger::ClientLogger()->info(__VA_ARGS__)
#define SPARK_WARN(...)          ::spark::log::Logger::ClientLogger()->warn(__VA_ARGS__)
#define SPARK_ERROR(...)         ::spark::log::Logger::ClientLogger()->error(__VA_ARGS__)
#define SPARK_CRITICAL(...)      ::spark::log::Logger::ClientLogger()->critical(__VA_ARGS__)
