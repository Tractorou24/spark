#pragma once

#include "spark/base/Platforms.h"

#include "spark/log/Logger.h"

/**
 * \brief Macro to silence unused variables warnings.
 * \param x The name of the variable to silence the warning for.
 */
#define SPARK_UNUSED(x) (void)(x)

/**
 * \brief Macro to crash the compilation if the compiler instantiates a template function but should not.
 * \param TplParam A compile-time parameter to the template function used to crash the compilation.
 */
#define SPARK_STATIC_UNREACHABLE(TplParam, Message) \
    static_assert(!std::is_same_v<TplParam, TplParam>, Message)

/**
 * \brief Macro to execute a compiler-specific pragma.
 * \param pragma The pragma to execute.
 */
#if defined(SPARK_COMPILER_MSVC)
#define SPARK_DO_PRAGMA(pragma) __pragma(pragma)
#elif defined(SPARK_COMPILER_GCC) || defined(SPARK_COMPILER_CLANG) || defined(SPARK_COMPILER_CLANG_ANALYSER)
    #define SPARK_DO_PRAGMA(pragma) _Pragma(#pragma)
#else
    #define SPARK_DO_PRAGMA(pragma)
#endif

/**
 * \brief Macro to push/pop warning settings.
 */
#if defined(SPARK_COMPILER_MSVC)
#define SPARK_WARNING_PUSH SPARK_DO_PRAGMA(warning(push))
#define SPARK_WARNING_POP  SPARK_DO_PRAGMA(warning(pop))
#elif defined(SPARK_COMPILER_GCC)
    #define SPARK_WARNING_PUSH SPARK_DO_PRAGMA(GCC diagnostic push)
    #define SPARK_WARNING_POP  SPARK_DO_PRAGMA(GCC diagnostic pop)
#elif defined(SPARK_COMPILER_CLANG) || defined(SPARK_COMPILER_CLANG_ANALYSER)
    #define SPARK_WARNING_PUSH SPARK_DO_PRAGMA(clang diagnostic push)
    #define SPARK_WARNING_POP  SPARK_DO_PRAGMA(clang diagnostic pop)
#else
    #define SPARK_WARNING_PUSH
    #define SPARK_WARNING_POP
#endif

/**
 * \brief Macro to disable/suppress compiler warnings.
 * \param warningValue The warning number or name to disable/suppress.
 */
#ifdef SPARK_COMPILER_MSVC
#define SPARK_DISABLE_MSVC_WARNING(warningValue) SPARK_DO_PRAGMA(warning(disable: warningValue))
#define SPARK_SUPPRESS_MSVC_WARNING(warningValue) SPARK_DO_PRAGMA(warning(suppress: warningValue))
#define SPARK_DISABLE_GCC_WARNING(warningValue)
#define SPARK_DISABLE_CLANG_WARNING(warningValue)
#elif defined(SPARK_COMPILER_GCC)
    #define SPARK_DISABLE_MSVC_WARNING(warningValue)
    #define SPARK_SUPPRESS_MSVC_WARNING(warningValue)
    #define SPARK_DISABLE_GCC_WARNING(warningValue) SPARK_DO_PRAGMA(GCC diagnostic ignored #warningValue)
    #define SPARK_DISABLE_CLANG_WARNING(warningValue)
#elif defined(SPARK_COMPILER_CLANG)
    #define SPARK_DISABLE_MSVC_WARNING(warningValue)
    #define SPARK_SUPPRESS_MSVC_WARNING(warningValue)
    #define SPARK_DISABLE_GCC_WARNING(warningValue)
    #define SPARK_DISABLE_CLANG_WARNING(warningValue) SPARK_DO_PRAGMA(clang diagnostic ignored #warningValue)
#else
    #define SPARK_DISABLE_MSVC_WARNING(warningValue)
    #define SPARK_SUPPRESS_MSVC_WARNING(warningValue)
    #define SPARK_DISABLE_GCC_WARNING(warningValue)
    #define SPARK_DISABLE_CLANG_WARNING(warningValue)
#endif

/**
 * * \brief Macro to make a debugger break.
 */
#ifdef SPARK_COMPILER_MSVC
#define SPARK_DEBUG_BREAK __debugbreak()
#elif defined(SPARK_COMPILER_GCC) || defined(SPARK_COMPILER_CLANG)
    #define SPARK_DEBUG_BREAK __builtin_trap()
#else
    #define SPARK_DEBUG_BREAK
#endif

/**
 * \brief Macro to offset bits. Used in enums definitions.
 * \param X The number of bits to offset.
 */
#define BIT(X) (1 << X)

/**
 * \brief Defines the bitwise operators for an enum class. (operator|, operator|=, operator&, operator&=)
 * \param T The enum class type.
 */
#define SPARK_DEFINE_ENUM_FLAGS(T)                                                                                                                                                                \
    inline T operator| (const T lhs, const T rhs) { using _base_t = std::underlying_type_t<T>; return static_cast<T>(static_cast<_base_t>(lhs) | static_cast<_base_t>(rhs)); }  \
    inline T& operator|= (T& lhs, const T& rhs) { lhs = lhs | rhs; return lhs; }                                                                                                \
    inline T operator& (const T lhs, const T rhs) { using _base_t = std::underlying_type_t<T>; return static_cast<T>(static_cast<_base_t>(lhs) & static_cast<_base_t>(rhs)); }  \
    inline T& operator&= (T& lhs, const T& rhs) { lhs = lhs & rhs; return lhs; }

#define SPARK_OFFSET_OF(struct, member) ((std::size_t)&reinterpret_cast<char const volatile&>((((struct*)0)->member)))

/*
 * Define assertions macros
 * TODO: Add support for custom assertion handlers
 * TODO: Add support for custom assertion messages
 * TODO: Add support for disabling assertions globally
*/
#define SPARK_CORE_ASSERT(cond)                                 \
    if (!(cond))                                                \
    {                                                           \
        spark::log::critical("Assertion Failed: {0}", #cond);   \
        SPARK_DEBUG_BREAK;                                      \
    }

#define SPARK_ASSERT(cond)                                      \
    if (!(cond))                                                \
    {                                                           \
        spark::log::critical("Assertion Failed: {0}", #cond);   \
        SPARK_DEBUG_BREAK;                                      \
    }
