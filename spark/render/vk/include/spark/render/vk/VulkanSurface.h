#pragma once

#include "spark/render/Resource.h"
#include "spark/render/Surface.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"

#include <memory>

SPARK_FWD_DECLARE_VK_HANDLE(VkSurfaceKHR)
SPARK_FWD_DECLARE_VK_HANDLE(VkInstance)

namespace spark::render::vk
{
    /**
     * \brief Implementation of \ref ISurface for Vulkan.
     */
    class SPARK_RENDER_VK_EXPORT VulkanSurface final : public ISurface, public Resource<VkSurfaceKHR>
    {
    public:
        /**
        * \brief Initializes the surface from a surface, instance and window handle.
        * \param surface A \ref VulkanSurfaceKHR handle to the vulkan surface.
        * \param instance A \ref VkInstance handle to the parent instance.
        */
        VulkanSurface(const VkSurfaceKHR& surface, const VkInstance& instance);
        ~VulkanSurface() override;

        VulkanSurface(const VulkanSurface& other) = delete;
        VulkanSurface(VulkanSurface&& other) noexcept = delete;
        VulkanSurface& operator=(const VulkanSurface& other) = delete;
        VulkanSurface& operator=(VulkanSurface&& other) noexcept = delete;

        /**
        * \brief Gets the backend handle from which the surface was created.
        * \return A \ref VkInstance handle to the parent instance, from which the surface was created.
        */
        [[nodiscard]] const VkInstance& instance() const noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
