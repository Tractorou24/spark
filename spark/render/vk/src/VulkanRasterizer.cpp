#include "spark/render/vk/VulkanRasterizer.h"

namespace spark::render::vk
{
    VulkanRasterizer::VulkanRasterizer(const PolygonMode polygon_mode,
                                       const CullMode cull_mode,
                                       const CullOrder cull_order,
                                       const float line_width,
                                       const DepthStencilState& depth_stencil_state) noexcept
        : Rasterizer(polygon_mode, cull_mode, cull_order, line_width, depth_stencil_state) {}

    void VulkanRasterizer::updateLineWidth(const float line_width) noexcept
    {
        lineWidth() = line_width;
    }
}
