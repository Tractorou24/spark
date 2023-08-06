#pragma once

#include <format>
#include <source_location>
#include <stdexcept>

SPARK_WARNING_PUSH
SPARK_DISABLE_MSVC_WARNING(4275) // non dll-interface class used as base for dll-interface class -> disabled since class is an stl class

namespace spark::base::details
{
    /**
     * \brief The base class for all exceptions in Spark. Should be used as a base class only.
     */
    class SPARK_BASE_EXPORT Exception : public std::runtime_error
    {
    public:
        explicit Exception(const char* class_name, const std::string_view& message, const std::source_location& source_location)
            : std::runtime_error(FormatMessage(class_name, message, source_location).c_str()) {}

        ~Exception() override = default;

        Exception(const Exception& other) = default;
        Exception(Exception&& other) noexcept = default;
        Exception& operator=(const Exception& other) = default;
        Exception& operator=(Exception&& other) noexcept = default;

    protected:
        static std::string FormatMessage(const char* class_name, const std::string_view& message, const std::source_location& source_location)
        {
            return std::format("Exception of type {0} has been thrown.\nError happen in {1}:{2}.\nException contains the message:\n{3}",
                               class_name,
                               source_location.file_name(),
                               source_location.line(),
                               message);
        }
    };
}

SPARK_WARNING_POP
