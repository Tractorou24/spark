#########
# Include guard to prevent repeated library definition
#########
include_guard(GLOBAL)

#########
# This sets the default config for single and multi-config generators
#########
block()
    get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(NOT CMAKE_BUILD_TYPE AND NOT IS_MULTI_CONFIG)
        set(default_build_type "Debug")
        message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
        set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    endif()

    # Define acceptable values for build type in mono-config (for CMake GUI)
    if(CMAKE_BUILD_TYPE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
    endif()

    # Define values for multi config builds
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "Build types for multi-config generators")
endblock()
