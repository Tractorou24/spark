#pragma once

#include <format>
#include <source_location>
#include <stdexcept>

#define SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG(ExceptionType, additionalMessage) throw ExceptionType(additionalMessage)
#define SPARK_BASE_DETAILS_THROW_EXCEPTION_1_ARG(ExceptionType)                    SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG(ExceptionType, nullptr)
#define SPARK_BASE_DETAILS_GET_3_TH_ARG(arg1, arg2, arg3, ...)                     arg3
#define SPARK_BASE_DETAILS_GET_3_TH_ARG_IN_SET(argsWithParenthesis)                SPARK_BASE_DETAILS_GET_3_TH_ARG argsWithParenthesis

/**
 * \brief The macro used to select the right throw exception macro.

 * If there is only one argument, it expands to \ref SPARK_BASE_DETAILS_THROW_EXCEPTION_1_ARG since __VA_ARGS__ length is 1.
 * \code
 * // Example with "AnyException" as argument:
 * SPARK_BASE_DETAILS_GET_3_TH_ARG_IN_SET((AnyException, SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG, SPARK_BASE_DETAILS_THROW_EXCEPTION_1_ARG)) AnyException
 * // The third argument in the set is SPARK_BASE_DETAILS_THROW_EXCEPTION_1_ARG
 * \endcode
 *
 * If there is two arguments, it expands to \ref SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG since __VA_ARGS__ length is 2.
 * \code
 * // Example with "AnyException" as argument:
 * SPARK_BASE_DETAILS_GET_3_TH_ARG_IN_SET((AnyException, SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG, SPARK_BASE_DETAILS_THROW_EXCEPTION_1_ARG)) AnyException
 * // The third argument in the set is now SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG
 * \endcode
 */
#define SPARK_BASE_DETAILS_THROW_EXCEPTION_MACRO_CHOOSER(...) SPARK_BASE_DETAILS_GET_3_TH_ARG_IN_SET((__VA_ARGS__, SPARK_BASE_DETAILS_THROW_EXCEPTION_2_ARG, SPARK_BASE_DETAILS_THROW_EXCEPTION_1_ARG)) (__VA_ARGS__)

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
