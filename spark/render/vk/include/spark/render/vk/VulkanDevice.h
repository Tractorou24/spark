#pragma once

#include "spark/render/DeviceState.h"
#include "spark/render/GraphicsDevice.h"
#include "spark/render/Image.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanFactory.h"
#include "spark/render/vk/VulkanGraphicsAdapter.h"
#include "spark/render/vk/VulkanQueue.h"
#include "spark/render/vk/VulkanRenderPass.h"
#include "spark/render/vk/VulkanSurface.h"
#include "spark/render/vk/VulkanSwapChain.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkDevice)

namespace spark::render::vk
{
    /**
     * \brief Vulkan implementation of \ref IGraphicsDevice.
     */
    class SPARK_RENDER_VK_EXPORT VulkanDevice final : public GraphicsDevice<VulkanFactory, VulkanSurface, VulkanGraphicsAdapter, VulkanSwapChain, VulkanQueue, VulkanRenderPass>,
                                                      public Resource<VkDevice>
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanDevice.
         * \param adapter The adapter to use for drawing.
         * \param surface The surface the device will draw to.
         * \param extensions The required extensions for the device to be initialized with.
         */
        explicit VulkanDevice(const VulkanGraphicsAdapter& adapter, std::unique_ptr<VulkanSurface>&& surface, std::span<std::string> extensions = {});

        /**
         * \brief Initializes a new \ref VulkanDevice.
         * \param adapter The adapter to use for drawing.
         * \param surface The surface the device will draw to.
         * \param format The initial format of the swap chain.
         * \param frame_buffer_size The initial size of the frame buffers.
         * \param frame_buffers The initial number of frame buffers to use.
         * \param extensions The required extensions for the device to be initialized with.
         */
        explicit VulkanDevice(const VulkanGraphicsAdapter& adapter,
                              std::unique_ptr<VulkanSurface>&& surface,
                              Format format,
                              const math::Vector2<unsigned>& frame_buffer_size,
                              unsigned frame_buffers,
                              std::span<std::string> extensions = {});
        ~VulkanDevice() override;

        VulkanDevice(const VulkanDevice& other) = delete;
        VulkanDevice(VulkanDevice&& other) noexcept = delete;
        VulkanDevice& operator=(const VulkanDevice& other) = delete;
        VulkanDevice& operator=(VulkanDevice&& other) noexcept = delete;

        /**
         * \brief Gets the extensions used to initialize the device.
         * \return A \ref std::span containing the extensions used to initialize the device.
         */
        [[nodiscard]] std::span<std::string> enabledExtensions() const noexcept;

        /// \copydoc GraphicsDevice::maximumMultiSamplingLevel()
        [[nodiscard]] MultiSamplingLevel maximumMultiSamplingLevel(Format format) const noexcept override;

        /// \copydoc GraphicsDevice::ticksPerMillisecond()
        [[nodiscard]] double ticksPerMillisecond() const noexcept override;

        /// \copydoc GraphicsDevice::wait()
        void wait() const override;

        /// \copydoc GraphicsDevice::state()
        [[nodiscard]] DeviceState& state() noexcept override;

        /// \copydoc GraphicsDevice::surface()
        [[nodiscard]] const VulkanSurface& surface() const noexcept override;

        /// \copydoc GraphicsDevice::graphicsAdapter()
        [[nodiscard]] const VulkanGraphicsAdapter& graphicsAdapter() const noexcept override;

        /// \copydoc GraphicsDevice::factory()
        [[nodiscard]] const VulkanFactory& factory() const noexcept override;

        /// \copydoc GraphicsDevice::swapChain()
        [[nodiscard]] const VulkanSwapChain& swapChain() const noexcept override;

        /// \copydoc GraphicsDevice::swapChain()
        [[nodiscard]] VulkanSwapChain& swapChain() noexcept override;

        /// \copydoc GraphicsDevice::graphicsQueue()
        [[nodiscard]] const VulkanQueue& graphicsQueue() const noexcept override;

        /// \copydoc GraphicsDevice::transferQueue()
        [[nodiscard]] const VulkanQueue& transferQueue() const noexcept override;

        /// \copydoc GraphicsDevice::bufferQueue()
        [[nodiscard]] const VulkanQueue& bufferQueue() const noexcept override;

        /// \copydoc GraphicsDevice::computeQueue()
        [[nodiscard]] const VulkanQueue& computeQueue() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
