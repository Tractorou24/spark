#include "spark/path/Paths.h"

#include "gtest/gtest.h"

extern int g_argc;
extern char** g_argv;

namespace spark::mpl::testing
{
    TEST(PathsShould, getTheExecutablePath)
    {
        const std::filesystem::path path = spark::path::executable_path();

        EXPECT_TRUE(std::filesystem::is_directory(path));
        EXPECT_TRUE(std::filesystem::exists(path));
        EXPECT_EQ(path, std::filesystem::path(g_argv[0]).parent_path());
    }

    TEST(PathsShould, getTheExecutableName)
    {
        const std::string name = spark::path::executable_name();

        EXPECT_FALSE(name.empty());
        EXPECT_EQ(name, std::filesystem::path(g_argv[0]).stem());
    }
}
