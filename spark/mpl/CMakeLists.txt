set (TARGET_NAME ${SPARK_NAME}_mpl)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

spark_add_library(${TARGET_NAME} INTERFACE
    NO_INSTALL
    PUBLIC_HEADERS
    ${HEADER_DIR}/${SPARK_NAME}/mpl/typelist.h
    ${HEADER_DIR}/${SPARK_NAME}/mpl/index_list.h

    ${HEADER_DIR}/${SPARK_NAME}/mpl/details/type_seq.h
)
