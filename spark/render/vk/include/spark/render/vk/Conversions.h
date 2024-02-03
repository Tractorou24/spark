#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/Image.h"
#include "spark/render/InputAssembler.h"
#include "spark/render/Rasterizer.h"
#include "spark/render/RenderTarget.h"
#include "spark/render/ShaderStages.h"
#include "spark/render/vk/Export.h"

#include "spark/base/Exception.h"

#include "vulkan/vulkan.h"

namespace spark::render::vk::conversions
{
    /**
     * \brief Converts a \ref ImageDimensions to a \ref VkImageViewType.
     * \param dimension The \ref ImageDimensions to convert.
     * \param layers The number of layers in the image.
     * \return A \ref VkImageViewType value representing the \ref ImageDimensions.
     *
     * \throws base::BadArgumentException If the \ref ImageDimensions is not supported.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkImageViewType to_image_view(const ImageDimensions dimension, const unsigned layers)
    {
        switch (dimension)
        {
        case ImageDimensions::DIM_1:
            return layers == 1 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_1D : VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        case ImageDimensions::DIM_2:
            return layers == 1 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_2D : VkImageViewType::VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case ImageDimensions::DIM_3:
            return VkImageViewType::VK_IMAGE_VIEW_TYPE_3D;
        case ImageDimensions::CUBE:
            return VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE;
        }
        throw base::BadArgumentException("Unsupported image dimension.");
    }

    /**
     * \brief Converts a \ref Format to a \ref VkFormat.
     * \param format The \ref Format to convert.
     * \return A \ref VkFormat value representing the \ref Format.
     *
     * \throws base::BadArgumentException If the \ref Format is not supported.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkFormat to_format(const Format format)
    {
        switch (format)
        {
        case Format::R4G4_UNORM:
            return VK_FORMAT_R4G4_UNORM_PACK8;
        case Format::R4G4B4A4_UNORM:
            return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        case Format::B4G4R4A4_UNORM:
            return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
        case Format::R5G6B5_UNORM:
            return VK_FORMAT_R5G6B5_UNORM_PACK16;
        case Format::B5G6R5_UNORM:
            return VK_FORMAT_B5G6R5_UNORM_PACK16;
        case Format::R5G5B5A1_UNORM:
            return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        case Format::B5G5R5A1_UNORM:
            return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        case Format::A1R5G5B5_UNORM:
            return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
        case Format::R8_UNORM:
            return VK_FORMAT_R8_UNORM;
        case Format::R8_SNORM:
            return VK_FORMAT_R8_SNORM;
        case Format::R8_USCALED:
            return VK_FORMAT_R8_USCALED;
        case Format::R8_SSCALED:
            return VK_FORMAT_R8_SSCALED;
        case Format::R8_UINT:
            return VK_FORMAT_R8_UINT;
        case Format::R8_SINT:
            return VK_FORMAT_R8_SINT;
        case Format::R8_SRGB:
            return VK_FORMAT_R8_SRGB;
        case Format::R8G8_UNORM:
            return VK_FORMAT_R8G8_UNORM;
        case Format::R8G8_SNORM:
            return VK_FORMAT_R8G8_SNORM;
        case Format::R8G8_USCALED:
            return VK_FORMAT_R8G8_USCALED;
        case Format::R8G8_SSCALED:
            return VK_FORMAT_R8G8_SSCALED;
        case Format::R8G8_UINT:
            return VK_FORMAT_R8G8_UINT;
        case Format::R8G8_SINT:
            return VK_FORMAT_R8G8_SINT;
        case Format::R8G8_SRGB:
            return VK_FORMAT_R8G8_SRGB;
        case Format::R8G8B8_UNORM:
            return VK_FORMAT_R8G8B8_UNORM;
        case Format::R8G8B8_SNORM:
            return VK_FORMAT_R8G8B8_SNORM;
        case Format::R8G8B8_USCALED:
            return VK_FORMAT_R8G8B8_USCALED;
        case Format::R8G8B8_SSCALED:
            return VK_FORMAT_R8G8B8_SSCALED;
        case Format::R8G8B8_UINT:
            return VK_FORMAT_R8G8B8_UINT;
        case Format::R8G8B8_SINT:
            return VK_FORMAT_R8G8B8_SINT;
        case Format::R8G8B8_SRGB:
            return VK_FORMAT_R8G8B8_SRGB;
        case Format::B8G8R8_UNORM:
            return VK_FORMAT_B8G8R8_UNORM;
        case Format::B8G8R8_SNORM:
            return VK_FORMAT_B8G8R8_SNORM;
        case Format::B8G8R8_USCALED:
            return VK_FORMAT_B8G8R8_USCALED;
        case Format::B8G8R8_SSCALED:
            return VK_FORMAT_B8G8R8_SSCALED;
        case Format::B8G8R8_UINT:
            return VK_FORMAT_B8G8R8_UINT;
        case Format::B8G8R8_SINT:
            return VK_FORMAT_B8G8R8_SINT;
        case Format::B8G8R8_SRGB:
            return VK_FORMAT_B8G8R8_SRGB;
        case Format::R8G8B8A8_UNORM:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case Format::R8G8B8A8_SNORM:
            return VK_FORMAT_R8G8B8A8_SNORM;
        case Format::R8G8B8A8_USCALED:
            return VK_FORMAT_R8G8B8A8_USCALED;
        case Format::R8G8B8A8_SSCALED:
            return VK_FORMAT_R8G8B8A8_SSCALED;
        case Format::R8G8B8A8_UINT:
            return VK_FORMAT_R8G8B8A8_UINT;
        case Format::R8G8B8A8_SINT:
            return VK_FORMAT_R8G8B8A8_SINT;
        case Format::R8G8B8A8_SRGB:
            return VK_FORMAT_R8G8B8A8_SRGB;
        case Format::B8G8R8A8_UNORM:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case Format::B8G8R8A8_SNORM:
            return VK_FORMAT_B8G8R8A8_SNORM;
        case Format::B8G8R8A8_USCALED:
            return VK_FORMAT_B8G8R8A8_USCALED;
        case Format::B8G8R8A8_SSCALED:
            return VK_FORMAT_B8G8R8A8_SSCALED;
        case Format::B8G8R8A8_UINT:
            return VK_FORMAT_B8G8R8A8_UINT;
        case Format::B8G8R8A8_SINT:
            return VK_FORMAT_B8G8R8A8_SINT;
        case Format::B8G8R8A8_SRGB:
            return VK_FORMAT_B8G8R8A8_SRGB;
        case Format::A8B8G8R8_UNORM:
            return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
        case Format::A8B8G8R8_SNORM:
            return VK_FORMAT_A8B8G8R8_SNORM_PACK32;
        case Format::A8B8G8R8_USCALED:
            return VK_FORMAT_A8B8G8R8_USCALED_PACK32;
        case Format::A8B8G8R8_SSCALED:
            return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
        case Format::A8B8G8R8_UINT:
            return VK_FORMAT_A8B8G8R8_UINT_PACK32;
        case Format::A8B8G8R8_SINT:
            return VK_FORMAT_A8B8G8R8_SINT_PACK32;
        case Format::A8B8G8R8_SRGB:
            return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
        case Format::A2R10G10B10_UNORM:
            return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
        case Format::A2R10G10B10_SNORM:
            return VK_FORMAT_A2R10G10B10_SNORM_PACK32;
        case Format::A2R10G10B10_USCALED:
            return VK_FORMAT_A2R10G10B10_USCALED_PACK32;
        case Format::A2R10G10B10_SSCALED:
            return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
        case Format::A2R10G10B10_UINT:
            return VK_FORMAT_A2R10G10B10_UINT_PACK32;
        case Format::A2R10G10B10_SINT:
            return VK_FORMAT_A2R10G10B10_SINT_PACK32;
        case Format::A2B10G10R10_UNORM:
            return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case Format::A2B10G10R10_SNORM:
            return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
        case Format::A2B10G10R10_USCALED:
            return VK_FORMAT_A2B10G10R10_USCALED_PACK32;
        case Format::A2B10G10R10_SSCALED:
            return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
        case Format::A2B10G10R10_UINT:
            return VK_FORMAT_A2B10G10R10_UINT_PACK32;
        case Format::A2B10G10R10_SINT:
            return VK_FORMAT_A2B10G10R10_SINT_PACK32;
        case Format::R16_UNORM:
            return VK_FORMAT_R16_UNORM;
        case Format::R16_SNORM:
            return VK_FORMAT_R16_SNORM;
        case Format::R16_USCALED:
            return VK_FORMAT_R16_USCALED;
        case Format::R16_SSCALED:
            return VK_FORMAT_R16_SSCALED;
        case Format::R16_UINT:
            return VK_FORMAT_R16_UINT;
        case Format::R16_SINT:
            return VK_FORMAT_R16_SINT;
        case Format::R16_SFLOAT:
            return VK_FORMAT_R16_SFLOAT;
        case Format::R16G16_UNORM:
            return VK_FORMAT_R16G16_UNORM;
        case Format::R16G16_SNORM:
            return VK_FORMAT_R16G16_SNORM;
        case Format::R16G16_USCALED:
            return VK_FORMAT_R16G16_USCALED;
        case Format::R16G16_SSCALED:
            return VK_FORMAT_R16G16_SSCALED;
        case Format::R16G16_UINT:
            return VK_FORMAT_R16G16_UINT;
        case Format::R16G16_SINT:
            return VK_FORMAT_R16G16_SINT;
        case Format::R16G16_SFLOAT:
            return VK_FORMAT_R16G16_SFLOAT;
        case Format::R16G16B16_UNORM:
            return VK_FORMAT_R16G16B16_UNORM;
        case Format::R16G16B16_SNORM:
            return VK_FORMAT_R16G16B16_SNORM;
        case Format::R16G16B16_USCALED:
            return VK_FORMAT_R16G16B16_USCALED;
        case Format::R16G16B16_SSCALED:
            return VK_FORMAT_R16G16B16_SSCALED;
        case Format::R16G16B16_UINT:
            return VK_FORMAT_R16G16B16_UINT;
        case Format::R16G16B16_SINT:
            return VK_FORMAT_R16G16B16_SINT;
        case Format::R16G16B16_SFLOAT:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case Format::R16G16B16A16_UNORM:
            return VK_FORMAT_R16G16B16A16_UNORM;
        case Format::R16G16B16A16_SNORM:
            return VK_FORMAT_R16G16B16A16_SNORM;
        case Format::R16G16B16A16_USCALED:
            return VK_FORMAT_R16G16B16A16_USCALED;
        case Format::R16G16B16A16_SSCALED:
            return VK_FORMAT_R16G16B16A16_SSCALED;
        case Format::R16G16B16A16_UINT:
            return VK_FORMAT_R16G16B16A16_UINT;
        case Format::R16G16B16A16_SINT:
            return VK_FORMAT_R16G16B16A16_SINT;
        case Format::R16G16B16A16_SFLOAT:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case Format::R32_UINT:
            return VK_FORMAT_R32_UINT;
        case Format::R32_SINT:
            return VK_FORMAT_R32_SINT;
        case Format::R32_SFLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case Format::R32G32_UINT:
            return VK_FORMAT_R32G32_UINT;
        case Format::R32G32_SINT:
            return VK_FORMAT_R32G32_SINT;
        case Format::R32G32_SFLOAT:
            return VK_FORMAT_R32G32_SFLOAT;
        case Format::R32G32B32_UINT:
            return VK_FORMAT_R32G32B32_UINT;
        case Format::R32G32B32_SINT:
            return VK_FORMAT_R32G32B32_SINT;
        case Format::R32G32B32_SFLOAT:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case Format::R32G32B32A32_UINT:
            return VK_FORMAT_R32G32B32A32_UINT;
        case Format::R32G32B32A32_SINT:
            return VK_FORMAT_R32G32B32A32_SINT;
        case Format::R32G32B32A32_SFLOAT:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case Format::R64_UINT:
            return VK_FORMAT_R64_UINT;
        case Format::R64_SINT:
            return VK_FORMAT_R64_SINT;
        case Format::R64_SFLOAT:
            return VK_FORMAT_R64_SFLOAT;
        case Format::R64G64_UINT:
            return VK_FORMAT_R64G64_UINT;
        case Format::R64G64_SINT:
            return VK_FORMAT_R64G64_SINT;
        case Format::R64G64_SFLOAT:
            return VK_FORMAT_R64G64_SFLOAT;
        case Format::R64G64B64_UINT:
            return VK_FORMAT_R64G64B64_UINT;
        case Format::R64G64B64_SINT:
            return VK_FORMAT_R64G64B64_SINT;
        case Format::R64G64B64_SFLOAT:
            return VK_FORMAT_R64G64B64_SFLOAT;
        case Format::R64G64B64A64_UINT:
            return VK_FORMAT_R64G64B64A64_UINT;
        case Format::R64G64B64A64_SINT:
            return VK_FORMAT_R64G64B64A64_SINT;
        case Format::R64G64B64A64_SFLOAT:
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        case Format::B10G11R11_UFLOAT:
            return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        case Format::E5B9G9R9_UFLOAT:
            return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        case Format::D16_UNORM:
            return VK_FORMAT_D16_UNORM;
        case Format::X8_D24_UNORM:
            return VK_FORMAT_X8_D24_UNORM_PACK32;
        case Format::D32_SFLOAT:
            return VK_FORMAT_D32_SFLOAT;
        case Format::S8_UINT:
            return VK_FORMAT_S8_UINT;
        case Format::D16_UNORM_S8_UINT:
            return VK_FORMAT_D16_UNORM_S8_UINT;
        case Format::D24_UNORM_S8_UINT:
            return VK_FORMAT_D24_UNORM_S8_UINT;
        case Format::D32_SFLOAT_S8_UINT:
            return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case Format::BC1_RGB_UNORM:
            return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
        case Format::BC1_RGB_SRGB:
            return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
        case Format::BC1_RGBA_UNORM:
            return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        case Format::BC1_RGBA_SRGB:
            return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        case Format::BC2_UNORM:
            return VK_FORMAT_BC2_UNORM_BLOCK;
        case Format::BC2_SRGB:
            return VK_FORMAT_BC2_SRGB_BLOCK;
        case Format::BC3_UNORM:
            return VK_FORMAT_BC3_UNORM_BLOCK;
        case Format::BC3_SRGB:
            return VK_FORMAT_BC3_SRGB_BLOCK;
        case Format::BC4_UNORM:
            return VK_FORMAT_BC4_UNORM_BLOCK;
        case Format::BC4_SNORM:
            return VK_FORMAT_BC4_SNORM_BLOCK;
        case Format::BC5_UNORM:
            return VK_FORMAT_BC5_UNORM_BLOCK;
        case Format::BC5_SNORM:
            return VK_FORMAT_BC5_SNORM_BLOCK;
        case Format::BC6H_UFLOAT:
            return VK_FORMAT_BC6H_UFLOAT_BLOCK;
        case Format::BC6H_SFLOAT:
            return VK_FORMAT_BC6H_SFLOAT_BLOCK;
        case Format::BC7_UNORM:
            return VK_FORMAT_BC7_UNORM_BLOCK;
        case Format::BC7_SRGB:
            return VK_FORMAT_BC7_SRGB_BLOCK;
        default:
            break;
        }
        throw base::BadArgumentException("Unsupported format.");
    }

    /**
     * \brief Converts a \ref BufferFormat to a \ref VkFormat.
     * \param format The \ref BufferFormat to convert.
     * \return A \ref VkFormat value representing the \ref BufferFormat.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkFormat to_format(const BufferFormat format)
    {
        switch (format)
        {
        case BufferFormat::X16F:
            return VK_FORMAT_R16_SFLOAT;
        case BufferFormat::X16I:
            return VK_FORMAT_R16_SINT;
        case BufferFormat::X16U:
            return VK_FORMAT_R16_UINT;
        case BufferFormat::XY16F:
            return VK_FORMAT_R16G16_SFLOAT;
        case BufferFormat::XY16I:
            return VK_FORMAT_R16G16_SINT;
        case BufferFormat::XY16U:
            return VK_FORMAT_R16G16_UINT;
        case BufferFormat::XYZ16F:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case BufferFormat::XYZ16I:
            return VK_FORMAT_R16G16B16_SINT;
        case BufferFormat::XYZ16U:
            return VK_FORMAT_R16G16B16_UINT;
        case BufferFormat::XYZW16F:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case BufferFormat::XYZW16I:
            return VK_FORMAT_R16G16B16A16_SINT;
        case BufferFormat::XYZW16U:
            return VK_FORMAT_R16G16B16A16_UINT;
        case BufferFormat::X32F:
            return VK_FORMAT_R32_SFLOAT;
        case BufferFormat::X32I:
            return VK_FORMAT_R32_SINT;
        case BufferFormat::X32U:
            return VK_FORMAT_R32_UINT;
        case BufferFormat::XY32F:
            return VK_FORMAT_R32G32_SFLOAT;
        case BufferFormat::XY32I:
            return VK_FORMAT_R32G32_SINT;
        case BufferFormat::XY32U:
            return VK_FORMAT_R32G32_UINT;
        case BufferFormat::XYZ32F:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case BufferFormat::XYZ32I:
            return VK_FORMAT_R32G32B32_SINT;
        case BufferFormat::XYZ32U:
            return VK_FORMAT_R32G32B32_UINT;
        case BufferFormat::XYZW32F:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case BufferFormat::XYZW32I:
            return VK_FORMAT_R32G32B32A32_SINT;
        case BufferFormat::XYZW32U:
            return VK_FORMAT_R32G32B32A32_UINT;
        default:
            break;
        }
        throw base::BadArgumentException("Unsupported buffer format.");
    }

    /**
     * \brief Converts a \ref VkFormat to a \ref Format.
     * \param format The \ref VkFormat to convert.
     * \return A \ref Format value representing the \ref VkFormat.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr Format from_format(const VkFormat format)
    {
        switch (format)
        {
        case VK_FORMAT_R4G4_UNORM_PACK8:
            return Format::R4G4_UNORM;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return Format::R4G4B4A4_UNORM;
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            return Format::B4G4R4A4_UNORM;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return Format::R5G6B5_UNORM;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return Format::B5G6R5_UNORM;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return Format::R5G5B5A1_UNORM;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return Format::B5G5R5A1_UNORM;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return Format::A1R5G5B5_UNORM;
        case VK_FORMAT_R8_UNORM:
            return Format::R8_UNORM;
        case VK_FORMAT_R8_SNORM:
            return Format::R8_SNORM;
        case VK_FORMAT_R8_USCALED:
            return Format::R8_USCALED;
        case VK_FORMAT_R8_SSCALED:
            return Format::R8_SSCALED;
        case VK_FORMAT_R8_UINT:
            return Format::R8_UINT;
        case VK_FORMAT_R8_SINT:
            return Format::R8_SINT;
        case VK_FORMAT_R8_SRGB:
            return Format::R8_SRGB;
        case VK_FORMAT_R8G8_UNORM:
            return Format::R8G8_UNORM;
        case VK_FORMAT_R8G8_SNORM:
            return Format::R8G8_SNORM;
        case VK_FORMAT_R8G8_USCALED:
            return Format::R8G8_USCALED;
        case VK_FORMAT_R8G8_SSCALED:
            return Format::R8G8_SSCALED;
        case VK_FORMAT_R8G8_UINT:
            return Format::R8G8_UINT;
        case VK_FORMAT_R8G8_SINT:
            return Format::R8G8_SINT;
        case VK_FORMAT_R8G8_SRGB:
            return Format::R8G8_SRGB;
        case VK_FORMAT_R8G8B8_UNORM:
            return Format::R8G8B8_UNORM;
        case VK_FORMAT_R8G8B8_SNORM:
            return Format::R8G8B8_SNORM;
        case VK_FORMAT_R8G8B8_USCALED:
            return Format::R8G8B8_USCALED;
        case VK_FORMAT_R8G8B8_SSCALED:
            return Format::R8G8B8_SSCALED;
        case VK_FORMAT_R8G8B8_UINT:
            return Format::R8G8B8_UINT;
        case VK_FORMAT_R8G8B8_SINT:
            return Format::R8G8B8_SINT;
        case VK_FORMAT_R8G8B8_SRGB:
            return Format::R8G8B8_SRGB;
        case VK_FORMAT_B8G8R8_UNORM:
            return Format::B8G8R8_UNORM;
        case VK_FORMAT_B8G8R8_SNORM:
            return Format::B8G8R8_SNORM;
        case VK_FORMAT_B8G8R8_USCALED:
            return Format::B8G8R8_USCALED;
        case VK_FORMAT_B8G8R8_SSCALED:
            return Format::B8G8R8_SSCALED;
        case VK_FORMAT_B8G8R8_UINT:
            return Format::B8G8R8_UINT;
        case VK_FORMAT_B8G8R8_SINT:
            return Format::B8G8R8_SINT;
        case VK_FORMAT_B8G8R8_SRGB:
            return Format::B8G8R8_SRGB;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return Format::R8G8B8A8_UNORM;
        case VK_FORMAT_R8G8B8A8_SNORM:
            return Format::R8G8B8A8_SNORM;
        case VK_FORMAT_R8G8B8A8_USCALED:
            return Format::R8G8B8A8_USCALED;
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return Format::R8G8B8A8_SSCALED;
        case VK_FORMAT_R8G8B8A8_UINT:
            return Format::R8G8B8A8_UINT;
        case VK_FORMAT_R8G8B8A8_SINT:
            return Format::R8G8B8A8_SINT;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return Format::R8G8B8A8_SRGB;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return Format::B8G8R8A8_UNORM;
        case VK_FORMAT_B8G8R8A8_SNORM:
            return Format::B8G8R8A8_SNORM;
        case VK_FORMAT_B8G8R8A8_USCALED:
            return Format::B8G8R8A8_USCALED;
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return Format::B8G8R8A8_SSCALED;
        case VK_FORMAT_B8G8R8A8_UINT:
            return Format::B8G8R8A8_UINT;
        case VK_FORMAT_B8G8R8A8_SINT:
            return Format::B8G8R8A8_SINT;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return Format::B8G8R8A8_SRGB;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return Format::A8B8G8R8_UNORM;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return Format::A8B8G8R8_SNORM;
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return Format::A8B8G8R8_USCALED;
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return Format::A8B8G8R8_SSCALED;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return Format::A8B8G8R8_UINT;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return Format::A8B8G8R8_SINT;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return Format::A8B8G8R8_SRGB;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return Format::A2R10G10B10_UNORM;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return Format::A2R10G10B10_SNORM;
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            return Format::A2R10G10B10_USCALED;
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            return Format::A2R10G10B10_SSCALED;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return Format::A2R10G10B10_UINT;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return Format::A2R10G10B10_SINT;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return Format::A2B10G10R10_UNORM;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return Format::A2B10G10R10_SNORM;
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            return Format::A2B10G10R10_USCALED;
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            return Format::A2B10G10R10_SSCALED;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return Format::A2B10G10R10_UINT;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return Format::A2B10G10R10_SINT;
        case VK_FORMAT_R16_UNORM:
            return Format::R16_UNORM;
        case VK_FORMAT_R16_SNORM:
            return Format::R16_SNORM;
        case VK_FORMAT_R16_USCALED:
            return Format::R16_USCALED;
        case VK_FORMAT_R16_SSCALED:
            return Format::R16_SSCALED;
        case VK_FORMAT_R16_UINT:
            return Format::R16_UINT;
        case VK_FORMAT_R16_SINT:
            return Format::R16_SINT;
        case VK_FORMAT_R16_SFLOAT:
            return Format::R16_SFLOAT;
        case VK_FORMAT_R16G16_UNORM:
            return Format::R16G16_UNORM;
        case VK_FORMAT_R16G16_SNORM:
            return Format::R16G16_SNORM;
        case VK_FORMAT_R16G16_USCALED:
            return Format::R16G16_USCALED;
        case VK_FORMAT_R16G16_SSCALED:
            return Format::R16G16_SSCALED;
        case VK_FORMAT_R16G16_UINT:
            return Format::R16G16_UINT;
        case VK_FORMAT_R16G16_SINT:
            return Format::R16G16_SINT;
        case VK_FORMAT_R16G16_SFLOAT:
            return Format::R16G16_SFLOAT;
        case VK_FORMAT_R16G16B16_UNORM:
            return Format::R16G16B16_UNORM;
        case VK_FORMAT_R16G16B16_SNORM:
            return Format::R16G16B16_SNORM;
        case VK_FORMAT_R16G16B16_USCALED:
            return Format::R16G16B16_USCALED;
        case VK_FORMAT_R16G16B16_SSCALED:
            return Format::R16G16B16_SSCALED;
        case VK_FORMAT_R16G16B16_UINT:
            return Format::R16G16B16_UINT;
        case VK_FORMAT_R16G16B16_SINT:
            return Format::R16G16B16_SINT;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return Format::R16G16B16_SFLOAT;
        case VK_FORMAT_R16G16B16A16_UNORM:
            return Format::R16G16B16A16_UNORM;
        case VK_FORMAT_R16G16B16A16_SNORM:
            return Format::R16G16B16A16_SNORM;
        case VK_FORMAT_R16G16B16A16_USCALED:
            return Format::R16G16B16A16_USCALED;
        case VK_FORMAT_R16G16B16A16_SSCALED:
            return Format::R16G16B16A16_SSCALED;
        case VK_FORMAT_R16G16B16A16_UINT:
            return Format::R16G16B16A16_UINT;
        case VK_FORMAT_R16G16B16A16_SINT:
            return Format::R16G16B16A16_SINT;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return Format::R16G16B16A16_SFLOAT;
        case VK_FORMAT_R32_UINT:
            return Format::R32_UINT;
        case VK_FORMAT_R32_SINT:
            return Format::R32_SINT;
        case VK_FORMAT_R32_SFLOAT:
            return Format::R32_SFLOAT;
        case VK_FORMAT_R32G32_UINT:
            return Format::R32G32_UINT;
        case VK_FORMAT_R32G32_SINT:
            return Format::R32G32_SINT;
        case VK_FORMAT_R32G32_SFLOAT:
            return Format::R32G32_SFLOAT;
        case VK_FORMAT_R32G32B32_UINT:
            return Format::R32G32B32_UINT;
        case VK_FORMAT_R32G32B32_SINT:
            return Format::R32G32B32_SINT;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return Format::R32G32B32_SFLOAT;
        case VK_FORMAT_R32G32B32A32_UINT:
            return Format::R32G32B32A32_UINT;
        case VK_FORMAT_R32G32B32A32_SINT:
            return Format::R32G32B32A32_SINT;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return Format::R32G32B32A32_SFLOAT;
        case VK_FORMAT_R64_UINT:
            return Format::R64_UINT;
        case VK_FORMAT_R64_SINT:
            return Format::R64_SINT;
        case VK_FORMAT_R64_SFLOAT:
            return Format::R64_SFLOAT;
        case VK_FORMAT_R64G64_UINT:
            return Format::R64G64_UINT;
        case VK_FORMAT_R64G64_SINT:
            return Format::R64G64_SINT;
        case VK_FORMAT_R64G64_SFLOAT:
            return Format::R64G64_SFLOAT;
        case VK_FORMAT_R64G64B64_UINT:
            return Format::R64G64B64_UINT;
        case VK_FORMAT_R64G64B64_SINT:
            return Format::R64G64B64_SINT;
        case VK_FORMAT_R64G64B64_SFLOAT:
            return Format::R64G64B64_SFLOAT;
        case VK_FORMAT_R64G64B64A64_UINT:
            return Format::R64G64B64A64_UINT;
        case VK_FORMAT_R64G64B64A64_SINT:
            return Format::R64G64B64A64_SINT;
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return Format::R64G64B64A64_SFLOAT;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return Format::B10G11R11_UFLOAT;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            return Format::E5B9G9R9_UFLOAT;
        case VK_FORMAT_D16_UNORM:
            return Format::D16_UNORM;
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            return Format::X8_D24_UNORM;
        case VK_FORMAT_D32_SFLOAT:
            return Format::D32_SFLOAT;
        case VK_FORMAT_S8_UINT:
            return Format::S8_UINT;
        case VK_FORMAT_D16_UNORM_S8_UINT:
            return Format::D16_UNORM_S8_UINT;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return Format::D24_UNORM_S8_UINT;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return Format::D32_SFLOAT_S8_UINT;
        case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
            return Format::BC1_RGB_UNORM;
        case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
            return Format::BC1_RGB_SRGB;
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
            return Format::BC1_RGBA_UNORM;
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
            return Format::BC1_RGBA_SRGB;
        case VK_FORMAT_BC2_UNORM_BLOCK:
            return Format::BC2_UNORM;
        case VK_FORMAT_BC2_SRGB_BLOCK:
            return Format::BC2_SRGB;
        case VK_FORMAT_BC3_UNORM_BLOCK:
            return Format::BC3_UNORM;
        case VK_FORMAT_BC3_SRGB_BLOCK:
            return Format::BC3_SRGB;
        case VK_FORMAT_BC4_UNORM_BLOCK:
            return Format::BC4_UNORM;
        case VK_FORMAT_BC4_SNORM_BLOCK:
            return Format::BC4_SNORM;
        case VK_FORMAT_BC5_UNORM_BLOCK:
            return Format::BC5_UNORM;
        case VK_FORMAT_BC5_SNORM_BLOCK:
            return Format::BC5_SNORM;
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
            return Format::BC6H_UFLOAT;
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
            return Format::BC6H_SFLOAT;
        case VK_FORMAT_BC7_UNORM_BLOCK:
            return Format::BC7_UNORM;
        case VK_FORMAT_BC7_SRGB_BLOCK:
            return Format::BC7_SRGB;
        default:
            break;
        }
        return Format::Other;
    }

    /**
     * \brief Converts a \ref ShaderStage to a \ref VkShaderStageFlagBits.
     * \param stage The \ref ShaderStage to convert.
     * \return A \ref VkShaderStageFlagBits value representing the \ref ShaderStage.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkShaderStageFlagBits to_shader_stage(const ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::Vertex:
            return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::TessellationControl:
            return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case ShaderStage::TessellationEvaluation:
            return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case ShaderStage::Geometry:
            return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
        case ShaderStage::Fragment:
            return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderStage::Compute:
            return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        default:
            break;
        }
        throw base::BadArgumentException("Unsupported shader type.");
    }

    /**
     * \brief Converts a \ref PolygonMode to a \ref VkPolygonMode.
     * \param mode The \ref PolygonMode to convert.
     * \return A \ref VkPolygonMode value representing the \ref PolygonMode.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkPolygonMode to_polygon_mode(const PolygonMode mode)
    {
        switch (mode)
        {
        case PolygonMode::Solid:
            return VkPolygonMode::VK_POLYGON_MODE_FILL;
        case PolygonMode::WireFrame:
            return VkPolygonMode::VK_POLYGON_MODE_LINE;
        case PolygonMode::Point:
            return VkPolygonMode::VK_POLYGON_MODE_POINT;
        }
        throw base::BadArgumentException("Unsupported polygon mode.");
    }

    /**
     * \brief Converts a \ref CullMode to a \ref VkCullModeFlags.
     * \param mode The \ref CullMode to convert.
     * \return A \ref VkCullModeFlags value representing the \ref CullMode.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkCullModeFlags to_cull_mode(const CullMode mode)
    {
        switch (mode)
        {
        case CullMode::BackFaces:
            return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
        case CullMode::FrontFaces:
            return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
        case CullMode::Both:
            return VkCullModeFlagBits::VK_CULL_MODE_FRONT_AND_BACK;
        case CullMode::Disabled:
            return VkCullModeFlagBits::VK_CULL_MODE_NONE;
        }
        throw base::BadArgumentException("Unsupported cull mode.");
    }

    /**
     * \brief Converts a \ref PrimitiveTopology to a \ref VkPrimitiveTopology.
     * \param topology The \ref PrimitiveTopology to convert.
     * \return A \ref VkPrimitiveTopology value representing the \ref PrimitiveTopology.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkPrimitiveTopology to_primitive_topology(const PrimitiveTopology topology)
    {
        switch (topology)
        {
        case PrimitiveTopology::LineList:
            return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case PrimitiveTopology::LineStrip:
            return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case PrimitiveTopology::PointList:
            return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case PrimitiveTopology::TriangleList:
            return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case PrimitiveTopology::TriangleStrip:
            return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        }
        throw base::BadArgumentException("Unsupported primitive topology.");
    }

    /**
     * \brief Converts a \ref BufferFormat to a \ref VkSampleCountFlagBits.
     * \param level The \ref BufferFormat to convert.
     * \return A \ref VkSampleCountFlagBits value representing the \ref BufferFormat.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkSampleCountFlagBits to_samples(const MultiSamplingLevel level)
    {
        switch (level)
        {
        case MultiSamplingLevel::X1:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
        case MultiSamplingLevel::X2:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT;
        case MultiSamplingLevel::X4:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT;
        case MultiSamplingLevel::X8:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT;
        case MultiSamplingLevel::X16:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT;
        case MultiSamplingLevel::X32:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT;
        case MultiSamplingLevel::X64:
            return VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT;
        }
        throw base::BadArgumentException("Unsupported multi sampling level.");
    }

    /**
     * \brief Converts a \ref BlendFactor to a \ref VkBlendFactor.
     * \param factor The \ref BlendFactor to convert.
     * \return A \ref VkBlendFactor value representing the \ref BlendFactor.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkBlendFactor to_blend_factor(const BlendFactor factor)
    {
        switch (factor)
        {
        case BlendFactor::Zero:
            return VkBlendFactor::VK_BLEND_FACTOR_ZERO;
        case BlendFactor::One:
            return VkBlendFactor::VK_BLEND_FACTOR_ONE;
        case BlendFactor::SourceColor:
            return VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR;
        case BlendFactor::OneMinusSourceColor:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DestinationColor:
            return VK_BLEND_FACTOR_DST_COLOR;
        case BlendFactor::OneMinusDestinationColor:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case BlendFactor::SourceAlpha:
            return VK_BLEND_FACTOR_SRC_ALPHA;
        case BlendFactor::OneMinusSourceAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DestinationAlpha:
            return VK_BLEND_FACTOR_DST_ALPHA;
        case BlendFactor::OneMinusDestinationAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantColor:
            return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case BlendFactor::OneMinusConstantColor:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case BlendFactor::ConstantAlpha:
            return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SourceAlphaSaturate:
            return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        case BlendFactor::Source1Color:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case BlendFactor::OneMinusSource1Color:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case BlendFactor::Source1Alpha:
            return VK_BLEND_FACTOR_SRC1_ALPHA;
        case BlendFactor::OneMinusSource1Alpha:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        }
        throw base::BadArgumentException("Unsupported blend factor.");
    }

    /**
     * \brief Converts a \ref BlendOperation to a \ref VkBlendOp.
     * \param operation The \ref BlendOperation to convert.
     * \return A \ref VkBlendOp value representing the \ref BlendOperation.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkBlendOp to_blend_operation(const BlendOperation operation)
    {
        switch (operation)
        {
        case BlendOperation::Add:
            return VkBlendOp::VK_BLEND_OP_ADD;
        case BlendOperation::Subtract:
            return VkBlendOp::VK_BLEND_OP_SUBTRACT;
        case BlendOperation::ReverseSubtract:
            return VkBlendOp::VK_BLEND_OP_REVERSE_SUBTRACT;
        case BlendOperation::Minimum:
            return VkBlendOp::VK_BLEND_OP_MIN;
        case BlendOperation::Maximum:
            return VkBlendOp::VK_BLEND_OP_MAX;
        }
        throw base::BadArgumentException("Unsupported blend operation.");
    }

    /**
     * \brief Converts a \ref CompareOperation to a \ref VkCompareOp.
     * \param operation The \ref CompareOperation to convert.
     * \return A \ref VkCompareOp value representing the \ref CompareOperation.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkCompareOp to_compare_operation(const CompareOperation operation)
    {
        switch (operation)
        {
        case CompareOperation::Never:
            return VkCompareOp::VK_COMPARE_OP_NEVER;
        case CompareOperation::Less:
            return VkCompareOp::VK_COMPARE_OP_LESS;
        case CompareOperation::Greater:
            return VkCompareOp::VK_COMPARE_OP_GREATER;
        case CompareOperation::Equal:
            return VkCompareOp::VK_COMPARE_OP_EQUAL;
        case CompareOperation::LessEqual:
            return VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOperation::GreaterEqual:
            return VkCompareOp::VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareOperation::NotEqual:
            return VkCompareOp::VK_COMPARE_OP_NOT_EQUAL;
        case CompareOperation::Always:
            return VkCompareOp::VK_COMPARE_OP_ALWAYS;
        }
        throw base::BadArgumentException("Unsupported compare operation.");
    }

    /**
     * \brief Converts a \ref StencilOperation to a \ref VkStencilOp.
     * \param operation The \ref StencilOperation to convert.
     * \return A \ref VkStencilOp value representing the \ref StencilOperation.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkStencilOp to_stencil_operation(const StencilOperation operation)
    {
        switch (operation)
        {
        case StencilOperation::Keep:
            return VkStencilOp::VK_STENCIL_OP_KEEP;
        case StencilOperation::Zero:
            return VkStencilOp::VK_STENCIL_OP_ZERO;
        case StencilOperation::Invert:
            return VkStencilOp::VK_STENCIL_OP_INVERT;
        case StencilOperation::Replace:
            return VkStencilOp::VK_STENCIL_OP_REPLACE;
        case StencilOperation::IncrementClamp:
            return VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case StencilOperation::IncrementWrap:
            return VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case StencilOperation::DecrementClamp:
            return VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case StencilOperation::DecrementWrap:
            return VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_WRAP;
        }
        throw base::BadArgumentException("Unsupported stencil operation.");
    }

    /**
     * \brief Converts a \ref ImageDimensions to a \ref VkImageViewType.
     * \param dimension The \ref ImageDimensions to convert.
     * \param layers The number of layers in the image. (default = 1)
     * \return A \ref VkImageViewType value representing the \ref ImageDimensions.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkImageViewType to_image_view_type(const ImageDimensions dimension, const unsigned layers = 1)
    {
        switch (dimension)
        {
        case ImageDimensions::DIM_1:
            return layers == 1 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_1D : VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        case ImageDimensions::DIM_2:
            return layers == 1 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_2D : VkImageViewType::VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case ImageDimensions::DIM_3:
            return VkImageViewType::VK_IMAGE_VIEW_TYPE_3D;
        case ImageDimensions::CUBE:
            return VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE;
        }
        throw base::BadArgumentException("Unsupported image dimensions.");
    }

    /**
     * \brief Converts a \ref ImageDimensions to a \ref VkImageType.
     * \param dimension The \ref ImageDimensions to convert.
     * \return A \ref VkImageType value representing the \ref ImageDimensions.
     */
    [[nodiscard]] SPARK_RENDER_VK_EXPORT constexpr VkImageType to_image_type(const ImageDimensions dimension)
    {
        switch (dimension)
        {
        case ImageDimensions::DIM_1:
            return VkImageType::VK_IMAGE_TYPE_1D;
        case ImageDimensions::DIM_2:
        case ImageDimensions::CUBE:
            return VkImageType::VK_IMAGE_TYPE_2D;
        case ImageDimensions::DIM_3:
            return VkImageType::VK_IMAGE_TYPE_3D;
        }
        throw base::BadArgumentException("Unsupported image dimensions.");
    }
}
