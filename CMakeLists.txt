cmake_minimum_required(VERSION 3.25)

# Add our CMake modules files in the CMake path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Enable folders for IDE projects generation (e.g Visual Studio)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Setup build type
include(SPARKBuildType)

# Setup vcpkg
include(SPARKVcpkg)

project(SPARK VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ version used throughout the project
set(CMAKE_CXX_STANDARD 20)
# Enforce C++ standard as required for the compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Disable C++ compiler extension (like gnu) to ensure portable code
set(CMAKE_CXX_EXTENSIONS OFF)
# Enable warning as errors on all targets
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Enable CTest (and create variable BUILD_TESTING)
include(CTest)

# Define installation directories variables using GNU Standard.
include(GNUInstallDirs)

# Declare SPARK general options
option(BUILD_SHARED_LIBS "Build shared libraries. Static libraries otherwise" ON)
option(SPARK_EXAMPLES_ENABLED "Build SPARK examples" ON)
option(SPARK_EXAMPLES_IN_ALL "Build SPARK examples with ALL target" ON)
option(SPARK_EXPERIMENTAL_ENABLED "Build SPARK experimental features" OFF)
option(SPARK_EXPERIMENTAL_IN_ALL "Build SPARK experimental features with ALL target" OFF)

set(SPARK_OUTPUT_DIR ${CMAKE_BINARY_DIR}/_output)

block()
    get_property(SPARK_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (SPARK_IS_MULTI_CONFIG)
        # Setup build output directories
        foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${TYPE} TYPE_UPPER)
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TYPE_UPPER} ${SPARK_OUTPUT_DIR}/${TYPE}/${CMAKE_INSTALL_LIBDIR} PARENT_SCOPE)
            set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TYPE_UPPER} ${SPARK_OUTPUT_DIR}/${TYPE}/${CMAKE_INSTALL_LIBDIR} PARENT_SCOPE)
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TYPE_UPPER} ${SPARK_OUTPUT_DIR}/${TYPE}/${CMAKE_INSTALL_BINDIR} PARENT_SCOPE)
        endforeach()
    else()
        set(ARCHIVE_OUTPUT_DIRECTORY ${SPARK_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_INSTALL_LIBDIR})
        set(LIBRARY_OUTPUT_DIRECTORY ${SPARK_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_INSTALL_LIBDIR})
        set(RUNTIME_OUTPUT_DIRECTORY ${SPARK_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_INSTALL_BINDIR})

        # Setup build output directories
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY} PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY} PARENT_SCOPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY} PARENT_SCOPE)

        # Setup also per-config build output directories to simplify generator expression writing when needed.
        string(TOUPPER ${CMAKE_BUILD_TYPE} TYPE_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TYPE_UPPER} ${ARCHIVE_OUTPUT_DIRECTORY} PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TYPE_UPPER} ${LIBRARY_OUTPUT_DIRECTORY} PARENT_SCOPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TYPE_UPPER} ${RUNTIME_OUTPUT_DIRECTORY} PARENT_SCOPE)
    endif()

    # Check compiler version
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13.1")
            message(FATAL_ERROR "Insufficient GCC version (required>=13.1)")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.31")
            message(FATAL_ERROR "Insufficient MSVC version (required>=19.31)")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
            message(FATAL_ERROR "Insufficient CLANG version (required>=16.0)")
        endif()
    else()
        message(WARNING "You are using an unsupported compiler! Compilation has only been tested with MSVC, GCC and CLANG.")
    endif()
endblock()

# Include our internal API for target definition
include(SPARKTarget)

# Set a default property for all targets so they are built by ALL
define_property(DIRECTORY PROPERTY DIR_IN_ALL INHERITED)
set_directory_properties(PROPERTIES DIR_IN_ALL ON)

set(SPARK_NAME spark)
add_subdirectory(${SPARK_NAME})

if (SPARK_EXPERIMENTAL_ENABLED)
    set(EXPERIMENTAL_NAME experimental)
    add_subdirectory(${EXPERIMENTAL_NAME})
endif()

if (SPARK_EXAMPLES_ENABLED)
    set(EXAMPLES_NAME examples)
    add_subdirectory(${EXAMPLES_NAME})
endif()
