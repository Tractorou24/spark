set (TARGET_NAME ${SPARK_NAME}_path)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Paths.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/path/Paths.h
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
)
