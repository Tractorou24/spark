#########
# Include guard to prevent repeated library definition
#########
include_guard(GLOBAL)

#########
# Include generic utilities
#########
include(GenerateExportHeader)
include(GNUInstallDirs)
include(SPARKCompilerFlags)

#########
# Internal helper function that setup runtime path following the target type
# _spark_set_executable_rpath(
#   target
#   <LIBRARY | EXECUTABLE>
# )
#########
function(_spark_set_executable_rpath target)
    # Define the supported set of keywords
    set(options EXECUTABLE LIBRARY)
    set(one_value_keywords "")
    set(multi_value_keywords "")

    # Parse arguments
    cmake_parse_arguments(RPATHOPT
        "${options}"
        "${one_value_keywords}"
        "${multi_value_keywords}"
        ${ARGN}
    )

    if (RPATHOPT_EXECUTABLE)
        if (NOT DEFINED CACHE{SPARK_EXECUTABLE_RPATH})
            file(RELATIVE_PATH bin_to_lib_rel_dir
                ${CMAKE_INSTALL_FULL_BINDIR}
                ${CMAKE_INSTALL_FULL_LIBDIR}
            )
            set(SPARK_EXECUTABLE_RPATH "$ORIGIN" "$ORIGIN/${bin_to_lib_rel_dir}" CACHE INTERNAL "RPATH for SPARK executable")
        endif()
        set(runtime_path "${SPARK_EXECUTABLE_RPATH}")
    elseif(RPATHOPT_LIBRARY)
        set(runtime_path "$ORIGIN")
    else()
        message(WARNING "Cannot set runtime path on ${target}, unkwown target type")
    endif()

    set_target_properties(${target} PROPERTIES
        INSTALL_RPATH "${runtime_path}"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endfunction()

#########
# Internal helper function that adds a new target
# _spark_add_target(
#   target
#   <LIBRARY | EXECUTABLE>
#   [TYPE <STATIC | SHARED | MODULE | INTERFACE | OBJECT>] # Mandatory with LIBRARY
#   [EXCLUDE_FROM_ALL]
#   [NO_INSTALL]
#   [CXX_SOURCES <cxx_source_list>]
#   [PUBLIC_HEADERS <public_header_list>]
#   [PRIVATE_HEADERS <private_header_list>]
# )
#########
function(_spark_add_target target)
    # Define the supported set of keywords
    set(options EXECUTABLE LIBRARY EXCLUDE_FROM_ALL NO_INSTALL)
    set(one_value_keywords TYPE)
    set(multi_value_keywords CXX_SOURCES PUBLIC_HEADERS PRIVATE_HEADERS)

    # Parse arguments
    cmake_parse_arguments(TARGET
        "${options}"
        "${one_value_keywords}"
        "${multi_value_keywords}"
        ${ARGN}
    )

    # Error cases handling
    if(NOT TARGET_EXECUTABLE AND NOT TARGET_LIBRARY)
        message(FATAL_ERROR "EXECUTABLE or LIBRARY must be set")
    endif()

    if((TARGET_EXECUTABLE AND TARGET_TYPE) OR (TARGET_LIBRARY AND NOT TARGET_TYPE))
        message(FATAL_ERROR "TYPE argument must be set only for a LIBRARY target")
    endif()

    if (TARGET_EXCLUDE_FROM_ALL)
        set(target_exclude_option "EXCLUDE_FROM_ALL")
    endif()

    # Target definition
    string(TOLOWER ${target} target_lower)
    if(TARGET_EXECUTABLE)
        add_executable(${target}
            ${target_exclude_option}
            ${TARGET_CXX_SOURCES}
        )
    else() # TARGET_LIBRARY
        add_library(${target}
            ${TARGET_TYPE}
            ${target_exclude_option}
            ${TARGET_CXX_SOURCES}
        )

        # If target is built (e.g SHARED, STATIC or MODULE)
        list(APPEND SYMBOL_EXPORTABLE_TARGETS "SHARED" "STATIC" "MODULE")
        if ("${TARGET_TYPE}" IN_LIST SYMBOL_EXPORTABLE_TARGETS)
            string(TOUPPER ${target} target_upper)
            string(TOLOWER ${target} target_lower)

            # Generate symbols export header
            cmake_path(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR BASE_DIRECTORY ${SPARK_SOURCE_DIR} OUTPUT_VARIABLE TARGET_PATH)

            generate_export_header(${target}
                BASE_NAME
                    ${target}
                EXPORT_FILE_NAME
                    ${CMAKE_CURRENT_BINARY_DIR}/include/${TARGET_PATH}/Export.h
            )

            target_sources(${target}
                PUBLIC
                    FILE_SET ${target_lower}_public_headers
                    TYPE HEADERS
                    FILES
                        ${CMAKE_CURRENT_BINARY_DIR}/include/${TARGET_PATH}/Export.h
                    BASE_DIRS
                        ${CMAKE_CURRENT_BINARY_DIR}/include
            )

            if ("${TARGET_TYPE}" EQUAL "STATIC" )
                target_compile_definitions(${target}
                    PUBLIC
                        ${target_upper}_STATIC_DEFINE
                )
            endif()

            # Add a private define to detect if we are building the library
            # or using it.
            target_compile_definitions(${target}
                PRIVATE
                    ${target_upper}_LIB
            )
        endif()

        add_library(${CMAKE_PROJECT_NAME}::${target} ALIAS ${target})
    endif()

    # Add headers
    foreach(VISIBILITY IN ITEMS PUBLIC PRIVATE)
        string(TOLOWER ${VISIBILITY} visibility_lower)
        if ("${VISIBILITY}" STREQUAL "PRIVATE")
            set(INCLUDE_DIR_SUFFIX "_p")
        endif()
        if (TARGET_${VISIBILITY}_HEADERS)
            target_sources(${target}
                ${VISIBILITY}
                    FILE_SET ${target_lower}_${visibility_lower}_headers
                    TYPE HEADERS
                    FILES
                        ${TARGET_${VISIBILITY}_HEADERS}
                    BASE_DIRS
                        ${CMAKE_CURRENT_SOURCE_DIR}/include${INCLUDE_DIR_SUFFIX}
            )
        endif()
    endforeach()

    # If target is built (e.g EXECUTABLE, SHARED, STATIC, OBJECT or MODULE)
    if (TARGET_EXECUTABLE OR NOT "${TARGET_TYPE}" STREQUAL "INTERFACE")
        # Add compiler flags to target
        if (TARGET ${CMAKE_PROJECT_NAME}::Compiler_Linker_options)
            target_link_libraries(${target}
                PRIVATE
                    ${CMAKE_PROJECT_NAME}::Compiler_Linker_options
            )
        endif()

        # Use MTT for parallel jobs with MSBuild instead of /MP
        # This is mandatory for compiler cache usage with MSBuild
        # since we need a compiler command for each source file.
        if (${CMAKE_GENERATOR} MATCHES "Visual Studio")
            set_target_properties(${target} PROPERTIES
                VS_GLOBAL_UseMultiToolTask true # Enable "new" MSBuild scheduler
                VS_GLOBAL_EnforceProcessCountAcrossBuilds true # Prevent over-commitment of MTT
            )
        endif()
    endif()

    # Set folder target property for IDE projects
    spark_target_folder_property(${target})

    # Set EXCLUDE_FROM_ALL property depending on directory (and similar default build property for VS)
    get_directory_property(IS_IN_ALL DIR_IN_ALL)
    if (NOT IS_IN_ALL)
        set_target_properties(${target} PROPERTIES
            EXCLUDE_FROM_ALL TRUE
            EXCLUDE_FROM_DEFAULT_BUILD TRUE
        )
    endif()

    # Add the config macros
    if(NOT "${TARGET_TYPE}" STREQUAL "INTERFACE")
        target_compile_definitions(${target}
            PRIVATE
                $<$<CONFIG:DEBUG>:SPARK_DEBUG>
                $<$<CONFIG:RELWITHDEBINFO>:SPARK_RELWITHDEBINFO>
                $<$<CONFIG:RELEASE>:SPARK_RELEASE>
        )
    endif()

    if (NOT TARGET_NO_INSTALL AND NOT "${TARGET_TYPE}" STREQUAL "MODULE")
        install(TARGETS ${target} RUNTIME LIBRARY ARCHIVE)
        if (MSVC AND "${TARGET_TYPE}" STREQUAL "SHARED")
            # Install pdb files for shared libraries on Windows
            install(FILES $<TARGET_PDB_FILE:${target}>
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                CONFIGURATIONS Debug RelWithDebInfo
            )
        endif()
    endif()
endfunction()

#########
# Helper function that adds a new library
# spark_add_library(
#   target
#   <STATIC | SHARED | MODULE | INTERFACE | OBJECT>
#   [EXCLUDE_FROM_ALL] # Exclude the target from "build all". Cannot be used with OBJECT.
#   [NO_INSTALL] # Do not create install rules for this target
#   [CXX_SOURCES <cxx_source_list>]
#   [PUBLIC_HEADERS <public_header_list>]
#   [PRIVATE_HEADER <private_header_list>]
# )
#
#########
function(spark_add_library target)
    # Define the supported set of keywords
    set(options SHARED STATIC MODULE OBJECT INTERFACE)
    set(one_value_keywords "")
    set(multi_value_keywords "")

    # Parse arguments
    cmake_parse_arguments(LIB
        "${options}"
        "${one_value_keywords}"
        "${multi_value_keywords}"
        ${ARGN}
    )

    # Ensure a good library type
    if (LIB_SHARED)
        set(lib_type SHARED)
    elseif(LIB_STATIC)
        set(lib_type STATIC)
    elseif(LIB_MODULE)
        set(lib_type MODULE)
    elseif(LIB_OBJECT)
        set(lib_type OBJECT)
    elseif(LIB_INTERFACE)
        set(lib_type INTERFACE)
    else()
        # Default follows BUILD_SHARED_LIBS
        if (BUILD_SHARED_LIBS)
            set(lib_type "SHARED")
        else()
            set(lib_type "STATIC")
        endif()
    endif()

    _spark_add_target(${target}
        LIBRARY
        TYPE ${lib_type}
        ${LIB_UNPARSED_ARGUMENTS}
    )

    # Handle RPATH
    _SPARK_set_executable_rpath(${target} LIBRARY)
endfunction()

#########
# Helper function that adds a new executable
# spark_add_executable(
#   target
#   [EXCLUDE_FROM_ALL]
#   [NO_INSTALL] # Do not create install rules for this target
#   [CXX_SOURCES <cxx_source_list>]
#   [PUBLIC_HEADERS <public_header_list>]
#   [PRIVATE_HEADER <private_header_list>]
# )
#########
function(spark_add_executable target)
    # Define the supported set of keywords
    set(options NO_INSTALL)
    set(one_value_keywords "")
    set(multi_value_keywords "")

    # Parse arguments
    cmake_parse_arguments(EXEC
        "${options}"
        "${one_value_keywords}"
        "${multi_value_keywords}"
        ${ARGN}
    )

    # Re-set a variable to forward correctly NO_INSTALL to the internal method.
    if (EXEC_NO_INSTALL)
        set(no_install NO_INSTALL)
    endif()

    _spark_add_target(${target}
        EXECUTABLE
        ${no_install}
        ${EXEC_UNPARSED_ARGUMENTS}
    )

    # Handle RPATH
    _spark_set_executable_rpath(${target} EXECUTABLE)
endfunction()

#########
# Helper function that adds a test executable
# spark_add_test_executable(
#   target
#   [EXCLUDE_FROM_ALL]
#   [GTEST_DISCOVER] # Uses gtest_discover_tests on the executable to populate in CTest
#   [CXX_SOURCES <cxx_source_list>]
#   [PUBLIC_HEADERS <public_header_list>]
#   [PRIVATE_HEADERS <private_header_list>]
# )
#########
function(spark_add_test_executable target)
    # Define the supported set of keywords
    set(options GTEST_DISCOVER)
    set(one_value_keywords "")
    set(multi_value_keywords "")

    # Parse arguments
    cmake_parse_arguments(TEST
        "${options}"
        "${one_value_keywords}"
        "${multi_value_keywords}"
        ${ARGN}
    )

    spark_add_executable(${target}
        NO_INSTALL
        ${TEST_UNPARSED_ARGUMENTS}
    )

    # Put into a "tests" folder in IDE
    spark_target_folder_property(${target} IS_TEST)

    # Add tests in CTest
    if (TEST_GTEST_DISCOVER)
        include(GoogleTest)
        gtest_discover_tests(${target})
    else()
        add_test(NAME ${target} COMMAND ${target})
    endif()
endfunction()

#########
# Set the FOLDER property of a target.
# Folder tree is based on source directory tree.
#
# spark_target_folder_property(target [IS_TEST])
# IS_TEST option put the target into a "tests" folder instead of the final folder.
#########
function(spark_target_folder_property folder_target)
    # Define the supported set of keywords
    set(options IS_TEST)
    set(one_value_keywords "")
    set(multi_value_keywords "")

    # Parse arguments
    cmake_parse_arguments(FOLDER
        "${options}"
        "${one_value_keywords}"
        "${multi_value_keywords}"
        ${ARGN}
    )

    cmake_path(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE VAR_FOLDER)
    cmake_path(GET VAR_FOLDER PARENT_PATH VAR_FOLDER)
    if (FOLDER_IS_TEST)
        cmake_path(GET VAR_FOLDER PARENT_PATH VAR_FOLDER)
        cmake_path(APPEND VAR_FOLDER "tests")
    endif()
    set_property(TARGET ${folder_target} PROPERTY FOLDER ${VAR_FOLDER})
endfunction()
