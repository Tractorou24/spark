#pragma once

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
