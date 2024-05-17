#pragma once

#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanImage.h"

#include "spark/render/Barrier.h"

#include <memory>

namespace spark::render::vk
{
    class SPARK_RENDER_VK_EXPORT VulkanBarrier final : public Barrier<IVulkanBuffer, IVulkanImage>
    {
    public:
        constexpr explicit VulkanBarrier() noexcept;

        [[nodiscard]] constexpr PipelineStage syncBefore() const noexcept override;
        [[nodiscard]] constexpr PipelineStage syncAfter() const noexcept override;
        constexpr void wait(ResourceAccess access_before, ResourceAccess access_after) noexcept override;
        constexpr void transition(buffer_type& buffer, ResourceAccess access_before, ResourceAccess access_after) override;
        constexpr void transition(image_type& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout layout) override;
        constexpr void transition(image_type& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout from_layout, ImageLayout to_layout) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
