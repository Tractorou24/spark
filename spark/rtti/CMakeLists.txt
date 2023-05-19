set (TARGET_NAME ${SPARK_NAME}_rtti)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/ExportSymbols.cpp
        ${SOURCE_DIR}/RttiBase.cpp
        ${SOURCE_DIR}/RttiDatabase.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/rtti/HasRtti.h
        ${HEADER_DIR}/${SPARK_NAME}/rtti/Rtti.h
        ${HEADER_DIR}/${SPARK_NAME}/rtti/RttiBase.h
        ${HEADER_DIR}/${SPARK_NAME}/rtti/RttiDatabase.h

        ${HEADER_DIR}/${SPARK_NAME}/rtti/impl/Rtti.h
        ${HEADER_DIR}/${SPARK_NAME}/rtti/impl/RttiDatabase.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_mpl
    PRIVATE
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
)
