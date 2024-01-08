#pragma once

#include "spark/render/CommandBuffer.h"
#include "spark/render/Export.h"

#include "spark/base/Exception.h"
#include "spark/lib/String.h"

#include <algorithm>
#include <memory>
#include <utility>

namespace spark::render
{
    /**
    * \brief Represents the type of a \ref ICommandQueue.
    *
    * There are three major queue types: `Graphics`, `Compute` and `Transfer`. Each queue type has a larger subset of commands it is allowed to execute. For example, a 
    * graphics queue can be used to execute transfer or compute commands, whilst a compute queue may not execute graphics commands (such as *draw*). 
    * 
    * You should always aim to use a queue that is dedicated for the workload you want to submit. For example, if you want to upload resources to the GPU, use the a
    * dedicated transfer queue and synchronize access to the resource by waiting for the queue to finish at the point of time you need to access the resource. You can,
    * however, also use the graphics queue for transfers. This can be more efficient, if you have resources that require to be updated with each frame. The performance 
    * impact of synchronizing two queues may be larger than simply using the graphics queue to begin with.
    * 
    * The advantage of using dedicated queues is, that they do not necessarily block execution. For example, when performing a compute or transfer workload on a 
    * graphics queue, you do not need to synchronize in order to wait for the result, however this also means that no rendering can take place until the workloads have
    * finished.
    */
    enum class QueueType
    {
        /// \brief Describes an unspecified command queue. It is not valid to create a queue instance with this type.
        None = 0x00000000,

        /// \brief Represents a queue that can execute graphics, compute and transfer workloads.
        Graphics = 0x00000001,

        /// \brief Represents a queue that can execute compute and transfer workloads.
        Compute = 0x00000002,

        /// \brief Represents a queue that can execute only transfer workloads.
        Transfer = 0x00000004,

        /// \brief Represents an invalid queue type.
        Other = 0x7FFFFFFF
    };

    SPARK_DEFINE_ENUM_FLAGS(QueueType);

    /**
     * \brief Specifies the priority with which a queue is scheduled on the GPU.
     */
    enum class QueuePriority
    {
        /// \brief The default queue priority.
        Normal = 33,

        /// \brief A high queue priority.
        High = 66,

        /// \brief The highest possible queue priority. Submitting work to this queue might block other queues.
        Realtime = 100
    };

    /**
     * \brief Interface for a command queue.
     */
    class SPARK_RENDER_EXPORT ICommandQueue
    {
    public:
        virtual ~ICommandQueue() noexcept = default;

        /**
         * \brief Checks whether the queue is currently bound to the parent device.
         * \return `true` if the queue is bound, `false` otherwise.
         */
        [[nodiscard]] virtual bool isBound() const noexcept = 0;

        /**
         * \brief Gets the priority of the queue.
         * \return A \ref QueuePriority value.
         */
        [[nodiscard]] virtual QueuePriority priority() const noexcept = 0;

        /**
         * \brief Gets the type of the queue.
         * \return A \ref QueueType value.
         */
        [[nodiscard]] virtual QueueType type() const noexcept = 0;

        /**
         * \brief Binds the queue to the parent device.
         */
        virtual void bind() const noexcept = 0;

        /**
         * \brief Releases the queue from the parent device.
         */
        virtual void release() const noexcept = 0;

        /**
         * \brief Creates a command buffer that can be used to allocate commands on the queue.
         * \param begin_recording `true` if the command buffer should be in recording state after creation, `false` otherwise.
         * \param secondary `true` if the command buffer should be a secondary command buffer, `false` otherwise.
         * \return A \ref std::shared_ptr to the created \ref ICommandBuffer.
         *
         * Specifying \p secondary allows to create secondary command buffers (aka. bundles). Those are intended to be used as efficient pre-recorded command buffers
         * that are re-used multiple times. Using such a command buffer allows drivers to pre-apply optimizations, which causes a one-time cost during setup, but reduces cost when re-
         * applying the command buffer multiple times. Ideally they are used as small chunks of re-occurring workloads.
         * 
         * A secondary command buffer must not be submitted to a queue, but rather to a primary command buffer by calling \ref ICommandBuffer::execute().
         */
        [[nodiscard]] std::shared_ptr<ICommandBuffer> createCommandBuffer(bool begin_recording = false, bool secondary = false) const noexcept
        {
            return genericCreateCommandBuffer(begin_recording, secondary);
        }

        /**
         * \brief Submits a single command buffer to the queue and inserts a fence to wait for it.
         * \param command_buffer The command buffer to submit to the queue.
         * \return The fence that was inserted to wait for the command buffer.
         *
         * \note Submitting a recording command buffer will implicitly end the recording.
         */
        [[nodiscard]] std::size_t submit(std::shared_ptr<ICommandBuffer> command_buffer) const noexcept { return genericSubmit(std::move(command_buffer)); }

        /**
         * \brief Submits a single command buffer to the queue and inserts a fence to wait for it.
         * \param command_buffer The command buffer to submit to the queue.
         * \return The fence that was inserted to wait for the command buffer.
         *
         * \note Submitting a recording command buffer will implicitly end the recording.
         */
        [[nodiscard]] std::size_t submit(std::shared_ptr<const ICommandBuffer> command_buffer) const noexcept { return genericSubmit(std::move(command_buffer)); }

