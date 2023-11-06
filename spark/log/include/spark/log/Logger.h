#pragma once

#include "spark/log/Export.h"
#include "spark/log/Level.h"

#include <format>
#include <string_view>

namespace spark::log
{
    /**
     * \brief Logs a message with the trace log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (white color)
     * \param level The \ref spark::log::Level for the message
     * \param message A \ref std::string containing the message to log
     */
    SPARK_LOG_EXPORT void log(Level level, const std::string& message);

    /**
     * \brief Logs a message with the trace log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (white color)
     * \param message A \ref std::string_view containing the message to log
     * \param args The arguments to format the message with
     */
    template <typename... Args>
    void trace(std::string_view message, Args&&... args)
    {
        log(Level::Trace, std::vformat(message, std::make_format_args(args...)));
    }

    /**
     * \brief Logs a message with the debug log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (blue color)
     * \param message A \ref std::string_view containing the message to log
     * \param args The arguments to format the message with
     */
    template <typename... Args>
    void debug(std::string_view message, Args&&... args)
    {
        log(Level::Debug, std::vformat(message, std::make_format_args(args...)));
    }

    /**
     * \brief Logs a message with the info log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (green color)
     * \param message A \ref std::string_view containing the message to log
     * \param args The arguments to format the message with
     */
    template <typename... Args>
    void info(std::string_view message, Args&&... args)
    {
        log(Level::Info, std::vformat(message, std::make_format_args(args...)));
    }

    /**
     * \brief Logs a message with the warning log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (yellow color)
     * \param message A \ref std::string_view containing the message to log
     * \param args The arguments to format the message with
     */
    template <typename... Args>
    void warning(std::string_view message, Args&&... args)
    {
        log(Level::Warning, std::vformat(message, std::make_format_args(args...)));
    }

    /**
     * \brief Logs a message with the error log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (red color)
     * \param message A \ref std::string_view containing the message to log
     * \param args The arguments to format the message with
     */
    template <typename... Args>
    void error(std::string_view message, Args&&... args)
    {
        log(Level::Error, std::vformat(message, std::make_format_args(args...)));
    }

    /**
     * \brief Logs a message with the critical log level
     * \details Logs into a file (spark.log) in the executable launch directory and in the console (white color on red background)
     * \param message A \ref std::string_view containing the message to log
     * \param args The arguments to format the message with
     */
    template <typename... Args>
    void critical(std::string_view message, Args&&... args)
    {
        log(Level::Critical, std::vformat(message, std::make_format_args(args...)));
    }
}
