find_package(spdlog CONFIG REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_log_executor)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_executable(${TARGET_NAME}
    NO_INSTALL
    CXX_SOURCES
        ${SOURCE_DIR}/main.cpp
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
)
