set (TARGET_NAME ${SPARK_NAME}_patterns)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME} INTERFACE
    NO_INSTALL
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/patterns/Composite.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/Factory.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/Signal.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/Singleton.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/Slot.h

        ${HEADER_DIR}/${SPARK_NAME}/patterns/details/Connection.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/details/Creators.h

        ${HEADER_DIR}/${SPARK_NAME}/patterns/impl/Composite.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/impl/Factory.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/impl/Signal.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/impl/Singleton.h
        ${HEADER_DIR}/${SPARK_NAME}/patterns/impl/Slot.h
)

target_link_libraries(${TARGET_NAME}
    INTERFACE
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
)
