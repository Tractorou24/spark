find_package(spdlog CONFIG REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_log)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Logger.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/log/Logger.h
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
        spdlog::spdlog
)
