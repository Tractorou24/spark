#pragma once

#include "spark/render/DeviceMemory.h"
#include "spark/render/Export.h"
#include "spark/render/StateResource.h"
#include "spark/render/Format.h"

#include "spark/math/Vector3.h"

namespace spark::render
{
    /**
     * \brief Describes the dimensions of a image resource, i.e. the dimensions that are required to access a texel or describe the image extent.
     */
    enum class ImageDimensions
    {
        /// \brief Represents a 1D image.
        DIM_1 = 0x01,

        /// \brief Represents a 2D image.
        DIM_2 = 0x02,

        /// \brief Represents a 3D image.
        DIM_3 = 0x03,

        /// \brief Represents a set of six 2D images that are used to build a cube map.
        CUBE = 0x04
    };

    /**
     * \brief Specifies the layout of an \ref IImage resource.
     */
    enum class ImageLayout
    {
        /**
         * \brief A common image layout that allows for all types of access (shader resource, transfer destination, transfer source).
         * \details This image layout translates to `VK_IMAGE_LAYOUT_GENERAL` in the Vulkan 🌋 backend.
         */
        Common = 0x00000001,

        /**
         * \brief Indicates that the image is used as a read-only storage or texel buffer.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL` in the Vulkan 🌋 backend.
         */
        ShaderResource = 0x00000002,

        /**
         * \brief Indicates that the image is used as a read-write storage or texel buffer.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_GENERAL` in the Vulkan 🌋 backend.
         */
        ReadWrite = 0x00000003,

        /**
         * \brief Allows the image to be used as a source for transfer operations.
         * \details This image layout translates to  `VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL` in the Vulkan 🌋 backend.
         */
        CopySource = 0x00000010,

        /**
         * \brief Allows the image to be used as a destination for transfer operations.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL` in the Vulkan 🌋 backend.
         **/
        CopyDestination = 0x00000011,

        /**
         * \brief Indicates that the image is used as a render target.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL` in the Vulkan 🌋 backend.
        **/
        RenderTarget = 0x00000020,

        /**
         * \brief Indicates that image is used as a read-only depth/stencil target.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL` in the Vulkan 🌋 backend.
        **/
        DepthRead = 0x00000021,

        /**
         * \brief Indicates that the image is used as a write-only depth/stencil target.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL` in the Vulkan 🌋 backend.
        **/
        DepthWrite = 0x00000022,

        /**
         * \brief Indicates that the image is presented on a swap chain.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_PRESENT_SRC_KHR` in the Vulkan 🌋 backend.
         *
         * \note Typically you do not want to manually transition a resource into this state. Render target transitions are automatically managed by RenderPasses.
        **/
        Present = 0x00000023,

        /**
         * \brief Indicates that the image is resolved from a multi-sampled image.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL` in the Vulkan 🌋 backend.
        **/
        ResolveSource = 0x00000024,

        /**
         * \brief Indicates that the image is a render-target that a multi-sampled image is resolved into.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL` in the Vulkan 🌋 backend.
        **/
        ResolveDestination = 0x00000025,

        /**
         * \brief The layout of the image is not known by the engine.
         * \details This image layout translates to `VK_IMAGE_LAYOUT_UNDEFINED` in the Vulkan 🌋 backend.
         *
         * \note Indicates that an image's layout is not known, which typically happens after creating image resources. It is not valid to transition any resource into this state.
        **/
        Undefined = 0x7FFFFFFF
    };

    /**
     * \brief Describes the number of samples with which a \ref IImage is sampled.
     * Default level is `x1`, which means that no multi-sampling is used.
     */
    enum class MultiSamplingLevel
    {
        /// <summary>
        /// The default number of samples. Multi-sampling will be de-activated, if this sampling level is used.
        /// </summary>
        X1  = 0x00000001,
        X2  = 0x00000002,
        X4  = 0x00000004,
        X8  = 0x00000008,
        X16 = 0x00000010,
        X32 = 0x00000020,
        X64 = 0x00000040
    };

    class SPARK_RENDER_EXPORT IImage : public virtual IStateResource, public virtual IDeviceMemory
    {
    public:
        ~IImage() noexcept override = default;

        // Needed to make clang happy, else it complains about a hidden overloaded virtual function (but it is not hidden)
        using IDeviceMemory::size;

