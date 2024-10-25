find_package(imgui QUIET REQUIRED)

set (TARGET_NAME pathfinding)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_executable(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Cell.cpp
        ${SOURCE_DIR}/Grid.cpp
        ${SOURCE_DIR}/GameManager.cpp
        ${SOURCE_DIR}/main.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/pathfinding/Cell.h
        ${HEADER_DIR}/pathfinding/Grid.h
        ${HEADER_DIR}/pathfinding/GameManager.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_core
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_events
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_path
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_patterns
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
    PRIVATE
        imgui::imgui
)
