#include "spark/render/vk/VulkanQueue.h"
#include "spark/render/vk/VulkanCommandBuffer.h"
#include "spark/render/vk/VulkanDevice.h"

#include "spark/base/Exception.h"

#include "vulkan/vulkan.h"

#include <mutex>

namespace spark::render::vk
{
    struct VulkanQueue::Impl
    {
        friend class VulkanQueue;

    public:
        explicit Impl(VulkanQueue* parent, const VulkanDevice& device, const QueueType type, const QueuePriority priority, const unsigned family_id, const unsigned queue_id)
            : m_parent(parent), m_device(device), m_type(type), m_priority(priority), m_familyId(family_id), m_queueId(queue_id) {}

        ~Impl() noexcept
        {
            release();
        }

        void bind()
        {
            if (m_isBound)
                return;

            // Store the queue handle if not already done
            if (m_parent->handle() == nullptr)
                vkGetDeviceQueue(m_device.handle(), m_familyId, m_queueId, &m_parent->handle());

            // Create the command pool
            VkCommandPoolCreateInfo command_pool_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = m_familyId
            };

            if (m_type == QueueType::Transfer)
                command_pool_info.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

            if (vkCreateCommandPool(m_device.handle(), &command_pool_info, nullptr, &m_commandPool) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create command pool");

            // Create the timeline semaphore
            const VkSemaphoreTypeCreateInfo timeline_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
                .pNext = nullptr,
                .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
                .initialValue = m_fence
            };

            const VkSemaphoreCreateInfo semaphore_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                .pNext = &timeline_info,
                .flags = 0
            };

            if (vkCreateSemaphore(m_device.handle(), &semaphore_info, nullptr, &m_timelineSemaphore) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create timeline semaphore");

            m_isBound = true;
        }

        void release()
        {
            m_submittedCommandBuffers.clear();

            if (m_timelineSemaphore)
                vkDestroySemaphore(m_device.handle(), m_timelineSemaphore, nullptr);
            if (m_isBound)
                vkDestroyCommandPool(m_device.handle(), m_commandPool, nullptr);

            m_isBound = false;
            m_commandPool = VK_NULL_HANDLE;
            m_timelineSemaphore = VK_NULL_HANDLE;
        }

    private:
        VulkanQueue* m_parent;
        const VulkanDevice& m_device;
        const QueueType m_type;
        const QueuePriority m_priority;

        VkSemaphore m_timelineSemaphore = VK_NULL_HANDLE;
        VkCommandPool m_commandPool = VK_NULL_HANDLE;

        std::vector<std::tuple<std::size_t, std::shared_ptr<const VulkanCommandBuffer>>> m_submittedCommandBuffers;
        std::mutex m_mutex;