        /**
         * \brief Submits multiple command buffers to the queue and inserts a fence to wait for them.
         * \param command_buffers A \ref std::vector of command buffers to submit to the queue.
         * \return The fence that was inserted to wait for the command buffers.
         *
         * \note If any of the command buffers is currently recording, it will implicitly end the recording.
         */
        [[nodiscard]] std::size_t submit(const std::vector<std::shared_ptr<const ICommandBuffer>>& command_buffers) const noexcept { return genericSubmit(command_buffers); }

        /**
         * \brief Submits multiple command buffers to the queue and inserts a fence to wait for them.
         * \param command_buffers A \ref std::vector of command buffers to submit to the queue.
         * \return The fence that was inserted to wait for the command buffers.
         *
         * \note If any of the command buffers is currently recording, it will implicitly end the recording.
         */
        [[nodiscard]] std::size_t submit(const std::vector<std::shared_ptr<ICommandBuffer>>& command_buffers) const noexcept
        {
            std::vector<std::shared_ptr<const ICommandBuffer>> command_buffers_vector;
            command_buffers_vector.reserve(command_buffers.size());
            std::ranges::transform(command_buffers,
                                   std::back_inserter(command_buffers_vector),
                                   [](auto& command_buffer) { return std::static_pointer_cast<const ICommandBuffer>(command_buffer); });
            return genericSubmit(command_buffers_vector);
        }

        /**
         * \brief Waits for fence value \p fence to complete on the command queue.
         * \param fence The fence value to wait for.
         *
         * Each time one or more command buffers are submitted to the queue, a fence is inserted and its value will be returned. By calling this method, it is possible to
         * wait for this fence. A fence value is guaranteed to be larger than earlier fences, so the method returns, if the latest signaled fence value is larger or equal
         * to the value specified in \p fence.
         */
        virtual void waitFor(std::size_t fence) const noexcept = 0;

        /**
         * \brief Gets the latest fence inserted into the queue.
         * \return The latest fence value.
         */
        [[nodiscard]] virtual std::size_t currentFence() const noexcept = 0;

    private:
        /// @{
        /// \brief Private method used to allow replacement of the generic methods by custom types.
        [[nodiscard]] virtual std::shared_ptr<ICommandBuffer> genericCreateCommandBuffer(bool begin_recording, bool secondary) const noexcept = 0;
        virtual std::size_t genericSubmit(std::shared_ptr<ICommandBuffer> command_buffer) const noexcept = 0;
        virtual std::size_t genericSubmit(std::shared_ptr<const ICommandBuffer> command_buffer) const noexcept = 0;
        virtual std::size_t genericSubmit(const std::vector<std::shared_ptr<const ICommandBuffer>>& command_buffers) const noexcept = 0;
        /// @}
    };

    /**
     * \brief Represents a \ref ICommandQueue. 
     * \tparam CommandBufferType Type of the command buffer that is used by the queue. (inherits from \ref ICommandBuffer)
     */
    template <typename CommandBufferType>
    class CommandQueue : public ICommandQueue
    {
    public:
        using command_buffer_type = CommandBufferType;

    public:
        /// \copydoc ICommandQueue::createCommandBuffer()
        [[nodiscard]] virtual std::shared_ptr<command_buffer_type> createCommandBuffer(bool begin_recording = false, bool secondary = false) const noexcept = 0;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] virtual std::size_t submit(std::shared_ptr<command_buffer_type> command_buffer) const noexcept = 0;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] virtual std::size_t submit(std::shared_ptr<const command_buffer_type> command_buffer) const noexcept = 0;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] virtual std::size_t submit(const std::vector<std::shared_ptr<const command_buffer_type>>& command_buffers) const noexcept = 0;

        /// \copydoc ICommandQueue::submit()
        [[nodiscard]] virtual std::size_t submit(const std::vector<std::shared_ptr<command_buffer_type>>& command_buffers) const noexcept = 0;

    private:
        [[nodiscard]] std::shared_ptr<ICommandBuffer> genericCreateCommandBuffer(bool begin_recording, bool secondary) const noexcept final
        {
            return createCommandBuffer(begin_recording, secondary);
        }

        std::size_t genericSubmit(std::shared_ptr<ICommandBuffer> command_buffer) const noexcept final
        {
            return submit(std::dynamic_pointer_cast<command_buffer_type>(std::move(command_buffer)));
        }

        std::size_t genericSubmit(std::shared_ptr<const ICommandBuffer> command_buffer) const noexcept final
        {
            return submit(std::dynamic_pointer_cast<const command_buffer_type>(std::move(command_buffer)));
        }

        std::size_t genericSubmit(const std::vector<std::shared_ptr<const ICommandBuffer>>& command_buffers) const noexcept final
        {
            std::vector<std::shared_ptr<const command_buffer_type>> command_buffers_vector;
            std::ranges::transform(command_buffers,
                                   std::back_inserter(command_buffers_vector),
                                   [](auto& command_buffer) { return std::dynamic_pointer_cast<const command_buffer_type>(command_buffer); });
            return submit(command_buffers_vector);
        }
    };
}
