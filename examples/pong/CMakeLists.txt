set (TARGET_NAME pong)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_executable(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/main.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/pong/Ball.h
        ${HEADER_DIR}/pong/GameManager.h
        ${HEADER_DIR}/pong/Paddle.h
        ${HEADER_DIR}/pong/ui/Background.h
        ${HEADER_DIR}/pong/ui/Button.h
        ${HEADER_DIR}/pong/ui/Menu.h
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_audio
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_core
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_engine
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_events
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_path
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_patterns
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
)

# Create a link to the assets folder in the output directory
add_custom_target(${TARGET_NAME}_assets ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink 
                  ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}_assets
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_INSTALL_BINDIR}/${TARGET_NAME}_assets)
