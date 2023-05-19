find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_engine)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Component.cpp
        ${SOURCE_DIR}/GameObject.cpp
        ${SOURCE_DIR}/Scene.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/engine/Component.h
        ${HEADER_DIR}/${SPARK_NAME}/engine/GameObject.h
        ${HEADER_DIR}/${SPARK_NAME}/engine/Scene.h

        ${HEADER_DIR}/${SPARK_NAME}/engine/components/Circle.h
        ${HEADER_DIR}/${SPARK_NAME}/engine/components/Image.h
        ${HEADER_DIR}/${SPARK_NAME}/engine/components/Rectangle.h
        ${HEADER_DIR}/${SPARK_NAME}/engine/components/Text.h
        ${HEADER_DIR}/${SPARK_NAME}/engine/components/Transform.h

        ${HEADER_DIR}/${SPARK_NAME}/engine/impl/GameObject.h)

target_link_libraries(${TARGET_NAME}
	PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_lib
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_math
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_patterns
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
    PRIVATE
        sfml-system
        sfml-graphics
        sfml-window
)
