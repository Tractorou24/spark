#pragma once

#include "spark/render/Backend.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanGraphicsAdapter.h"
#include "spark/render/vk/VulkanSurface.h"

#include <functional>
#include <span>

SPARK_FWD_DECLARE_VK_HANDLE(VkInstance)

namespace spark::render::vk
{
    /**
     * \brief Vulkan implementation of \ref IRenderBackend.
     */
    class SPARK_RENDER_VK_EXPORT VulkanBackend final : public RenderBackend<VulkanDevice>, public Resource<VkInstance>
    {
    public:
        /**
         * \brief Initializes a \ref VulkanBackend.
         * \param extensions The instance extensions to enable on the backend.
         * \param validation_layers The validation layers to enable on the backend.
         */
        explicit VulkanBackend(std::span<std::string> extensions = {}, std::span<std::string> validation_layers = {});
        ~VulkanBackend() override;

        VulkanBackend(const VulkanBackend& other) = delete;
        VulkanBackend(VulkanBackend&& other) noexcept = delete;
        VulkanBackend& operator=(const VulkanBackend& other) = delete;
        VulkanBackend& operator=(VulkanBackend&& other) noexcept = delete;

        /**
         * \brief Creates a surface using the \p predicate callback.
         * \param predicate A callback that gets called with the backend instance handle and creates the surface instance.
         * \return The instance of the created surface.
         */
        [[nodiscard]] std::unique_ptr<VulkanSurface> createSurface(const std::function<VkSurfaceKHR(const VkInstance&)>& predicate) const;

        /**
         * \brief Checks if all elements of \p validation_layers are contained by the a list of available validation layers.
         * \param validation_layers The validation layers to check.
         * \return `true`, if all elements of \p validation_layers are contained by the a list of available validation layers.
         */
        static bool ValidateInstanceLayers(std::span<std::string> validation_layers) noexcept;

        /**
         * \brief Gets all available validation layers.
         * \return A list of available validation layers.
         */
        static std::vector<std::string> InstanceValidationLayers() noexcept;

        /**
         * \brief Gets the list of enabled validation layers.
         * \return A \ref std::span of enabled validation layers.
         */
        std::span<std::string> enabledValidationLayers() const noexcept;

        /**
         * \brief Checks if all elements of \p extensions are contained by the a list of available extensions.
         * \param extensions The extensions to check.
         * \return `true`, if all elements of \p extensions are contained by the a list of available extensions.
         */
        static bool ValidateInstanceExtensions(std::span<std::string> extensions) noexcept;

        /**
         * \brief Gets all available instance extensions.
         * \return A list of available extensions.
         */
        static std::vector<std::string> AvailableInstanceExtensions() noexcept;

        /// \copydoc IRenderBackend::adapters()
        [[nodiscard]] std::vector<const VulkanGraphicsAdapter*> adapters() const noexcept override;

        /// \copydoc IRenderBackend::findAdapter()
        [[nodiscard]] const VulkanGraphicsAdapter* findAdapter(const std::optional<lib::Uuid>& id) const override;

        /// \copydoc IRenderBackend::device()
        [[nodiscard]] VulkanDevice* device(const std::string& name) noexcept override;

        /// \copydoc IRenderBackend::device()
        [[nodiscard]] const VulkanDevice* device(const std::string& name) const noexcept override;

        /// \copydoc RenderBackend::registerDevice()
        void registerDevice(std::string name, std::unique_ptr<VulkanDevice>&& device) override;

        /// \copydoc RenderBackend::releaseDevice()
        void releaseDevice(const std::string& name) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
