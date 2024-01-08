#pragma once

#include "spark/render/CommandQueue.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanCommandBuffer.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkQueue)
SPARK_FWD_DECLARE_VK_HANDLE(VkCommandPool)
SPARK_FWD_DECLARE_VK_HANDLE(VkSemaphore)
using VkPipelineStageFlags = std::uint32_t;

namespace spark::render::vk
{
    class VulkanDevice;
    class VulkanCommandBuffer;

    /**
     * \brief Vulkan implementation of \ref ICommandQueue.
     */
    class SPARK_RENDER_VK_EXPORT VulkanQueue final : public CommandQueue<VulkanCommandBuffer>, public Resource<VkQueue>
    {
    public:
        explicit VulkanQueue(const VulkanDevice& device, QueueType type, QueuePriority priority, unsigned family_id, unsigned queue_id);
        ~VulkanQueue() override;

        VulkanQueue(const VulkanQueue& other) = delete;
        VulkanQueue(VulkanQueue&& other) noexcept = delete;
        VulkanQueue& operator=(const VulkanQueue& other) = delete;
        VulkanQueue& operator=(VulkanQueue&& other) noexcept = delete;

        /**
         * \brief Gets the parent \ref VulkanDevice that provides the queue.
         * \return The parent \ref VulkanDevice that provides the queue.
         */
        [[nodiscard]] const VulkanDevice& device() const noexcept;

        /**
         * \brief Gets a reference to the command pool used to allocate command buffers.
         * \return A reference to the command pool used to allocate command buffers.
         *
         * \note The command pool exists only if the queue is bound to a device.
         */
        [[nodiscard]] const VkCommandPool& commandPool() const noexcept;

        /**
         * \brief Gets the family ID of the queue.
         * \return The family ID of the queue.
         */
        [[nodiscard]] unsigned familyId() const noexcept;

        /**
         * \brief Gets the queue ID of the queue.
         * \return The queue ID.
         */
        [[nodiscard]] unsigned queueId() const noexcept;

        /**
         * \brief Gets the internal semaphore used to synchronize the queue.
         * \return The internal semaphore used to synchronize the queue.
         */
        [[nodiscard]] const VkSemaphore& timelineSemaphore() const noexcept;

        /// \copydoc ICommandQueue::isBound()
        [[nodiscard]] bool isBound() const noexcept override;

        /// \copydoc ICommandQueue::priority()
        [[nodiscard]] QueuePriority priority() const noexcept override;

        /// \copydoc ICommandQueue::type()
        [[nodiscard]] QueueType type() const noexcept override;

        /// \copydoc ICommandQueue::bind()
        void bind() const noexcept override;

        /// \copydoc ICommandQueue::release()
        void release() const noexcept override;

        /// \copydoc ICommandQueue::waitFor()
        void waitFor(std::size_t fence) const noexcept override;

        /// \copydoc ICommandQueue::currentFence()
        [[nodiscard]] std::size_t currentFence() const noexcept override;

        /// \copydoc ICommandQueue::createCommandBuffer()
        [[nodiscard]] std::shared_ptr<VulkanCommandBuffer> createCommandBuffer(bool begin_recording, bool secondary) const noexcept override;

        /**
         * \brief Submits a single command buffer and inserts a fence to wait for it.
         * \param command_buffer The command buffer to submit to the queue.
         * \param wait_for_semaphores The semaphores to wait for on each pipeline stage. There must be a semaphore for each entry in the \p wait_for_stages array.
         * \param wait_for_stages The pipeline stages of the current render pass to wait for before submitting the command buffer.
         * \param signal_semaphores The semaphores to signal, when the command buffer is executed.
         * \return The value of the fence, inserted after the command buffer.
         *
         * By calling this method, the queue takes shared ownership over the \p command_buffer until the fence is passed. The reference will be released
         * during a \ref waitFor(), if the awaited fence is inserted after the associated one.
         *
         * \note If any of the command buffers is currently recording, it will implicitly end the recording.
         */
        [[nodiscard]] std::size_t submit(std::shared_ptr<const VulkanCommandBuffer> command_buffer,
                                         std::span<VkSemaphore> wait_for_semaphores,
                                         std::span<VkPipelineStageFlags> wait_for_stages,
                                         std::span<VkSemaphore> signal_semaphores = {}) const;

        /**
         * \brief Submits a set of command buffers and inserts a fence to wait for them.
         * \param command_buffers The command buffers to submit to the command queue.
         * \param wait_for_semaphores The semaphores to wait for on each pipeline stage. There must be a semaphore for each entry in the \p waitForStages array.
         * \param wait_for_stages The pipeline stages of the current render pass to wait for before submitting the command buffer.
         * \param signal_semaphores The semaphores to signal, when the command buffer is executed.
         * \return The value of the fence, inserted after the command buffer.
         *
         * By calling this method, the queue takes shared ownership over the \p command_buffers until the fence is passed. The reference will be released
         * during a \ref waitFor(), if the awaited fence is inserted after the associated one.
         *
         * \note If any of the command buffers is currently recording, it will implicitly end the recording.
         */
        [[nodiscard]] std::size_t submit(const std::vector<std::shared_ptr<const VulkanCommandBuffer>>& command_buffers,
                                         std::span<VkSemaphore> wait_for_semaphores,
                                         std::span<VkPipelineStageFlags> wait_for_stages,
                                         std::span<VkSemaphore> signal_semaphores = {}) const;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] std::size_t submit(std::shared_ptr<VulkanCommandBuffer> command_buffer) const noexcept override;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] std::size_t submit(std::shared_ptr<const VulkanCommandBuffer> command_buffer) const noexcept override;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] std::size_t submit(const std::vector<std::shared_ptr<const VulkanCommandBuffer>>& command_buffers) const noexcept override;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] std::size_t submit(const std::vector<std::shared_ptr<VulkanCommandBuffer>>& command_buffers) const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