        /**
         * \brief Get the size of the image (in bytes) at the specified mip-map level.
         * \return The size of the image (in bytes) at the specified mip-map or `0` if the mip-map level is invalid.
         *
         * \note The returned size is only for one layer. If you have multiple layers, you need to multiply the returned size with the number of layers.
         */
        [[nodiscard]] virtual std::size_t size(unsigned int level) const = 0;

        /**
         * \brief Get the extent of the image at the specified mip-map level.
         * \param level The mip-map level for which to get the extent.
         * \return The extent of the image at the specified mip-map level or `0` if the mip-map level is invalid.
         *
         * \note Not all components of the returned extent are valid for all image dimensions. For example, a 1D image only has a valid `x` component. See \ref ImageDimensions.
         */
        [[nodiscard]] virtual math::Vector3<unsigned> extent(unsigned level = 0) const noexcept = 0;

        /**
         * \brief Gets the internal format of the image.
         * \return A \ref Format that describes the internal format of the image.
         */
        [[nodiscard]] virtual Format format() const noexcept = 0;

        /**
         * \brief Gets the dimensions of the image. (1D, 2D, 3D, Cube)
         * \return A \ref ImageDimensions that describes the dimensions of the image.
         *
         * \note This imply various things, especially which values of the \ref extent() are valid.
         */
        [[nodiscard]] virtual ImageDimensions dimensions() const noexcept = 0;

        /**
         * \brief Gets the number of mip-map levels of the image.
         * \return The number of mip-map levels of the image.
         */
        [[nodiscard]] virtual unsigned int levels() const noexcept = 0;

        /**
         * \brief Gets the number of layers of the image.
         * \return The number of layers (slices) of the image.
         */
        [[nodiscard]] virtual unsigned int layers() const noexcept = 0;

        /**
         * \brief Returns the number of planes of the image.
         * \return The number of planes of the image.
         *
         * \note This values is dictated from the \ref format() of the image.
         */
        [[nodiscard]] virtual unsigned int planes() const noexcept = 0;

        /**
         * \brief Gets the number of samples with which the image is sampled.
         * \return The number of samples of the image.
         */
        [[nodiscard]] virtual MultiSamplingLevel samples() const noexcept = 0;

        /**
         * \brief Gets the layout of the image.
         * \param sub_resource The sub-resource id for which to get the layout.
         * \return The layout of the image.
         */
        [[nodiscard]] virtual ImageLayout layout(unsigned int sub_resource = 0) const = 0;

        /**
         * \brief Returns the sub-resource ID for a combination of mip-map \p level, array \p layer and \p plane.
         * \param level The mip-map level of the sub-resource.
         * \param layer The array layer of the sub-resource.
         * \param plane The plane of the sub-resource.
         * \return The sub-resource ID for the specified combination of \p level, \p layer and \p plane.
         */
        [[nodiscard]] virtual unsigned int subResourceId(unsigned& level, unsigned& layer, unsigned& plane) const noexcept
        {
            return level + layer * levels() + plane * levels() * layers();
        }

        /**
         * \brief Returns the sub-resource ID for a combination of mip-map \p level, array \p layer and \p plane.
         * \param subresource The sub-resource ID.
         * \return The sub-resource ID for the specified combination of \p level, \p layer and \p plane.
         */
        [[nodiscard]] std::tuple<unsigned, unsigned, unsigned> resolveSubresource(const unsigned subresource) const noexcept
        {
            const unsigned level = levels();
            const unsigned resources_per_plane = level * layers();
            return std::make_tuple<unsigned, unsigned, unsigned>(subresource / resources_per_plane, subresource % resources_per_plane / level, subresource % level);
        }
    };
}

template <>
struct std::formatter<spark::render::MultiSamplingLevel> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::MultiSamplingLevel level, auto& ctx)
    {
        switch (level) // NOLINT(clang-diagnostic-switch-enum): X1 is handled after the switch statement.
        {
        case spark::render::MultiSamplingLevel::X2:
            return std::format_to(ctx.out(), "X2");
        case spark::render::MultiSamplingLevel::X4:
            return std::format_to(ctx.out(), "X4");
        case spark::render::MultiSamplingLevel::X8:
            return std::format_to(ctx.out(), "X8");
        case spark::render::MultiSamplingLevel::X16:
            return std::format_to(ctx.out(), "X16");
        case spark::render::MultiSamplingLevel::X32:
            return std::format_to(ctx.out(), "X32");
        case spark::render::MultiSamplingLevel::X64:
            return std::format_to(ctx.out(), "X64");
        default:
            break;
        }
        return std::format_to(ctx.out(), "X1");
    }
};
