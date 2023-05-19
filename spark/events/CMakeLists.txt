set (TARGET_NAME ${SPARK_NAME}_events)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Event.cpp
        ${SOURCE_DIR}/EventDispatcher.cpp
        ${SOURCE_DIR}/KeyEvents.cpp
        ${SOURCE_DIR}/MouseEvents.cpp
        ${SOURCE_DIR}/WindowEvents.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/events/Event.h
        ${HEADER_DIR}/${SPARK_NAME}/events/EventDispatcher.h
        ${HEADER_DIR}/${SPARK_NAME}/events/KeyEvents.h
        ${HEADER_DIR}/${SPARK_NAME}/events/MouseEvents.h
        ${HEADER_DIR}/${SPARK_NAME}/events/WindowEvents.h

        ${HEADER_DIR}/${SPARK_NAME}/events/impl/EventDispatcher.h
)

target_link_libraries(${TARGET_NAME}
	PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
)
