find_package(GTest QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_mpl_tests)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_test_executable(${TARGET_NAME}
    GTEST_DISCOVER
    CXX_SOURCES
        ${SOURCE_DIR}/TypelistTests.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_mpl
    GTest::gtest_main
)
