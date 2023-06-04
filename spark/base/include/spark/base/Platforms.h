#pragma once

// @formatter:off

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Target OS is Windows
    #define SPARK_OS_WINDOWS 1
    #ifdef _WIN64
        #if defined(_MSC_VER)
            // Compiler is MSVC
            #define SPARK_COMPILER_MSVC 1
        #else
            #error "Unknown/unsupported compiler"
        #endif
    #else
        #error "32-bit Windows platform is not supported"
    #endif
#elif __APPLE__
    #error "Apple platform is not supported"
#elif defined(unix) | defined(__unix) | defined(__unix__)
    #ifdef __linux__
        // Target OS is Linux
        #define SPARK_OS_LINUX 1
        #if defined(__clang__)
            // Compiler is Clang
            #define SPARK_COMPILER_CLANG 1
        #elif defined(__GNUC__) || defined(__GNUG__)
            // Compiler is GCC
            #define SPARK_COMPILER_GCC 1
        #else
            #error "Unknown/unsupported UNIX platform"
        #endif
    #endif
#else
    #error "Unknown and unsupported platform"
#endif

// @formatter:on
