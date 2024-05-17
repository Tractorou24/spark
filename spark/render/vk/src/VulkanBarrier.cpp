#include "spark/render/vk/VulkanBarrier.h"

namespace spark::render::vk
{
    struct VulkanBarrier::Impl final
    {
        // ...
    };

    constexpr VulkanBarrier::VulkanBarrier() noexcept {}

    constexpr PipelineStage VulkanBarrier::syncBefore() const noexcept
    {
        return PipelineStage::None;
    }

    constexpr PipelineStage VulkanBarrier::syncAfter() const noexcept
    {
        return PipelineStage::None;
    }

    constexpr void VulkanBarrier::wait(ResourceAccess access_before, ResourceAccess access_after) noexcept {}
    constexpr void VulkanBarrier::transition(buffer_type& buffer, ResourceAccess access_before, ResourceAccess access_after) {}
    constexpr void VulkanBarrier::transition(image_type& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout layout) {}
    constexpr void VulkanBarrier::transition(image_type& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout from_layout, ImageLayout to_layout) {}
}
