#########
# Include guard to prevent repeated library definition
#########
include_guard(GLOBAL)

#########
# Function to setup an alternative linker on Linux
# This function will try to find lld as preferred linker and
# will search for gold as fallback.
# _spark_setup_linker(compiler_linker_options_target)
#########
function(_spark_setup_linker compiler_linker_options_target) # lld for Clang and GNU gold for GCC
    if (NOT LINUX)
        return()
    endif()

    # Finds lld as preferred alternative linker.
    find_program(LLD_PROGRAM
        NAMES
            lld
            ld.lld
    )

    if(LLD_PROGRAM)
        # By default LLD uses all system threads.
        # This could be tweaked for versions 11+ (LINKER:--threads=1), but cannot be disabled for older versions
        message(STATUS "Set linker to LLD (multi-threaded): ${LLD_PROGRAM}")
        # Linker options
        target_link_options(${compiler_linker_options_target} INTERFACE
            # Change linker to lld
            -fuse-ld=lld
        )
    else() # Try to fallback with gold
        find_program(GNU_GOLD_PROGRAM
            NAMES
                gold
                ld.gold
        )
        if (GNU_GOLD_PROGRAM)
            include(ProcessorCount)
            ProcessorCount(HOST_PROC_COUNT)
            message(STATUS "Set linker to GNU gold: ${GNU_GOLD_PROGRAM}, using threads: ${HOST_PROC_COUNT}")
            target_link_options(${compiler_linker_options_target} INTERFACE
                # Change linker to gold
                -fuse-ld=gold
                LINKER:--threads,--thread-count=${HOST_PROC_COUNT}
            )
        endif()
    endif()

    # Improve debugger load time if an alternative linker was found
    if (LLD_PROGRAM OR GNU_GOLD_PROGRAM)
        target_link_options(${compiler_linker_options_target} INTERFACE
            $<$<CONFIG:Debug,RelWithDebInfo>:LINKER:--gdb-index>
        )
    endif()
endfunction()

#########
# Target to hold compiler and linker flags used throughout the project
#########
block()
    set(_spark_compiler_linker_opt_target "spark_compiler_linker_options")
    add_library(${_spark_compiler_linker_opt_target} INTERFACE)

    # Internal variable to simplify generator expression writing
    set(_c_cxx_lang "$<COMPILE_LANGUAGE:C,CXX>")

    if(MSVC)
        # Compiler options
        target_compile_options(${_spark_compiler_linker_opt_target} INTERFACE
            # Enable warning level to W4
            $<${_c_cxx_lang}:/W4>
            # Increase object file's address capacity to 2^32
            $<${_c_cxx_lang}:/bigobj>
            # Activate standard conformance mode
            $<${_c_cxx_lang}:/permissive->
            # Enable conforming lambda grammar and processing support
            $<${_c_cxx_lang}:/Zc:lambda>
            # Have an updated value of __cplusplus preprocessor macro
            $<${_c_cxx_lang}:/Zc:__cplusplus>
            # Assume operator new throws (no throw is a very old MSVC behavior)
            $<${_c_cxx_lang}:/Zc:throwingNew>
            # Enable new conforming preprocessor
            $<${_c_cxx_lang}:/Zc:preprocessor>
        )

        # Change debug info generation to Embedded. Mandatory to support ccache.
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>" PARENT_SCOPE)

        # Linker options
        target_link_options(${_spark_compiler_linker_opt_target} INTERFACE
            # Add support of addresses >= 2GB
            /LARGEADDRESSAWARE
        )
    else() # Linux
        # Compiler options
        target_compile_options(${_spark_compiler_linker_opt_target} INTERFACE
            # Enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.
            $<${_c_cxx_lang}:-Wall>
            -march=native
        )

        # Define installation directories variables using GNU Standard.
        include(GNUInstallDirs)
        # Ensure correct rpath on binaries at build and install step
        file(RELATIVE_PATH relDir
            ${CMAKE_INSTALL_FULL_BINDIR}
            ${CMAKE_INSTALL_FULL_LIBDIR}
        )
        set(CMAKE_INSTALL_RPATH "$ORIGIN" "$ORIGIN/${relDir}" PARENT_SCOPE)
        set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE PARENT_SCOPE)
    endif()

    # Setup alternative linker if possible
    _spark_setup_linker(${_spark_compiler_linker_opt_target})

    add_library(${CMAKE_PROJECT_NAME}::Compiler_Linker_options ALIAS ${_spark_compiler_linker_opt_target})

    # Set hidden visibility for symbols (especially for gcc-like compilers)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden PARENT_SCOPE)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 1 PARENT_SCOPE)
endblock()
