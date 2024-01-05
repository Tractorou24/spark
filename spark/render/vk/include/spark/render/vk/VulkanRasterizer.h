#pragma once

#include "spark/render/Rasterizer.h"
#include "spark/render/vk/Export.h"

namespace spark::render::vk
{
    /**
     * \brief Vulkan implementation of \ref IRasterizer.
     */
    class SPARK_RENDER_VK_EXPORT VulkanRasterizer final : public Rasterizer
    {
    public:
        /**
         * \brief Initializes a new rasterizer state for Vulkan.
         * \param polygon_mode The \ref PolygonMode of the rasterizer.
         * \param cull_mode The \ref CullMode of the rasterizer.
         * \param cull_order The \ref CullOrder of the rasterizer.
         * \param line_width The line width of the rasterizer.
         * \param depth_stencil_state The \ref DepthStencilState of the rasterizer.
         */
        explicit VulkanRasterizer(PolygonMode polygon_mode, CullMode cull_mode, CullOrder cull_order, float line_width, const DepthStencilState& depth_stencil_state) noexcept;
        ~VulkanRasterizer() override = default;

        VulkanRasterizer(const VulkanRasterizer& other) = delete;
        VulkanRasterizer(VulkanRasterizer&& other) noexcept = delete;
        VulkanRasterizer& operator=(const VulkanRasterizer& other) = delete;
        VulkanRasterizer& operator=(VulkanRasterizer&& other) noexcept = delete;

        /**
         * \brief Sets the line width of the rasterizer.
         * \param line_width The new line width.
         *
         * \note Using this function requires the `wide lines` feature to be enabled.
         * \warning If the feature is not supported, you must ensure that the line width is `1.0f`.
         */
        void updateLineWidth(float line_width) noexcept;
    };
}
