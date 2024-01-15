#include "spark/render/vk/VulkanInputAttachmentMapping.h"

namespace spark::render::vk
{
    struct VulkanInputAttachmentMapping::Impl
    {
        friend class VulkanInputAttachmentMapping;

    public:
        explicit Impl(const VulkanRenderPass* render_pass, const RenderTarget& render_target, const unsigned location)
            : m_renderPass(render_pass), m_renderTarget(render_target), m_location(location) {}

    private:
        const VulkanRenderPass* m_renderPass;
        RenderTarget m_renderTarget;
        unsigned m_location;
    };

    VulkanInputAttachmentMapping::VulkanInputAttachmentMapping()
        : m_impl(std::make_unique<Impl>(nullptr, RenderTarget(), 0)) {}

    VulkanInputAttachmentMapping::VulkanInputAttachmentMapping(const VulkanRenderPass& render_pass, const RenderTarget& render_target, unsigned location)
        : m_impl(std::make_unique<Impl>(&render_pass, render_target, location)) {}

    VulkanInputAttachmentMapping::~VulkanInputAttachmentMapping() = default;

    VulkanInputAttachmentMapping::VulkanInputAttachmentMapping(const VulkanInputAttachmentMapping& other)
    {
        if (this == &other)
            return;

        m_impl = std::make_unique<Impl>(other.m_impl->m_renderPass, other.m_impl->m_renderTarget, other.m_impl->m_location);
    }

    VulkanInputAttachmentMapping::VulkanInputAttachmentMapping(VulkanInputAttachmentMapping&& other) noexcept
    {
        if (this == &other)
            return;

        m_impl = std::move(other.m_impl);
    }

    VulkanInputAttachmentMapping& VulkanInputAttachmentMapping::operator=(const VulkanInputAttachmentMapping& other)
    {
        if (this == &other)
            return *this;

        m_impl = std::make_unique<Impl>(other.m_impl->m_renderPass, other.m_impl->m_renderTarget, other.m_impl->m_location);
        return *this;
    }

    VulkanInputAttachmentMapping& VulkanInputAttachmentMapping::operator=(VulkanInputAttachmentMapping&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_impl = std::move(other.m_impl);
        return *this;
    }

    const VulkanRenderPass* VulkanInputAttachmentMapping::source() const noexcept
    {
        return m_impl->m_renderPass;
    }

    const RenderTarget& VulkanInputAttachmentMapping::renderTarget() const noexcept
    {
        return m_impl->m_renderTarget;
    }

    unsigned VulkanInputAttachmentMapping::location() const noexcept
    {
        return m_impl->m_location;
    }
}
