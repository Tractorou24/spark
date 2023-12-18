set (TARGET_NAME ${SPARK_NAME}_math)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

spark_add_library(${TARGET_NAME} INTERFACE
    NO_INSTALL
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/math/Vector2.h
        ${HEADER_DIR}/${SPARK_NAME}/math/Vector3.h

        ${HEADER_DIR}/${SPARK_NAME}/math/details/Vector.h
        ${HEADER_DIR}/${SPARK_NAME}/math/impl/Vector2.h
        ${HEADER_DIR}/${SPARK_NAME}/math/impl/Vector3.h
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
