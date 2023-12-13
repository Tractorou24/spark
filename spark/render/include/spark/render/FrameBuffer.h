#pragma once

#include "spark/render/CommandBuffer.h"
#include "spark/render/Export.h"
#include "spark/render/Image.h"

#include "spark/math/Vector2.h"

#include <memory>

namespace spark::render
{
    /**
     * \brief Interface for a frame buffer.
     */
    class SPARK_RENDER_EXPORT IFrameBuffer
    {
    public:
        virtual ~IFrameBuffer() noexcept = default;

        /**
         * \brief Gets the index of the frame buffer within the \ref IRenderPass.
         * \return The index of the frame buffer within the \ref IRenderPass.
         *
         * A render pass stores multiple frame buffers, each with their own index. Calling \ref IRenderPass::frameBuffer() with this index on the frame buffers render
         * pass returns the current frame buffer instance (i.e. the same instance, as the one, the index has been requested from).
         */
        [[nodiscard]] virtual unsigned int bufferIndex() const noexcept = 0;

        /**
         * \brief Gets the current size of the frame buffer.
         * \return A \ref spark::math::Vector2 containing the current size of the frame buffer.
         */
        [[nodiscard]] virtual spark::math::Vector2<unsigned> size() const noexcept = 0;

        /**
         * \brief Gets all command buffers of the frame buffer.
         * \return A \ref std::vector containing a \ref std::shared_ptr to each command buffer of the frame buffer.
         */
        [[nodiscard]] std::vector<std::shared_ptr<const ICommandBuffer>> commandBuffers() const noexcept { return genericCommandBuffers(); }

        /**
         * \brief Gets the command buffer that records draw commands for the frame buffer.
         * \param index The index of the command buffer to get.
         * \return A \ref std::shared_ptr to the command buffer at \p index.
         *
         * \throws spark::base::ArgumentOutOfRangeException if the frame buffer does not have a command buffer at given \p index.
         */
        [[nodiscard]] std::shared_ptr<const ICommandBuffer> commandBuffer(unsigned int index) const { return genericCommandBuffer(index); }

        /**
         * \brief Gets all images storing the output attachments for the render targets of the \ref IRenderPass.
         * \return A \ref std::vector containing each image of the frame buffer.
         */
        [[nodiscard]] std::vector<const IImage*> images() const noexcept { return genericImages(); }

        /**
         * \brief Gets the image storing the output attachment for the render target mapped at given \p location.
         * \param location The location of the render target to get the image for.
         * \return A \ref std::shared_ptr to the image storing the output attachment for the render target mapped at given \p location.
         */
        [[nodiscard]] const IImage& image(unsigned int location) const { return genericImage(location); }

        /**
         * \brief Invalidate and resize the frame buffer with given \p render_area.
         * \param render_area The new size of the frame buffer.
         *
         * A frame buffer resize causes all render target resources (i.e. images) to be re-created. This is done by the implementation itself, except for present targets, which require
         * a view of an image created on a \ref ISwapChain. If the frame buffer has a present target, it calls \ref ISwapChain::images() on the parent devices' swap chain.
         *
         * \warning There should only be one render pass, that contains present targets, otherwise the images are written by different render passes, which may result in undefined behavior.
         */
        virtual void resize(const spark::math::Vector2<unsigned>& render_area) = 0;

    private:
        /// @{
        /// \brief Private method used to allow replacement of the generic methods by custom types.
        [[nodiscard]] virtual std::vector<std::shared_ptr<const ICommandBuffer>> genericCommandBuffers() const noexcept = 0;
        [[nodiscard]] virtual std::shared_ptr<const ICommandBuffer> genericCommandBuffer(unsigned int index) const = 0;
        [[nodiscard]] virtual std::vector<const IImage*> genericImages() const noexcept = 0;
        [[nodiscard]] virtual const IImage& genericImage(unsigned int location) const = 0;
        ///@}
    };
}
