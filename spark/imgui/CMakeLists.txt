find_package(glfw3 QUIET REQUIRED)
find_package(imgui QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_imgui)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/ImGui.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/imgui/ImGui.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
    PRIVATE
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_render
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_render_vk
        glfw
        imgui::imgui
)
