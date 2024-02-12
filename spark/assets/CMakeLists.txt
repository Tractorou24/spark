#
# ----- SPARK ASSETS -----
# 
# This package allows to manage engine assets, which are used by the engine.
# All of this without having to manually copy them into each game.

set(TARGET_NAME ${SPARK_NAME}_assets)

set(ASSET_FOLDERS 
    shaders
)

#########
# Helper function that setup targets tpo create symbolic links in the given directory
# spark_core_assets_define_symbolic_links(
#  target folder
# )
#########
function(spark_core_assets_define_symbolic_links target_name root_folder)
    foreach(ASSET_FOLDER IN LISTS ASSET_FOLDERS)
        file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_FOLDER} ${root_folder}/${ASSET_FOLDER} SYMBOLIC)
        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_FOLDER} DESTINATION ${CMAKE_INSTALL_BINDIR}/${target_name})
    endforeach()
endfunction()

# Create the output directory and add the links to the core assets
block()
    set(TARGET_PATH "")
    get_property(SPARK_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (SPARK_IS_MULTI_CONFIG)
        foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${TYPE} TYPE_UPPER)
            set(TARGET_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TYPE_UPPER}}/${TARGET_NAME})
            file(MAKE_DIRECTORY ${TARGET_PATH})
            spark_core_assets_define_symbolic_links(${TARGET_NAME} ${TARGET_PATH})
        endforeach()
    else()
        set(TARGET_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME})
        file(MAKE_DIRECTORY ${TARGET_PATH})
        spark_core_assets_define_symbolic_links(${TARGET_NAME} ${TARGET_PATH})
    endif()
endblock()
