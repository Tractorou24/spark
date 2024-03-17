find_package(Boost QUIET REQUIRED)

set (TARGET_NAME ${EXPERIMENTAL_NAME}_ser)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/FileSerializer.cpp
        ${SOURCE_DIR}/MemorySerializer.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/AbstractSerializer.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/BinarySerializer.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/FileSerializer.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/MemorySerializer.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/SerializationRegistry.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/SerializerScheme.h

        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/impl/SerializerScheme.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/impl/BinarySerializer.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/impl/SerializationRegistry.h
        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/impl/SerializerScheme.h

        ${HEADER_DIR}/${EXPERIMENTAL_NAME}/ser/details/SerializerScheme.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_mpl
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
        Boost::headers
)
