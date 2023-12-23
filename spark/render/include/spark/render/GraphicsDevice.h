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
}
