set (TARGET_NAME ${SPARK_NAME}_render)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/render/Buffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/DeviceMemory.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Format.h
        ${HEADER_DIR}/${SPARK_NAME}/render/GraphicsAdapter.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Image.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Mappable.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Resource.h
        ${HEADER_DIR}/${SPARK_NAME}/render/StateResource.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Surface.h
        ${HEADER_DIR}/${SPARK_NAME}/render/SwapChain.h

        ${HEADER_DIR}/${SPARK_NAME}/render/impl/Resource.h
    CXX_SOURCES
        ${SOURCE_DIR}/Buffer.cpp
        ${SOURCE_DIR}/ExportSymbols.cpp
        ${SOURCE_DIR}/StateResource.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_lib
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_math
)
