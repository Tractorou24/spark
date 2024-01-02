#pragma once

#include "spark/render/CommandQueue.h"
#include "spark/render/DeviceState.h"
#include "spark/render/Export.h"
#include "spark/render/GraphicsAdapter.h"
#include "spark/render/GraphicsFactory.h"
#include "spark/render/Surface.h"
#include "spark/render/SwapChain.h"
#include "spark/render/Format.h"

namespace spark::render
{
    /**
     * \brief Interface for a graphics device.
     */
    class SPARK_RENDER_EXPORT IGraphicsDevice
    {
    public:
        virtual ~IGraphicsDevice() noexcept = default;

        /**
         * \brief Gets the device state used to manage the device resources.
         * \return A reference to the \ref DeviceState.
         */
        [[nodiscard]] virtual DeviceState& state() noexcept = 0;

        /**
         * \brief Gets the surface the device is rendering to.
         * \return The \ref ISurface the device is rendering to.
         */
        [[nodiscard]] virtual const ISurface& surface() const noexcept = 0;

        /**
         * \brief Gets the graphics factory used to create buffers, textures, samplers from the device.
         * \return The \ref IGraphicsFactory instance for the device.
         */
        [[nodiscard]] virtual const IGraphicsFactory& factory() const noexcept = 0;

        /**
         * \brief Gets the graphics adapter the device uses for drawing.
         * \return The \ref IGraphicsAdapter the device uses for drawing.
         */
        [[nodiscard]] virtual const IGraphicsAdapter& graphicsAdapter() const noexcept = 0;

        /**
         * \brief Gets the graphics adapter that contains the front and back buffers used for presentation.
         * \return The \ref ISwapChain the device uses for presentation.
         */
        [[nodiscard]] virtual const ISwapChain& swapChain() const noexcept = 0;

        /**
         * \brief Gets the graphics adapter that contains the front and back buffers used for presentation.
         * \return A reference to the \ref ISwapChain the device uses for presentation.
         */
        [[nodiscard]] virtual ISwapChain& swapChain() noexcept = 0;

        /**
         * \brief Gets the queue instance used to process draw calls.
         * \return The \ref ICommandQueue used to process draw calls.
         */
        [[nodiscard]] virtual const ICommandQueue& graphicsQueue() const noexcept = 0;

        /**
         * \brief Gets the queue instance used for device-device transfers (e.g. between render-passes).
         * \return The \ref ICommandQueue used for device-device transfers.
         * \note This can be the same as the \ref graphicsQueue if the device does not support dedicated transfer queues.
         */
        [[nodiscard]] virtual const ICommandQueue& transferQueue() const noexcept = 0;

        /**
         * \brief Gets the queue instance used for buffer transfers (e.g. between CPU and GPU).
         * \return The \ref ICommandQueue used for buffer transfers.
         * \note This can be the same as the \ref graphicsQueue if the device does not support dedicated transfer queues.
         */
        [[nodiscard]] virtual const ICommandQueue& bufferQueue() const noexcept = 0;

        /**
         * \brief Gets the queue instance used for compute operations.
         * \return The \ref ICommandQueue used for compute operations.
         * \note This can be the same as the \ref graphicsQueue if the device does not support dedicated compute queues.
         */
        [[nodiscard]] virtual const ICommandQueue& computeQueue() const noexcept = 0;

        /**
         * \brief Gets the maximum number of multi-sampling levels supported for the given format.
         * \param format The \ref Format of the target (i.e. back buffer).
         * \return The maximum \ref MultiSamplingLevel supported for the given format.
         */
        [[nodiscard]] virtual MultiSamplingLevel maximumMultiSamplingLevel(Format format) const noexcept = 0;

        /**
         * \brief Gets the number of GPU ticks per millisecond.
         * \return The number of GPU ticks per millisecond.
         */
        [[nodiscard]] virtual double ticksPerMillisecond() const noexcept = 0;

        /**
         * \brief Waits until the device is idle.
         *
         * The complexity of this operation may depend on the graphics API that implements this method.
         * Calling this method guarantees, that the device resources are in an unused state and may safely be released.
         */
        virtual void wait() const = 0;
    };

    /**
     * \brief Represents the graphics device that a rendering back-end is doing work on.
     * \tparam SurfaceType The type of the surface the device is rendering to. (Implements \ref ISurface)
     * \tparam GraphicsAdapterType The type of the graphics adapter the device uses for drawing. (Implements \ref IGraphicsAdapter)
     * \tparam SwapChainType The type of the swap chain the device uses for presentation. (Implements \ref ISwapChain)
     * \tparam CommandQueueType The type of the command queue the device uses for draw calls. (Implements \ref ICommandQueue)
     * \tparam RenderPassType The type of the render pass the device uses for draw calls. (Implements \ref IRenderPass)
     *
     * The graphics device is the central instance of a renderer.
     * It owns the device state, which contains objects required for communication between your application and the graphics driver. Most notably, those objects
     * contain the \ref ISwapChain instance and the \ref ICommandQueue instances used for data and command transfer.
     */
    template <typename FactoryType, typename SurfaceType, typename GraphicsAdapterType, typename SwapChainType, typename CommandQueueType, typename RenderPassType>
    class GraphicsDevice : public IGraphicsDevice
    {
    public:
        using surface_type = SurfaceType;
        using adapter_type = GraphicsAdapterType;
        using swap_chain_type = SwapChainType;
        using command_queue_type = CommandQueueType;
        using command_buffer_type = typename command_queue_type::command_buffer_type;
        using factory_type = FactoryType;
        using descriptor_layout_type = typename factory_type::descriptor_layout_type;
        using vertex_buffer_type = typename factory_type::vertex_buffer_type;
        using index_buffer_type = typename factory_type::index_buffer_type;
        using buffer_type = typename factory_type::buffer_type;
        using image_type = typename factory_type::image_type;
        using sampler_type = typename factory_type::sampler_type;
        using render_pass_type = RenderPassType;
        using frame_buffer_type = typename render_pass_type::frame_buffer_type;
        using render_pipeline_type = typename render_pass_type::render_pipeline_type;
        using pipeline_layout_type = typename render_pipeline_type::pipeline_layout_type;
        using shader_program_type = typename render_pipeline_type::shader_program_type;
        using input_assembler_type = typename render_pipeline_type::input_assembler_type;
        using rasterizer_type = typename render_pipeline_type::rasterizer_type;

    public:
        /// \copydoc IGraphicsDevice::state()
        [[nodiscard]] DeviceState& state() noexcept override = 0;

        /// \copydoc IGraphicsDevice::surface()
        [[nodiscard]] const surface_type& surface() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::graphicsAdapter()
        [[nodiscard]] const adapter_type& graphicsAdapter() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::swapChain()
        [[nodiscard]] const swap_chain_type& swapChain() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::swapChain()
        [[nodiscard]] swap_chain_type& swapChain() noexcept override = 0;

        /// \copydoc IGraphicsDevice::factory()
        [[nodiscard]] const factory_type& factory() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::graphicsQueue()
        [[nodiscard]] const command_queue_type& graphicsQueue() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::transferQueue()
        [[nodiscard]] const command_queue_type& transferQueue() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::bufferQueue()
        [[nodiscard]] const command_queue_type& bufferQueue() const noexcept override = 0;

        /// \copydoc IGraphicsDevice::computeQueue()
        [[nodiscard]] const command_queue_type& computeQueue() const noexcept override = 0;
    };
}
