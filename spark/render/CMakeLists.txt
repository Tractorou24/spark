set (TARGET_NAME ${SPARK_NAME}_render)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

spark_add_library(${TARGET_NAME} INTERFACE
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/render/Resource.h

        ${HEADER_DIR}/${SPARK_NAME}/render/impl/Resource.h
)
