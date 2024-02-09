#include "gtest/gtest.h"

/// \brief The number of command line arguments
int g_argc = 0;

/// \brief The command line arguments
char** g_argv = nullptr;

int main(int argc, char* argv[])
{
    // Store the command line arguments for later use in the tests
    g_argc = argc;
    g_argv = argv;

    // Init GTest and run all tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
