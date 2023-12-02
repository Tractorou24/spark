#include "spark/render/vk/VulkanSurface.h"

#include "vulkan/vulkan.h"

namespace spark::render::vk
{
    struct VulkanSurface::Impl
    {
        friend VulkanSurface;

    public:
        explicit Impl(const VkInstance& instance)
            : m_instance(instance) {}

    private:
        VkInstance m_instance = VK_NULL_HANDLE;
    };

    VulkanSurface::VulkanSurface(const VkSurfaceKHR& surface, const VkInstance& instance)
        : Resource(surface), m_impl(std::make_unique<Impl>(instance)) {}

    VulkanSurface::~VulkanSurface()
    {
        vkDestroySurfaceKHR(m_impl->m_instance, handle(), nullptr);
    }

    const VkInstance& VulkanSurface::instance() const noexcept
    {
        return m_impl->m_instance;
    }
}
