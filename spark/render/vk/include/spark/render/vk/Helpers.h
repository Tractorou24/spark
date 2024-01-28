#pragma once

#include "spark/render/Format.h"
#include "spark/render/vk/Export.h"

#include "spark/base/Exception.h"

#include <algorithm>
#include <vector>

/**
 * \brief Helper macro to forward declare a vulkan handle. Avoids the need to kink vulkan publicly in the library.
 * \param handle The name of the handle to forward declare.
 */
#define SPARK_FWD_DECLARE_VK_HANDLE(handle) using handle = struct handle##_T*;

namespace spark::render::vk::helpers
{
    /**
     * \brief Checks if a \ref Format has a depth channel.
     * \param format The \ref Format to check.
     * \return `true` if the \ref Format has a depth channel, `false` otherwise.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr bool has_depth(const Format format)
    {
        const std::vector depth_formats = {
            Format::D16_UNORM,
            Format::D32_SFLOAT,
            Format::X8_D24_UNORM,
            Format::D16_UNORM_S8_UINT,
            Format::D24_UNORM_S8_UINT,
            Format::D32_SFLOAT_S8_UINT
        };

        return std::ranges::any_of(depth_formats, [&](const Format f) { return f == format; });
    }

    /**
     * \brief Checks if a \ref Format has a stencil channel.
     * \param format The \ref Format to check.
     * \return `true` if the \ref Format has a stencil channel, `false` otherwise.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr bool has_stencil(const Format format)
    {
        const std::vector stencil_formats = {
            Format::D16_UNORM_S8_UINT,
            Format::D24_UNORM_S8_UINT,
            Format::D32_SFLOAT_S8_UINT,
            Format::S8_UINT
        };

        return std::ranges::any_of(stencil_formats, [&](const Format f) { return f == format; });
    }

    /**
     * \brief Gets the size of a \ref Format in bytes.
     * \param format The \ref Format to get the size of.
     * \return The size of the \ref Format in bytes.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr std::size_t format_size(const Format format)
    {
        switch (format)
        {
        case Format::None:
            return 0;
        case Format::R4G4_UNORM:
        case Format::R8_UNORM:
        case Format::R8_SNORM:
        case Format::R8_USCALED:
        case Format::R8_SSCALED:
        case Format::R8_UINT:
        case Format::R8_SINT:
        case Format::R8_SRGB:
        case Format::S8_UINT:
            return 1;
        case Format::R4G4B4A4_UNORM:
        case Format::B4G4R4A4_UNORM:
        case Format::R5G6B5_UNORM:
        case Format::B5G6R5_UNORM:
        case Format::R5G5B5A1_UNORM:
        case Format::B5G5R5A1_UNORM:
        case Format::A1R5G5B5_UNORM:
        case Format::R8G8_UNORM:
        case Format::R8G8_SNORM:
        case Format::R8G8_USCALED:
        case Format::R8G8_SSCALED:
        case Format::R8G8_UINT:
        case Format::R8G8_SINT:
        case Format::R8G8_SRGB:
        case Format::R16_UNORM:
        case Format::R16_SNORM:
        case Format::R16_USCALED:
        case Format::R16_SSCALED:
        case Format::R16_UINT:
        case Format::R16_SINT:
        case Format::R16_SFLOAT:
        case Format::D16_UNORM:
            return 2;
        case Format::R8G8B8_UNORM:
        case Format::R8G8B8_SNORM:
        case Format::R8G8B8_USCALED:
        case Format::R8G8B8_SSCALED:
        case Format::R8G8B8_UINT:
        case Format::R8G8B8_SINT:
        case Format::R8G8B8_SRGB:
        case Format::B8G8R8_UNORM:
        case Format::B8G8R8_SNORM:
        case Format::B8G8R8_USCALED:
        case Format::B8G8R8_SSCALED:
        case Format::B8G8R8_UINT:
        case Format::B8G8R8_SINT:
        case Format::B8G8R8_SRGB:
        case Format::D16_UNORM_S8_UINT:
            return 3;
        case Format::R8G8B8A8_UNORM:
        case Format::R8G8B8A8_SNORM:
        case Format::R8G8B8A8_USCALED:
        case Format::R8G8B8A8_SSCALED:
        case Format::R8G8B8A8_UINT:
        case Format::R8G8B8A8_SINT:
        case Format::R8G8B8A8_SRGB:
        case Format::B8G8R8A8_UNORM:
        case Format::B8G8R8A8_SNORM:
        case Format::B8G8R8A8_USCALED:
        case Format::B8G8R8A8_SSCALED:
        case Format::B8G8R8A8_UINT:
        case Format::B8G8R8A8_SINT:
        case Format::B8G8R8A8_SRGB:
        case Format::A8B8G8R8_UNORM:
        case Format::A8B8G8R8_SNORM:
        case Format::A8B8G8R8_USCALED:
        case Format::A8B8G8R8_SSCALED:
        case Format::A8B8G8R8_UINT:
        case Format::A8B8G8R8_SINT:
        case Format::A8B8G8R8_SRGB:
        case Format::A2R10G10B10_UNORM:
        case Format::A2R10G10B10_SNORM:
        case Format::A2R10G10B10_USCALED:
        case Format::A2R10G10B10_SSCALED:
        case Format::A2R10G10B10_UINT:
        case Format::A2R10G10B10_SINT:
        case Format::A2B10G10R10_UNORM:
        case Format::A2B10G10R10_SNORM:
        case Format::A2B10G10R10_USCALED:
        case Format::A2B10G10R10_SSCALED:
        case Format::A2B10G10R10_UINT:
        case Format::A2B10G10R10_SINT:
        case Format::R16G16_UNORM:
        case Format::R16G16_SNORM:
        case Format::R16G16_USCALED:
        case Format::R16G16_SSCALED:
        case Format::R16G16_UINT:
        case Format::R16G16_SINT:
        case Format::R16G16_SFLOAT:
        case Format::R32_UINT:
        case Format::R32_SINT:
        case Format::R32_SFLOAT:
        case Format::B10G11R11_UFLOAT:
        case Format::E5B9G9R9_UFLOAT:
        case Format::X8_D24_UNORM:
        case Format::D32_SFLOAT:
        case Format::D24_UNORM_S8_UINT:
            return 4;
        case Format::R16G16B16_UNORM:
        case Format::R16G16B16_SNORM:
        case Format::R16G16B16_USCALED:
        case Format::R16G16B16_SSCALED:
        case Format::R16G16B16_UINT:
        case Format::R16G16B16_SINT:
        case Format::R16G16B16_SFLOAT:
            return 6;
        case Format::R16G16B16A16_UNORM:
        case Format::R16G16B16A16_SNORM:
        case Format::R16G16B16A16_USCALED:
        case Format::R16G16B16A16_SSCALED:
        case Format::R16G16B16A16_UINT:
        case Format::R16G16B16A16_SINT:
        case Format::R16G16B16A16_SFLOAT:
        case Format::R32G32_UINT:
        case Format::R32G32_SINT:
        case Format::R32G32_SFLOAT:
        case Format::R64_UINT:
        case Format::R64_SINT:
        case Format::R64_SFLOAT:
        case Format::D32_SFLOAT_S8_UINT:
        case Format::BC1_RGB_UNORM:
        case Format::BC1_RGB_SRGB:
        case Format::BC1_RGBA_UNORM:
        case Format::BC1_RGBA_SRGB:
        case Format::BC4_UNORM:
        case Format::BC4_SNORM:
            return 8;
        case Format::R32G32B32_UINT:
        case Format::R32G32B32_SINT:
        case Format::R32G32B32_SFLOAT:
            return 12;
        case Format::R32G32B32A32_UINT:
        case Format::R32G32B32A32_SINT:
        case Format::R32G32B32A32_SFLOAT:
        case Format::R64G64_UINT:
        case Format::R64G64_SINT:
        case Format::R64G64_SFLOAT:
        case Format::BC2_UNORM:
        case Format::BC2_SRGB:
        case Format::BC3_UNORM:
        case Format::BC3_SRGB:
        case Format::BC5_UNORM:
        case Format::BC5_SNORM:
        case Format::BC6H_UFLOAT:
        case Format::BC6H_SFLOAT:
        case Format::BC7_UNORM:
        case Format::BC7_SRGB:
            return 16;
        case Format::R64G64B64_UINT:
        case Format::R64G64B64_SINT:
        case Format::R64G64B64_SFLOAT:
            return 24;
        case Format::R64G64B64A64_UINT:
        case Format::R64G64B64A64_SINT:
        case Format::R64G64B64A64_SFLOAT:
            return 32;
        default:
            break;
        }
        throw base::BadArgumentException("Unsupported texel format.");
    }
}
