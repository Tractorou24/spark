#include "spark/path/Paths.h"

#include "spark/base/Macros.h"
#include "spark/base/Platforms.h"

#ifdef SPARK_OS_WINDOWS
#   include <Windows.h>
#else
#   include <dlfcn.h>
#endif

namespace spark::path
{
    [[nodiscard]] std::filesystem::path executable_path()
    {
#ifdef SPARK_OS_WINDOWS
        // Get the module handle of the library containing this function.
        HMODULE hm = nullptr;
        const BOOL success = GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                                reinterpret_cast<LPWSTR>(&executable_path),
                                                &hm);
        if (!success)
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "Failed to get module handle");

        // Get the full path to the executable executing this code using Win32 API.
        WCHAR wide_path_buffer[MAX_PATH];
        const DWORD path_size = GetModuleFileNameW(hm, wide_path_buffer, MAX_PATH);
        if (path_size == 0 || (path_size == MAX_PATH && GetLastError() == ERROR_INSUFFICIENT_BUFFER))
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "Failed to get module filename");

        // Convert std::wstring to std::filesystem::path
        auto result = std::filesystem::path(wide_path_buffer).parent_path();
#else
        Dl_info dl_info;
        dladdr(reinterpret_cast<void*>(&executable_path), &dl_info);
        auto result = std::filesystem::path(std::string(dl_info.dli_fname)).parent_path().parent_path().parent_path();
#endif
        SPARK_ASSERT(std::filesystem::is_directory(result))
        return result;
    }

    [[nodiscard]] std::filesystem::path assets_path()
    {
        return executable_path() / "assets";
    }
}
