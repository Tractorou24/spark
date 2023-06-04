#pragma once

#include "spark/base/Platforms.h"

#include "spark/log/Logger.h"

/*
 *  Define macros to disable warnings on compilers
 */
#ifdef SPARK_COMPILER_MSVC
#	define DO_PRAGMA(pragma) __pragma(pragma)

#	define SPARK_WARNING_PUSH DO_PRAGMA(warning(push))
#	define SPARK_WARNING_POP  DO_PRAGMA(warning(pop))

#	define SPARK_DISABLE_MSVC_WARNING(warningNumber) DO_PRAGMA(warning(disable: warningNumber))
#	define SPARK_SUPPRESS_MSVC_WARNING(warningNumber) DO_PRAGMA(warning(suppress: warningNumber))
#	define SPARK_DISABLE_GCC_WARNING(warningName)
#	define SPARK_DISABLE_CLANG_WARNING(warningName)

#	define SPARK_DEBUG_BREAK __debugbreak()
#elif SPARK_COMPILER_GCC
#   define DO_PRAGMA(pragma) _Pragma(#pragma)

#   define SPARK_WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
#   define SPARK_WARNING_POP  DO_PRAGMA(GCC diagnostic pop)

#   define SPARK_DISABLE_MSVC_WARNING(warningNumber)
#   define SPARK_SUPPRESS_MSVC_WARNING(warningNumber)
#   define SPARK_DISABLE_GCC_WARNING(warningName) DO_PRAGMA(GCC diagnostic ignored #warningName)
#   define SPARK_DISABLE_CLANG_WARNING(warningName)

#   define SPARK_DEBUG_BREAK __builtin_trap()
#elif SPARK_COMPILER_CLANG
#   define DO_PRAGMA(pragma) _Pragma(#pragma)

#   define SPARK_WARNING_PUSH DO_PRAGMA(clang diagnostic push)
#   define SPARK_WARNING_POP  DO_PRAGMA(clang diagnostic pop)

#   define SPARK_DISABLE_MSVC_WARNING(warningNumber)
#   define SPARK_SUPPRESS_MSVC_WARNING(warningNumber)
#   define SPARK_DISABLE_GCC_WARNING(warningName)
#   define SPARK_DISABLE_CLANG_WARNING(warningName) DO_PRAGMA(clang diagnostic ignored #warningName)

#   define SPARK_DEBUG_BREAK __builtin_trap()
#else
#   define DO_PRAGMA(pragma) 

#   define SPARK_WARNING_PUSH
#   define SPARK_WARNING_POP

#   define SPARK_DISABLE_MSVC_WARNING(warningNumber)
#   define SPARK_SUPPRESS_MSVC_WARNING(warningNumber)
#   define SPARK_DISABLE_GCC_WARNING(warningName)
#   define SPARK_DISABLE_CLANG_WARNING(warningName)

#   define SPARK_DEBUG_BREAK 

#endif

/*
 * Define macro to disable unused variable warning
 */
#define SPARK_UNUSED(x) (void)(x)

/*
 * Define macro to offset bits. Used in enums definitions.
 */
#define BIT(X) (1 << X)

/*
 * Define assertions macros
 * TODO: Add support for custom assertion handlers
 * TODO: Add support for custom assertion messages
 * TODO: Add support for disabling assertions globally
*/
#define SPARK_CORE_ASSERT(cond)                                 \
    if (!(cond))                                                \
    {                                                           \
        SPARK_CORE_CRITICAL("Assertion Failed: {0}", #cond);    \
        SPARK_DEBUG_BREAK;                                      \
    }

#define SPARK_ASSERT(cond)                              \
    if (!(cond))                                        \
    {                                                   \
        SPARK_CRITICAL("Assertion Failed: {0}", #cond); \
        SPARK_DEBUG_BREAK;                              \
    }
