#pragma once

#include "spark/render/FrameBuffer.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanCommandBuffer.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkFramebuffer)
SPARK_FWD_DECLARE_VK_HANDLE(VkSemaphore)

namespace spark::render::vk
{
    class VulkanRenderPass;
    class IVulkanImage;

    /**
     * \brief Vulkan implementation of \ref IFrameBuffer.
     */
    class SPARK_RENDER_VK_EXPORT VulkanFrameBuffer final : public FrameBuffer<VulkanCommandBuffer>, public Resource<VkFramebuffer>
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanFrameBuffer.
         * \param render_pass The parent \ref VulkanRenderPass of the frame buffer.
         * \param buffer_index The index of the frame buffer within the render pass.
         * \param render_area The initial render area size of the frame buffer.
         * \param command_buffers The number of command buffers the frame buffer stores.
         */
        explicit VulkanFrameBuffer(const VulkanRenderPass& render_pass, unsigned buffer_index, const math::Vector2<unsigned>& render_area, unsigned command_buffers = 1);
        ~VulkanFrameBuffer() override;

        VulkanFrameBuffer(const VulkanFrameBuffer& other) = delete;
        VulkanFrameBuffer(VulkanFrameBuffer&& other) noexcept = delete;
        VulkanFrameBuffer& operator=(const VulkanFrameBuffer& other) = delete;
        VulkanFrameBuffer& operator=(VulkanFrameBuffer&& other) noexcept = delete;

        /**
         * \brief Gets a reference of the semaphore, that can be used to signal, that the frame buffer is finished.
         * \return A reference of the semaphore, that can be used to signal, that the frame buffer is finished.
         */
        [[nodiscard]] const VkSemaphore& semaphore() const noexcept;

        /**
         * \brief Gets a reference of the last fence value for the frame buffer.
         * \return A reference of the last fence value for the frame buffer.
         *
         * The frame buffer must only be re-used, if this fence is reached in the graphics queue.
         */
        [[nodiscard]] std::size_t& lastFence() const noexcept;

        /// \copydoc IFrameBuffer::bufferIndex()
        [[nodiscard]] unsigned bufferIndex() const noexcept override;

        /// \copydoc IFrameBuffer::size()
        [[nodiscard]] spark::math::Vector2<unsigned> size() const noexcept override;

        /// \copydoc IFrameBuffer::resize()
        void resize(const spark::math::Vector2<unsigned>& render_area) override;

        /// \copydoc IFrameBuffer::commandBuffers()
        [[nodiscard]] std::vector<std::shared_ptr<const VulkanCommandBuffer>> commandBuffers() const noexcept override;

        /// \copydoc IFrameBuffer::commandBuffer()
        [[nodiscard]] std::shared_ptr<const VulkanCommandBuffer> commandBuffer(unsigned index) const override;

        /// \copydoc IFrameBuffer::images()
        [[nodiscard]] std::vector<const IVulkanImage*> images() const noexcept override;

        /// \copydoc IFrameBuffer::image()
        [[nodiscard]] const IVulkanImage& image(unsigned location) const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
