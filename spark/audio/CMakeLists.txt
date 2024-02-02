find_package(SFML COMPONENTS audio QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_audio)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Sound.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/audio/Sound.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
    PRIVATE
        sfml-audio
)
