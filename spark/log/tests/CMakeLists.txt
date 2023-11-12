find_package(GTest QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_log_tests)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_test_executable(${TARGET_NAME}
    GTEST_DISCOVER
    CXX_SOURCES
        ${SOURCE_DIR}/LoggerTests.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/ExecutorHelpers.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_path
		GTest::gtest_main
        fmt::fmt
)

add_subdirectory(executor)
