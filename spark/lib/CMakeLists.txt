set (TARGET_NAME ${SPARK_NAME}_lib)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Clock.cpp
        ${SOURCE_DIR}/Random.cpp
        ${SOURCE_DIR}/Uuid.cpp
        ${SOURCE_DIR}/UuidGenerator.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/lib/Clock.h
        ${HEADER_DIR}/${SPARK_NAME}/lib/Overloaded.h
        ${HEADER_DIR}/${SPARK_NAME}/lib/Random.h
        ${HEADER_DIR}/${SPARK_NAME}/lib/Uuid.h
        ${HEADER_DIR}/${SPARK_NAME}/lib/UuidGenerator.h

        ${HEADER_DIR}/${SPARK_NAME}/lib/details/Uuid.h

        ${HEADER_DIR}/${SPARK_NAME}/lib/impl/Clock.h
        ${HEADER_DIR}/${SPARK_NAME}/lib/impl/Random.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
)
