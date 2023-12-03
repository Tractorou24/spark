#pragma once

#include <format>

// ReSharper disable StringLiteralTypo
// ReSharper disable CppInconsistentNaming
// ReSharper disable IdentifierTypo

namespace spark::render
{
    /**
     * \brief Describes the format of a single texel.
     */
    enum class Format
    {
        None = 0x00000000,
        R4G4_UNORM,
        R4G4B4A4_UNORM,
        B4G4R4A4_UNORM,
        R5G6B5_UNORM,
        B5G6R5_UNORM,
        R5G5B5A1_UNORM,
        B5G5R5A1_UNORM,
        A1R5G5B5_UNORM,
        R8_UNORM,
        R8_SNORM,
        R8_USCALED,
        R8_SSCALED,
        R8_UINT,
        R8_SINT,
        R8_SRGB,
        R8G8_UNORM,
        R8G8_SNORM,
        R8G8_USCALED,
        R8G8_SSCALED,
        R8G8_UINT,
        R8G8_SINT,
        R8G8_SRGB,
        R8G8B8_UNORM,
        R8G8B8_SNORM,
        R8G8B8_USCALED,
        R8G8B8_SSCALED,
        R8G8B8_UINT,
        R8G8B8_SINT,
        R8G8B8_SRGB,
        B8G8R8_UNORM,
        B8G8R8_SNORM,
        B8G8R8_USCALED,
        B8G8R8_SSCALED,
        B8G8R8_UINT,
        B8G8R8_SINT,
        B8G8R8_SRGB,
        R8G8B8A8_UNORM,
        R8G8B8A8_SNORM,
        R8G8B8A8_USCALED,
        R8G8B8A8_SSCALED,
        R8G8B8A8_UINT,
        R8G8B8A8_SINT,
        R8G8B8A8_SRGB,
        B8G8R8A8_UNORM,
        B8G8R8A8_SNORM,
        B8G8R8A8_USCALED,
        B8G8R8A8_SSCALED,
        B8G8R8A8_UINT,
        B8G8R8A8_SINT,
        B8G8R8A8_SRGB,
        A8B8G8R8_UNORM,
        A8B8G8R8_SNORM,
        A8B8G8R8_USCALED,
        A8B8G8R8_SSCALED,
        A8B8G8R8_UINT,
        A8B8G8R8_SINT,
        A8B8G8R8_SRGB,
        A2R10G10B10_UNORM,
        A2R10G10B10_SNORM,
        A2R10G10B10_USCALED,
        A2R10G10B10_SSCALED,
        A2R10G10B10_UINT,
        A2R10G10B10_SINT,
        A2B10G10R10_UNORM,
        A2B10G10R10_SNORM,
        A2B10G10R10_USCALED,
        A2B10G10R10_SSCALED,
        A2B10G10R10_UINT,
        A2B10G10R10_SINT,
        R16_UNORM,
        R16_SNORM,
        R16_USCALED,
        R16_SSCALED,
        R16_UINT,
        R16_SINT,
        R16_SFLOAT,
        R16G16_UNORM,
        R16G16_SNORM,
        R16G16_USCALED,
        R16G16_SSCALED,
        R16G16_UINT,
        R16G16_SINT,
        R16G16_SFLOAT,
        R16G16B16_UNORM,
        R16G16B16_SNORM,
        R16G16B16_USCALED,
        R16G16B16_SSCALED,
        R16G16B16_UINT,
        R16G16B16_SINT,
        R16G16B16_SFLOAT,
        R16G16B16A16_UNORM,
        R16G16B16A16_SNORM,
        R16G16B16A16_USCALED,
        R16G16B16A16_SSCALED,
        R16G16B16A16_UINT,
        R16G16B16A16_SINT,
        R16G16B16A16_SFLOAT,
        R32_UINT,
        R32_SINT,
        R32_SFLOAT,
        R32G32_UINT,
        R32G32_SINT,
        R32G32_SFLOAT,
        R32G32B32_UINT,
        R32G32B32_SINT,
        R32G32B32_SFLOAT,
        R32G32B32A32_UINT,
        R32G32B32A32_SINT,
        R32G32B32A32_SFLOAT,
        R64_UINT,
        R64_SINT,
        R64_SFLOAT,
        R64G64_UINT,
        R64G64_SINT,
        R64G64_SFLOAT,
        R64G64B64_UINT,
        R64G64B64_SINT,
        R64G64B64_SFLOAT,
        R64G64B64A64_UINT,
        R64G64B64A64_SINT,
        R64G64B64A64_SFLOAT,
        B10G11R11_UFLOAT,
        E5B9G9R9_UFLOAT,
        D16_UNORM,
        X8_D24_UNORM,
        D32_SFLOAT,
        S8_UINT,
        D16_UNORM_S8_UINT,
        D24_UNORM_S8_UINT,
        D32_SFLOAT_S8_UINT,
        BC1_RGB_UNORM,
        BC1_RGB_SRGB,
        BC1_RGBA_UNORM,
        BC1_RGBA_SRGB,
        BC2_UNORM,
        BC2_SRGB,
        BC3_UNORM,
        BC3_SRGB,
        BC4_UNORM,
        BC4_SNORM,
        BC5_UNORM,
        BC5_SNORM,
        BC6H_UFLOAT,
        BC6H_SFLOAT,
        BC7_UNORM,
        BC7_SRGB,
        Other = 0x7FFFFFFF
        // ReSharper restore CppInconsistentNaming
    };
}