        std::size_t m_fence = 0;
        unsigned m_familyId = 0;
        unsigned m_queueId = 0;
        bool m_isBound = false;
    };

    VulkanQueue::VulkanQueue(const VulkanDevice& device, QueueType type, QueuePriority priority, unsigned family_id, unsigned queue_id)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(this, device, type, priority, family_id, queue_id)) {}

    VulkanQueue::~VulkanQueue() = default;

    const VulkanDevice& VulkanQueue::device() const noexcept
    {
        return m_impl->m_device;
    }

    const VkCommandPool& VulkanQueue::commandPool() const noexcept
    {
        return m_impl->m_commandPool;
    }

    unsigned VulkanQueue::familyId() const noexcept
    {
        return m_impl->m_familyId;
    }

    unsigned VulkanQueue::queueId() const noexcept
    {
        return m_impl->m_queueId;
    }

    const VkSemaphore& VulkanQueue::timelineSemaphore() const noexcept
    {
        return m_impl->m_timelineSemaphore;
    }

    bool VulkanQueue::isBound() const noexcept
    {
        return m_impl->m_isBound;
    }

    QueuePriority VulkanQueue::priority() const noexcept
    {
        return m_impl->m_priority;
    }

    QueueType VulkanQueue::type() const noexcept
    {
        return m_impl->m_type;
    }

    void VulkanQueue::bind() const noexcept
    {
        m_impl->bind();
    }

    void VulkanQueue::release() const noexcept
    {
        m_impl->release();
    }

    void VulkanQueue::waitFor(std::size_t fence) const noexcept
    {
        std::size_t completed = 0;
        vkGetSemaphoreCounterValue(m_impl->m_device.handle(), m_impl->m_timelineSemaphore, &completed);

        // Wait for the fence to be signaled
        if (completed < fence)
        {
            const VkSemaphoreWaitInfo wait_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
                .semaphoreCount = 1,
                .pSemaphores = &m_impl->m_timelineSemaphore,
                .pValues = &fence
            };

            vkWaitSemaphores(m_impl->m_device.handle(), &wait_info, std::numeric_limits<std::size_t>::max());
        }

        // Release all shared command buffers
        const auto [from, to] = std::ranges::remove_if(m_impl->m_submittedCommandBuffers,
                                                       [&completed](auto& pair)
                                                       {
                                                           auto& [current_fence, current_command_buffer] = pair;
                                                           if (current_fence > completed)
                                                               return false;

                                                           current_command_buffer->releaseSharedState();
                                                           return true;
                                                       });
        m_impl->m_submittedCommandBuffers.erase(from, to);
    }

    std::size_t VulkanQueue::currentFence() const noexcept
    {
        return m_impl->m_fence;
    }

    std::shared_ptr<VulkanCommandBuffer> VulkanQueue::createCommandBuffer(bool begin_recording, const bool secondary) const noexcept
    {
        return std::make_shared<VulkanCommandBuffer>(*this, begin_recording, !secondary);
    }

    std::size_t VulkanQueue::submit(std::shared_ptr<const VulkanCommandBuffer> command_buffer,
                                    std::span<VkSemaphore> wait_for_semaphores,
                                    std::span<VkPipelineStageFlags> wait_for_stages,
                                    std::span<VkSemaphore> signal_semaphores) const
    {
        if (command_buffer == nullptr)
            throw base::NullPointerException("Command buffer cannot be null");

        if (command_buffer->isSecondary())
            throw base::BadArgumentException("Cannot submit a secondary command buffer.");

        std::scoped_lock lock(m_impl->m_mutex);

        // End the command buffer recording
        command_buffer->end();

        // Create an array of semaphores to signal
        std::vector<VkSemaphore> semaphores_to_signal(signal_semaphores.size());
        std::ranges::generate(semaphores_to_signal, [&signal_semaphores, i = 0]() mutable { return signal_semaphores[i++]; });
        semaphores_to_signal.insert(semaphores_to_signal.begin(), m_impl->m_timelineSemaphore);

        // Submit the command buffer
        const std::size_t fence = ++m_impl->m_fence;
        std::vector<std::size_t> wait_values(wait_for_semaphores.size(), 0);
        std::vector<std::size_t> signal_values(semaphores_to_signal.size(), 0);
        signal_values[0] = fence;

        const VkTimelineSemaphoreSubmitInfo timeline_info = {
            .sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreValueCount = static_cast<unsigned>(wait_values.size()),
            .pWaitSemaphoreValues = wait_values.data(),
            .signalSemaphoreValueCount = static_cast<unsigned>(signal_values.size()),
            .pSignalSemaphoreValues = signal_values.data()
        };

        const VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = &timeline_info,
            .waitSemaphoreCount = static_cast<unsigned>(wait_for_semaphores.size()),
            .pWaitSemaphores = wait_for_semaphores.data(),
            .pWaitDstStageMask = wait_for_stages.data(),
            .commandBufferCount = 1,
            .pCommandBuffers = &command_buffer->handle(),
            .signalSemaphoreCount = static_cast<unsigned>(semaphores_to_signal.size()),
            .pSignalSemaphores = semaphores_to_signal.data()
        };

        if (vkQueueSubmit(this->handle(), 1, &submit_info, nullptr) != VK_SUCCESS)
            throw base::NullPointerException("Failed to submit command buffer");

        // Store the command buffer
        m_impl->m_submittedCommandBuffers.emplace_back(fence, command_buffer);
        return fence;
    }

    std::size_t VulkanQueue::submit(const std::vector<std::shared_ptr<const VulkanCommandBuffer>>& command_buffers,
                                    std::span<VkSemaphore> wait_for_semaphores,
                                    std::span<VkPipelineStageFlags> wait_for_stages,
                                    std::span<VkSemaphore> signal_semaphores) const
    {
        if (std::ranges::any_of(command_buffers, [](auto& buffer) { return buffer == nullptr; }))
            throw base::NullPointerException("Command buffer cannot be null");

        if (std::ranges::any_of(command_buffers, [](auto& buffer) { return buffer->isSecondary(); }))
            throw base::BadArgumentException("Cannot submit a secondary command buffer.");

        std::unique_lock lock(m_impl->m_mutex);

        // End the command buffers recording
        std::vector<VkCommandBuffer> command_buffer_handles(command_buffers.size());
        for (const auto& command_buffer : command_buffers)
        {
            command_buffer->end();
            command_buffer_handles.emplace_back(command_buffer->handle());
        }

        // Create an array of semaphores to signal
        std::vector<VkSemaphore> semaphores_to_signal(signal_semaphores.size() + 1);
        std::ranges::generate(semaphores_to_signal, [&signal_semaphores, i = 0]() mutable { return signal_semaphores[i++]; });
        semaphores_to_signal.insert(semaphores_to_signal.begin(), m_impl->m_timelineSemaphore);

        // Submit the command buffers
        const std::size_t fence = ++m_impl->m_fence;
        std::vector<std::size_t> wait_for_fences(wait_for_semaphores.size(), 0);
        std::vector<std::size_t> signal_values(semaphores_to_signal.size(), 0);
        signal_values.front() = fence;

        const VkTimelineSemaphoreSubmitInfo timeline_info = {
            .sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreValueCount = static_cast<unsigned>(wait_for_stages.size()),
            .pWaitSemaphoreValues = wait_for_fences.data(),
            .signalSemaphoreValueCount = static_cast<unsigned>(signal_values.size()),
            .pSignalSemaphoreValues = signal_values.data()
        };

        const VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = &timeline_info,
            .waitSemaphoreCount = static_cast<unsigned>(wait_for_semaphores.size()),
            .pWaitSemaphores = wait_for_semaphores.data(),
            .pWaitDstStageMask = wait_for_stages.data(),
            .commandBufferCount = static_cast<unsigned>(command_buffer_handles.size()),
            .pCommandBuffers = command_buffer_handles.data(),
            .signalSemaphoreCount = static_cast<unsigned>(semaphores_to_signal.size()),
            .pSignalSemaphores = semaphores_to_signal.data()
        };

        if (vkQueueSubmit(handle(), 1, &submit_info, nullptr) != VK_SUCCESS)
            throw base::NullPointerException("Failed to submit command buffer");

        // Store the command buffer
        for (const auto& buffer : command_buffers)
            m_impl->m_submittedCommandBuffers.emplace_back(fence, buffer);
        return fence;
    }

    std::size_t VulkanQueue::submit(const std::shared_ptr<VulkanCommandBuffer> command_buffer) const noexcept
    {
        return submit(std::static_pointer_cast<const VulkanCommandBuffer>(command_buffer), {}, {}, {});
    }

    std::size_t VulkanQueue::submit(const std::shared_ptr<const VulkanCommandBuffer> command_buffer) const noexcept
    {
        return submit(command_buffer, {}, {}, {});
    }

    std::size_t VulkanQueue::submit(const std::vector<std::shared_ptr<const VulkanCommandBuffer>>& command_buffers) const noexcept
    {
        return submit(command_buffers, {}, {}, {});
    }

    std::size_t VulkanQueue::submit(const std::vector<std::shared_ptr<VulkanCommandBuffer>>& command_buffers) const noexcept
    {
        std::vector<std::shared_ptr<const VulkanCommandBuffer>> buffers;
        buffers.reserve(command_buffers.size());
        std::ranges::transform(command_buffers, std::back_inserter(buffers), [](auto& buffer) { return std::static_pointer_cast<const VulkanCommandBuffer>(buffer); });
        return submit(buffers, {}, {}, {});
    }
}
