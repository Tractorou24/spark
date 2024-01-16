#pragma once

#include "spark/render/SwapChain.h"
#include "spark/render/Format.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanFrameBuffer.h"
#include "spark/render/vk/VulkanImage.h"

#include "spark/math/Vector2.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkSemaphore)

namespace spark::render::vk
{
    class VulkanDevice;

    /**
     * \brief Vulkan implementation of \ref ISwapChain.
     */
    class SPARK_RENDER_VK_EXPORT VulkanSwapChain final : public SwapChain<IVulkanImage, VulkanFrameBuffer>
    {
    public:
        /**
         * \brief Initializes a \ref VulkanSwapChain.
         * \param device The device owning the swap chain.
         * \param surface_format The initial surface format. (default: B8G8R8A8_SRGB)
         * \param render_area The initial render area. (default: 1280x720)
         * \param buffers The initial number of buffers. (default: 3)
         */
        explicit VulkanSwapChain(const VulkanDevice& device,
                                 Format surface_format = Format::B8G8R8A8_SRGB,
                                 const math::Vector2<unsigned>& render_area = {1280, 720},
                                 unsigned buffers = 3);
        ~VulkanSwapChain() override;

        VulkanSwapChain(const VulkanSwapChain& other) = delete;
        VulkanSwapChain(VulkanSwapChain&& other) noexcept = delete;
        VulkanSwapChain& operator=(const VulkanSwapChain& other) = delete;
        VulkanSwapChain& operator=(VulkanSwapChain&& other) noexcept = delete;

        /**
         * \brief Gets the current swap semaphore, a command queue can wait on for presenting.
         * \return The current swap semaphore, a command queue can wait on for presenting.
         */
        [[nodiscard]] const VkSemaphore& semaphore() const noexcept;

        /// \copydoc ISwapChain::buffers()
        [[nodiscard]] unsigned buffers() const noexcept override;

        /// \copydoc ISwapChain::renderArea()
        [[nodiscard]] math::Vector2<unsigned> renderArea() const noexcept override;

        /// \copydoc ISwapChain::surfaceFormat()
        [[nodiscard]] Format surfaceFormat() const noexcept override;

        /// \copydoc ISwapChain::surfaceFormats()
        [[nodiscard]] std::vector<Format> surfaceFormats() const noexcept override;

        /// \copydoc ISwapChain::reset()
        void reset(Format surface_format, math::Vector2<unsigned> render_area, unsigned buffers) noexcept override;

        /// \copydoc ISwapChain::swapBackBuffer()
        [[nodiscard]] unsigned swapBackBuffer() const noexcept override;

        /// \copydoc ISwapChain::images()
        [[nodiscard]] std::vector<const IVulkanImage*> images() const noexcept override;

        /// \copydoc ISwapChain::image()
        [[nodiscard]] const IVulkanImage* image(unsigned back_buffer) const override;

        /// \copydoc ISwapChain::present()
        void present(const VulkanFrameBuffer& frame_buffer) const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
