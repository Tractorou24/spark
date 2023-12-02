set (TARGET_NAME ${SPARK_NAME}_render)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

spark_add_library(${TARGET_NAME}
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/render/GraphicsAdapter.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Resource.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Surface.h

        ${HEADER_DIR}/${SPARK_NAME}/render/impl/Resource.h
    CXX_SOURCES
        ${SOURCE_DIR}/ExportSymbols.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_lib
)
