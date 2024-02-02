set (TARGET_NAME ${SPARK_NAME}_base)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/ExportSymbols.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/base/BaseType.h
        ${HEADER_DIR}/${SPARK_NAME}/base/Endianness.h
        ${HEADER_DIR}/${SPARK_NAME}/base/Exception.h
        ${HEADER_DIR}/${SPARK_NAME}/base/KeyCodes.h
        ${HEADER_DIR}/${SPARK_NAME}/base/Macros.h
        ${HEADER_DIR}/${SPARK_NAME}/base/MouseCodes.h
        ${HEADER_DIR}/${SPARK_NAME}/base/Platforms.h
        ${HEADER_DIR}/${SPARK_NAME}/base/details/Exception.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
)
