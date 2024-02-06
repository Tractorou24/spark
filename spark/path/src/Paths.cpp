#include "spark/path/Paths.h"

#include "spark/base/Macros.h"
#include "spark/base/Platforms.h"

#include <format>

#ifdef SPARK_OS_WINDOWS
#   include <Windows.h>
#else
#   include <unistd.h>
#endif

#include <iostream>

namespace
{
    std::filesystem::path executable_full_path()
    {
#ifdef SPARK_OS_WINDOWS
        wchar_t path[FILENAME_MAX] = {0};
        GetModuleFileNameW(nullptr, path, FILENAME_MAX);
        SPARK_ASSERT(std::filesystem::is_regular_file(path));
        return path;
#else
        char tmp_path[FILENAME_MAX];
        const ssize_t count = readlink("/proc/self/exe", tmp_path, FILENAME_MAX);
        const std::filesystem::path path = std::string(tmp_path, (count > 0) ? count : 0);
        SPARK_ASSERT(count != 0 && std::filesystem::is_regular_file(path))
        return path;
#endif
    }
}

namespace spark::path
{
    [[nodiscard]] std::filesystem::path executable_path()
    {
        return executable_full_path().parent_path();
    }

    std::string executable_name()
    {
        return executable_full_path().stem().string();
    }

    [[nodiscard]] std::filesystem::path assets_path()
    {
        return executable_path() / std::format("{}_assets", executable_name());
    }
}
