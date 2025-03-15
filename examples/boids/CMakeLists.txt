find_package(imgui QUIET REQUIRED)

set (TARGET_NAME boids)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_executable(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Bird.cpp
        ${SOURCE_DIR}/BoidsManager.cpp
        ${SOURCE_DIR}/main.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/boids/Bird.h
        ${HEADER_DIR}/boids/BoidsManager.h
        ${HEADER_DIR}/boids/SimulationData.h
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_audio
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_core
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_events
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
    PRIVATE
        imgui::imgui
)