template <>
struct std::formatter<spark::render::Format> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::Format format, format_context& ctx)
    {
        switch (format)
        {
        case spark::render::Format::R4G4_UNORM:
            return std::format_to(ctx.out(), "R4G4_UNORM");
        case spark::render::Format::R4G4B4A4_UNORM:
            return std::format_to(ctx.out(), "R4G4B4A4_UNORM");
        case spark::render::Format::B4G4R4A4_UNORM:
            return std::format_to(ctx.out(), "B4G4R4A4_UNORM");
        case spark::render::Format::R5G6B5_UNORM:
            return std::format_to(ctx.out(), "R5G6B5_UNORM");
        case spark::render::Format::B5G6R5_UNORM:
            return std::format_to(ctx.out(), "B5G6R5_UNORM");
        case spark::render::Format::R5G5B5A1_UNORM:
            return std::format_to(ctx.out(), "R5G5B5A1_UNORM");
        case spark::render::Format::B5G5R5A1_UNORM:
            return std::format_to(ctx.out(), "B5G5R5A1_UNORM");
        case spark::render::Format::A1R5G5B5_UNORM:
            return std::format_to(ctx.out(), "A1R5G5B5_UNORM");
        case spark::render::Format::R8_UNORM:
            return std::format_to(ctx.out(), "R8_UNORM");
        case spark::render::Format::R8_SNORM:
            return std::format_to(ctx.out(), "R8_SNORM");
        case spark::render::Format::R8_USCALED:
            return std::format_to(ctx.out(), "R8_USCALED");
        case spark::render::Format::R8_SSCALED:
            return std::format_to(ctx.out(), "R8_SSCALED");
        case spark::render::Format::R8_UINT:
            return std::format_to(ctx.out(), "R8_UINT");
        case spark::render::Format::R8_SINT:
            return std::format_to(ctx.out(), "R8_SINT");
        case spark::render::Format::R8_SRGB:
            return std::format_to(ctx.out(), "R8_SRGB");
        case spark::render::Format::R8G8_UNORM:
            return std::format_to(ctx.out(), "R8G8_UNORM");
        case spark::render::Format::R8G8_SNORM:
            return std::format_to(ctx.out(), "R8G8_SNORM");
        case spark::render::Format::R8G8_USCALED:
            return std::format_to(ctx.out(), "R8G8_USCALED");
        case spark::render::Format::R8G8_SSCALED:
            return std::format_to(ctx.out(), "R8G8_SSCALED");
        case spark::render::Format::R8G8_UINT:
            return std::format_to(ctx.out(), "R8G8_UINT");
        case spark::render::Format::R8G8_SINT:
            return std::format_to(ctx.out(), "R8G8_SINT");
        case spark::render::Format::R8G8_SRGB:
            return std::format_to(ctx.out(), "R8G8_SRGB");
        case spark::render::Format::R8G8B8_UNORM:
            return std::format_to(ctx.out(), "R8G8B8_UNORM");
        case spark::render::Format::R8G8B8_SNORM:
            return std::format_to(ctx.out(), "R8G8B8_SNORM");
        case spark::render::Format::R8G8B8_USCALED:
            return std::format_to(ctx.out(), "R8G8B8_USCALED");
        case spark::render::Format::R8G8B8_SSCALED:
            return std::format_to(ctx.out(), "R8G8B8_SSCALED");
        case spark::render::Format::R8G8B8_UINT:
            return std::format_to(ctx.out(), "R8G8B8_UINT");
        case spark::render::Format::R8G8B8_SINT:
            return std::format_to(ctx.out(), "R8G8B8_SINT");
        case spark::render::Format::R8G8B8_SRGB:
            return std::format_to(ctx.out(), "R8G8B8_SRGB");
        case spark::render::Format::B8G8R8_UNORM:
            return std::format_to(ctx.out(), "B8G8R8_UNORM");
        case spark::render::Format::B8G8R8_SNORM:
            return std::format_to(ctx.out(), "B8G8R8_SNORM");
        case spark::render::Format::B8G8R8_USCALED:
            return std::format_to(ctx.out(), "B8G8R8_USCALED");
        case spark::render::Format::B8G8R8_SSCALED:
            return std::format_to(ctx.out(), "B8G8R8_SSCALED");
        case spark::render::Format::B8G8R8_UINT:
            return std::format_to(ctx.out(), "B8G8R8_UINT");
        case spark::render::Format::B8G8R8_SINT:
            return std::format_to(ctx.out(), "B8G8R8_SINT");
        case spark::render::Format::B8G8R8_SRGB:
            return std::format_to(ctx.out(), "B8G8R8_SRGB");
        case spark::render::Format::R8G8B8A8_UNORM:
            return std::format_to(ctx.out(), "R8G8B8A8_UNORM");
        case spark::render::Format::R8G8B8A8_SNORM:
            return std::format_to(ctx.out(), "R8G8B8A8_SNORM");
        case spark::render::Format::R8G8B8A8_USCALED:
            return std::format_to(ctx.out(), "R8G8B8A8_USCALED");
        case spark::render::Format::R8G8B8A8_SSCALED:
            return std::format_to(ctx.out(), "R8G8B8A8_SSCALED");
        case spark::render::Format::R8G8B8A8_UINT:
            return std::format_to(ctx.out(), "R8G8B8A8_UINT");
        case spark::render::Format::R8G8B8A8_SINT:
            return std::format_to(ctx.out(), "R8G8B8A8_SINT");
        case spark::render::Format::R8G8B8A8_SRGB:
            return std::format_to(ctx.out(), "R8G8B8A8_SRGB");
        case spark::render::Format::B8G8R8A8_UNORM:
            return std::format_to(ctx.out(), "B8G8R8A8_UNORM");
        case spark::render::Format::B8G8R8A8_SNORM:
            return std::format_to(ctx.out(), "B8G8R8A8_SNORM");
        case spark::render::Format::B8G8R8A8_USCALED:
            return std::format_to(ctx.out(), "B8G8R8A8_USCALED");
        case spark::render::Format::B8G8R8A8_SSCALED:
            return std::format_to(ctx.out(), "B8G8R8A8_SSCALED");
        case spark::render::Format::B8G8R8A8_UINT:
            return std::format_to(ctx.out(), "B8G8R8A8_UINT");
        case spark::render::Format::B8G8R8A8_SINT:
            return std::format_to(ctx.out(), "B8G8R8A8_SINT");
        case spark::render::Format::B8G8R8A8_SRGB:
            return std::format_to(ctx.out(), "B8G8R8A8_SRGB");
        case spark::render::Format::A8B8G8R8_UNORM:
            return std::format_to(ctx.out(), "A8B8G8R8_UNORM");
        case spark::render::Format::A8B8G8R8_SNORM:
            return std::format_to(ctx.out(), "A8B8G8R8_SNORM");
        case spark::render::Format::A8B8G8R8_USCALED:
            return std::format_to(ctx.out(), "A8B8G8R8_USCALED");
        case spark::render::Format::A8B8G8R8_SSCALED:
            return std::format_to(ctx.out(), "A8B8G8R8_SSCALED");
        case spark::render::Format::A8B8G8R8_UINT:
            return std::format_to(ctx.out(), "A8B8G8R8_UINT");
        case spark::render::Format::A8B8G8R8_SINT:
            return std::format_to(ctx.out(), "A8B8G8R8_SINT");
        case spark::render::Format::A8B8G8R8_SRGB:
            return std::format_to(ctx.out(), "A8B8G8R8_SRGB");
        case spark::render::Format::A2R10G10B10_UNORM:
            return std::format_to(ctx.out(), "A2R10G10B10_UNORM");
        case spark::render::Format::A2R10G10B10_SNORM:
            return std::format_to(ctx.out(), "A2R10G10B10_SNORM");
        case spark::render::Format::A2R10G10B10_USCALED:
            return std::format_to(ctx.out(), "A2R10G10B10_USCALED");
        case spark::render::Format::A2R10G10B10_SSCALED:
            return std::format_to(ctx.out(), "A2R10G10B10_SSCALED");
        case spark::render::Format::A2R10G10B10_UINT:
            return std::format_to(ctx.out(), "A2R10G10B10_UINT");
        case spark::render::Format::A2R10G10B10_SINT:
            return std::format_to(ctx.out(), "A2R10G10B10_SINT");
        case spark::render::Format::A2B10G10R10_UNORM:
            return std::format_to(ctx.out(), "A2B10G10R10_UNORM");
        case spark::render::Format::A2B10G10R10_SNORM:
            return std::format_to(ctx.out(), "A2B10G10R10_SNORM");
        case spark::render::Format::A2B10G10R10_USCALED:
            return std::format_to(ctx.out(), "A2B10G10R10_USCALED");
        case spark::render::Format::A2B10G10R10_SSCALED:
            return std::format_to(ctx.out(), "A2B10G10R10_SSCALED");
        case spark::render::Format::A2B10G10R10_UINT:
            return std::format_to(ctx.out(), "A2B10G10R10_UINT");
        case spark::render::Format::A2B10G10R10_SINT:
            return std::format_to(ctx.out(), "A2B10G10R10_SINT");
        case spark::render::Format::R16_UNORM:
            return std::format_to(ctx.out(), "R16_UNORM");
        case spark::render::Format::R16_SNORM:
            return std::format_to(ctx.out(), "R16_SNORM");
        case spark::render::Format::R16_USCALED:
            return std::format_to(ctx.out(), "R16_USCALED");
        case spark::render::Format::R16_SSCALED:
            return std::format_to(ctx.out(), "R16_SSCALED");
        case spark::render::Format::R16_UINT:
            return std::format_to(ctx.out(), "R16_UINT");
        case spark::render::Format::R16_SINT:
            return std::format_to(ctx.out(), "R16_SINT");
        case spark::render::Format::R16_SFLOAT:
            return std::format_to(ctx.out(), "R16_SFLOAT");
        case spark::render::Format::R16G16_UNORM:
            return std::format_to(ctx.out(), "R16G16_UNORM");
        case spark::render::Format::R16G16_SNORM:
            return std::format_to(ctx.out(), "R16G16_SNORM");
        case spark::render::Format::R16G16_USCALED:
            return std::format_to(ctx.out(), "R16G16_USCALED");
        case spark::render::Format::R16G16_SSCALED:
            return std::format_to(ctx.out(), "R16G16_SSCALED");
        case spark::render::Format::R16G16_UINT:
            return std::format_to(ctx.out(), "R16G16_UINT");
        case spark::render::Format::R16G16_SINT:
            return std::format_to(ctx.out(), "R16G16_SINT");
        case spark::render::Format::R16G16_SFLOAT:
            return std::format_to(ctx.out(), "R16G16_SFLOAT");
        case spark::render::Format::R16G16B16_UNORM:
            return std::format_to(ctx.out(), "R16G16B16_UNORM");
        case spark::render::Format::R16G16B16_SNORM:
            return std::format_to(ctx.out(), "R16G16B16_SNORM");
        case spark::render::Format::R16G16B16_USCALED:
            return std::format_to(ctx.out(), "R16G16B16_USCALED");
        case spark::render::Format::R16G16B16_SSCALED:
            return std::format_to(ctx.out(), "R16G16B16_SSCALED");
        case spark::render::Format::R16G16B16_UINT:
            return std::format_to(ctx.out(), "R16G16B16_UINT");
        case spark::render::Format::R16G16B16_SINT:
            return std::format_to(ctx.out(), "R16G16B16_SINT");
        case spark::render::Format::R16G16B16_SFLOAT:
            return std::format_to(ctx.out(), "R16G16B16_SFLOAT");
        case spark::render::Format::R16G16B16A16_UNORM:
            return std::format_to(ctx.out(), "R16G16B16A16_UNORM");
        case spark::render::Format::R16G16B16A16_SNORM:
            return std::format_to(ctx.out(), "R16G16B16A16_SNORM");
        case spark::render::Format::R16G16B16A16_USCALED:
            return std::format_to(ctx.out(), "R16G16B16A16_USCALED");
        case spark::render::Format::R16G16B16A16_SSCALED:
            return std::format_to(ctx.out(), "R16G16B16A16_SSCALED");
        case spark::render::Format::R16G16B16A16_UINT:
            return std::format_to(ctx.out(), "R16G16B16A16_UINT");
        case spark::render::Format::R16G16B16A16_SINT:
            return std::format_to(ctx.out(), "R16G16B16A16_SINT");
        case spark::render::Format::R16G16B16A16_SFLOAT:
            return std::format_to(ctx.out(), "R16G16B16A16_SFLOAT");
        case spark::render::Format::R32_UINT:
            return std::format_to(ctx.out(), "R32_UINT");
        case spark::render::Format::R32_SINT:
            return std::format_to(ctx.out(), "R32_SINT");
        case spark::render::Format::R32_SFLOAT:
            return std::format_to(ctx.out(), "R32_SFLOAT");
        case spark::render::Format::R32G32_UINT:
            return std::format_to(ctx.out(), "R32G32_UINT");
        case spark::render::Format::R32G32_SINT:
            return std::format_to(ctx.out(), "R32G32_SINT");
        case spark::render::Format::R32G32_SFLOAT:
            return std::format_to(ctx.out(), "R32G32_SFLOAT");
        case spark::render::Format::R32G32B32_UINT:
            return std::format_to(ctx.out(), "R32G32B32_UINT");
        case spark::render::Format::R32G32B32_SINT:
            return std::format_to(ctx.out(), "R32G32B32_SINT");
        case spark::render::Format::R32G32B32_SFLOAT:
            return std::format_to(ctx.out(), "R32G32B32_SFLOAT");
        case spark::render::Format::R32G32B32A32_UINT:
            return std::format_to(ctx.out(), "R32G32B32A32_UINT");
        case spark::render::Format::R32G32B32A32_SINT:
            return std::format_to(ctx.out(), "R32G32B32A32_SINT");
        case spark::render::Format::R32G32B32A32_SFLOAT:
            return std::format_to(ctx.out(), "R32G32B32A32_SFLOAT");
        case spark::render::Format::R64_UINT:
            return std::format_to(ctx.out(), "R64_UINT");
        case spark::render::Format::R64_SINT:
            return std::format_to(ctx.out(), "R64_SINT");
        case spark::render::Format::R64_SFLOAT:
            return std::format_to(ctx.out(), "R64_SFLOAT");
        case spark::render::Format::R64G64_UINT:
            return std::format_to(ctx.out(), "R64G64_UINT");
        case spark::render::Format::R64G64_SINT:
            return std::format_to(ctx.out(), "R64G64_SINT");
        case spark::render::Format::R64G64_SFLOAT:
            return std::format_to(ctx.out(), "R64G64_SFLOAT");
        case spark::render::Format::R64G64B64_UINT:
            return std::format_to(ctx.out(), "R64G64B64_UINT");
        case spark::render::Format::R64G64B64_SINT:
            return std::format_to(ctx.out(), "R64G64B64_SINT");
        case spark::render::Format::R64G64B64_SFLOAT:
            return std::format_to(ctx.out(), "R64G64B64_SFLOAT");
        case spark::render::Format::R64G64B64A64_UINT:
            return std::format_to(ctx.out(), "R64G64B64A64_UINT");
        case spark::render::Format::R64G64B64A64_SINT:
            return std::format_to(ctx.out(), "R64G64B64A64_SINT");
        case spark::render::Format::R64G64B64A64_SFLOAT:
            return std::format_to(ctx.out(), "R64G64B64A64_SFLOAT");
        case spark::render::Format::B10G11R11_UFLOAT:
            return std::format_to(ctx.out(), "B10G11R11_UFLOAT");
        case spark::render::Format::E5B9G9R9_UFLOAT:
            return std::format_to(ctx.out(), "E5B9G9R9_UFLOAT");
        case spark::render::Format::D16_UNORM:
            return std::format_to(ctx.out(), "D16_UNORM");
        case spark::render::Format::X8_D24_UNORM:
            return std::format_to(ctx.out(), "X8_D24_UNORM");
        case spark::render::Format::D32_SFLOAT:
            return std::format_to(ctx.out(), "D32_SFLOAT");
        case spark::render::Format::S8_UINT:
            return std::format_to(ctx.out(), "S8_UINT");
        case spark::render::Format::D16_UNORM_S8_UINT:
            return std::format_to(ctx.out(), "D16_UNORM_S8_UINT");
        case spark::render::Format::D24_UNORM_S8_UINT:
            return std::format_to(ctx.out(), "D24_UNORM_S8_UINT");
        case spark::render::Format::D32_SFLOAT_S8_UINT:
            return std::format_to(ctx.out(), "D32_SFLOAT_S8_UINT");
        case spark::render::Format::BC1_RGB_UNORM:
            return std::format_to(ctx.out(), "BC1_RGB_UNORM");
        case spark::render::Format::BC1_RGB_SRGB:
            return std::format_to(ctx.out(), "BC1_RGB_SRGB");
        case spark::render::Format::BC1_RGBA_UNORM:
            return std::format_to(ctx.out(), "BC1_RGBA_UNORM");
        case spark::render::Format::BC1_RGBA_SRGB:
            return std::format_to(ctx.out(), "BC1_RGBA_SRGB");
        case spark::render::Format::BC2_UNORM:
            return std::format_to(ctx.out(), "BC2_UNORM");
        case spark::render::Format::BC2_SRGB:
            return std::format_to(ctx.out(), "BC2_SRGB");
        case spark::render::Format::BC3_UNORM:
            return std::format_to(ctx.out(), "BC3_UNORM");
        case spark::render::Format::BC3_SRGB:
            return std::format_to(ctx.out(), "BC3_SRGB");
        case spark::render::Format::BC4_UNORM:
            return std::format_to(ctx.out(), "BC4_UNORM");
        case spark::render::Format::BC4_SNORM:
            return std::format_to(ctx.out(), "BC4_SNORM");
        case spark::render::Format::BC5_UNORM:
            return std::format_to(ctx.out(), "BC5_UNORM");
        case spark::render::Format::BC5_SNORM:
            return std::format_to(ctx.out(), "BC5_SNORM");
        case spark::render::Format::BC6H_UFLOAT:
            return std::format_to(ctx.out(), "BC6H_UFLOAT");
        case spark::render::Format::BC6H_SFLOAT:
            return std::format_to(ctx.out(), "BC6H_SFLOAT");
        case spark::render::Format::BC7_UNORM:
            return std::format_to(ctx.out(), "BC7_UNORM");
        case spark::render::Format::BC7_SRGB:
            return std::format_to(ctx.out(), "BC7_SRGB");
        case spark::render::Format::None:
            return std::format_to(ctx.out(), "None");
        case spark::render::Format::Other:
            break;
        }
        return std::format_to(ctx.out(), "Other");
    }
};
