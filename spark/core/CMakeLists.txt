find_package(SFML COMPONENTS system window graphics QUIET REQUIRED)
find_package(Vulkan QUIET REQUIRED)
find_package(glfw3 QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_core)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Application.cpp
        ${SOURCE_DIR}/Input.cpp
        ${SOURCE_DIR}/SceneManager.cpp
        ${SOURCE_DIR}/Window.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/core/Application.h
        ${HEADER_DIR}/${SPARK_NAME}/core/ApplicationBuilder.h
        ${HEADER_DIR}/${SPARK_NAME}/core/EntryPoint.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Input.h
        ${HEADER_DIR}/${SPARK_NAME}/core/SceneManager.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Window.h
        ${HEADER_DIR}/${SPARK_NAME}/core/impl/ApplicationBuilder.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_events
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_engine
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_lib
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_math
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_patterns
    PRIVATE
        glfw
        sfml-system
        sfml-window
        sfml-graphics
        Vulkan::Vulkan
)
