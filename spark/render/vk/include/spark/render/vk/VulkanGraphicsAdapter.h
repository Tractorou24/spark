#pragma once

#include "spark/render/GraphicsAdapter.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"

#include <memory>
#include <string>
#include <vector>

SPARK_FWD_DECLARE_VK_HANDLE(VkPhysicalDevice)
struct VkPhysicalDeviceLimits;

namespace spark::render::vk
{
    /**
     * \brief Implementation of \ref IGraphicsAdapter for Vulkan.
     */
    class SPARK_RENDER_VK_EXPORT VulkanGraphicsAdapter final : public IGraphicsAdapter, public Resource<VkPhysicalDevice>
    {
    public:
        /**
         * \brief Initializes a new instance of the \ref VulkanGraphicsAdapter class.
         * \param adapter The Vulkan physical device to initialize the adapter with.
         */
        explicit VulkanGraphicsAdapter(VkPhysicalDevice adapter);
        ~VulkanGraphicsAdapter() override;

        VulkanGraphicsAdapter(const VulkanGraphicsAdapter& other) = delete;
        VulkanGraphicsAdapter(VulkanGraphicsAdapter&& other) noexcept = delete;
        VulkanGraphicsAdapter& operator=(const VulkanGraphicsAdapter& other) = delete;
        VulkanGraphicsAdapter& operator=(VulkanGraphicsAdapter&& other) noexcept = delete;

        /// \copydoc IGraphicsAdapter::name()
        [[nodiscard]] std::string name() const noexcept override;

        /// \copydoc IGraphicsAdapter::uuid()
        [[nodiscard]] lib::Uuid uuid() const noexcept override;

        /// \copydoc IGraphicsAdapter::vendorId()
        [[nodiscard]] unsigned vendorId() const noexcept override;

        /// \copydoc IGraphicsAdapter::deviceId()
        [[nodiscard]] unsigned deviceId() const noexcept override;

        /// \copydoc IGraphicsAdapter::type()
        [[nodiscard]] GraphicsAdapterType type() const noexcept override;

        /// \copydoc IGraphicsAdapter::driverVersion()
        [[nodiscard]] unsigned driverVersion() const noexcept override;

        /// \copydoc IGraphicsAdapter::apiVersion()
        [[nodiscard]] unsigned apiVersion() const noexcept override;

        /// \copydoc IGraphicsAdapter::dedicatedVideoMemory()
        [[nodiscard]] unsigned long long dedicatedVideoMemory() const noexcept override;

        /**
         * \brief Gets the limits of the physical device.
         * \return A \ref VkPhysicalDeviceLimits struct containing the limits of the physical device.
         */
        [[nodiscard]] VkPhysicalDeviceLimits limits() const noexcept;

        /**
         * \brief Gets the features supported by the physical device.
         * \return A \ref std::vector containing the features supported by the physical device as \ref std::string.
         */
        [[nodiscard]] std::vector<std::string> deviceExtensions() const noexcept;

        /**
         * \brief Validates the given device extensions against the physical device.
         * \param extensions A \ref std::vector containing the device extensions to validate.
         * \return A boolean indicating whether the given device extensions are supported by the physical device.
         *
         * \note To return true, all given device extensions must be supported by the physical device.
         */
        [[nodiscard]] bool validateDeviceExtensions(const std::vector<std::string>& extensions) const noexcept;

        /**
         * \brief Gets the validation layers supported by the physical device.
         * \return A \ref std::vector containing the validation layers supported by the physical device as \ref std::string.
         */
        [[nodiscard]] std::vector<std::string> deviceValidationLayers() const noexcept;

        /**
         * \brief Validates the given validation layers against the physical device.
         * \param layers A \ref std::vector containing the validation layers to validate.
         * \return A boolean indicating whether the given validation layers are supported by the physical device.
         *
         * \note To return true, all given validation layers must be supported by the physical device.
         */
        [[nodiscard]] bool validateDeviceValidationLayers(const std::vector<std::string>& layers) const noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
