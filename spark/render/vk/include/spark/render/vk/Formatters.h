#pragma once

#include "spark/base/Macros.h"

#include "vulkan/vulkan.h"

#include <format>

SPARK_WARNING_PUSH
SPARK_DISABLE_CLANG_WARNING(-Wswitch) // warning: enumeration value `VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR` not handled in switch
SPARK_DISABLE_GCC_WARNING(-Wswitch) // warning: enumeration value `VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR` not handled in switch

template <>
struct std::formatter<VkResult> : std::formatter<string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const VkResult& result, auto& ctx)
    {
        switch (result)
        {
        case VK_SUCCESS:
            return std::format_to(ctx.out(), "VK_SUCCESS");
        case VK_NOT_READY:
            return std::format_to(ctx.out(), "VK_NOT_READY");
        case VK_TIMEOUT:
            return std::format_to(ctx.out(), "VK_TIMEOUT");
        case VK_EVENT_SET:
            return std::format_to(ctx.out(), "VK_EVENT_SET");
        case VK_EVENT_RESET:
            return std::format_to(ctx.out(), "VK_EVENT_RESET");
        case VK_INCOMPLETE:
            return std::format_to(ctx.out(), "VK_INCOMPLETE");
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return std::format_to(ctx.out(), "VK_ERROR_OUT_OF_HOST_MEMORY");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return std::format_to(ctx.out(), "VK_ERROR_OUT_OF_DEVICE_MEMORY");
        case VK_ERROR_INITIALIZATION_FAILED:
            return std::format_to(ctx.out(), "VK_ERROR_INITIALIZATION_FAILED");
        case VK_ERROR_DEVICE_LOST:
            return std::format_to(ctx.out(), "VK_ERROR_DEVICE_LOST");
        case VK_ERROR_MEMORY_MAP_FAILED:
            return std::format_to(ctx.out(), "VK_ERROR_MEMORY_MAP_FAILED");
        case VK_ERROR_LAYER_NOT_PRESENT:
            return std::format_to(ctx.out(), "VK_ERROR_LAYER_NOT_PRESENT");
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return std::format_to(ctx.out(), "VK_ERROR_EXTENSION_NOT_PRESENT");
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return std::format_to(ctx.out(), "VK_ERROR_FEATURE_NOT_PRESENT");
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return std::format_to(ctx.out(), "VK_ERROR_INCOMPATIBLE_DRIVER");
        case VK_ERROR_TOO_MANY_OBJECTS:
            return std::format_to(ctx.out(), "VK_ERROR_TOO_MANY_OBJECTS");
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return std::format_to(ctx.out(), "VK_ERROR_FORMAT_NOT_SUPPORTED");
        case VK_ERROR_FRAGMENTED_POOL:
            return std::format_to(ctx.out(), "VK_ERROR_FRAGMENTED_POOL");
        case VK_ERROR_UNKNOWN:
            return std::format_to(ctx.out(), "VK_ERROR_UNKNOWN");
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return std::format_to(ctx.out(), "VK_ERROR_OUT_OF_POOL_MEMORY");
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return std::format_to(ctx.out(), "VK_ERROR_INVALID_EXTERNAL_HANDLE");
        case VK_ERROR_FRAGMENTATION:
            return std::format_to(ctx.out(), "VK_ERROR_FRAGMENTATION");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return std::format_to(ctx.out(), "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS");
        case VK_ERROR_SURFACE_LOST_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_SURFACE_LOST_KHR");
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
        case VK_SUBOPTIMAL_KHR:
            return std::format_to(ctx.out(), "VK_SUBOPTIMAL_KHR");
        case VK_ERROR_OUT_OF_DATE_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_OUT_OF_DATE_KHR");
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR");
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return std::format_to(ctx.out(), "VK_ERROR_VALIDATION_FAILED_EXT");
        case VK_ERROR_INVALID_SHADER_NV:
            return std::format_to(ctx.out(), "VK_ERROR_INVALID_SHADER_NV");
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return std::format_to(ctx.out(), "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT");
        case VK_ERROR_NOT_PERMITTED_EXT:
            return std::format_to(ctx.out(), "VK_ERROR_NOT_PERMITTED_EXT");
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return std::format_to(ctx.out(), "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT");
        case VK_PIPELINE_COMPILE_REQUIRED:
            return std::format_to(ctx.out(), "VK_PIPELINE_COMPILE_REQUIRED");
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR");
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR");
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR");
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR");
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR");
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR");
#ifdef VK_ENABLE_BETA_EXTENSIONS
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            return std::format_to(ctx.out(), "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR");
#endif
        case VK_THREAD_IDLE_KHR:
            return std::format_to(ctx.out(), "VK_THREAD_IDLE_KHR");
        case VK_THREAD_DONE_KHR:
            return std::format_to(ctx.out(), "VK_THREAD_DONE_KHR");
        case VK_OPERATION_DEFERRED_KHR:
            return std::format_to(ctx.out(), "VK_OPERATION_DEFERRED_KHR");
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return std::format_to(ctx.out(), "VK_OPERATION_NOT_DEFERRED_KHR");
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            return std::format_to(ctx.out(), "VK_ERROR_COMPRESSION_EXHAUSTED_EXT");
        case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
            return std::format_to(ctx.out(), "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT");
        case VK_RESULT_MAX_ENUM:
            break;
        }

        return std::format_to(ctx.out(), "Unknown VkResult: {}", static_cast<int>(result));
    }
};

SPARK_WARNING_POP
