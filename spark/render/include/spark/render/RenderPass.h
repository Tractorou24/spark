#pragma once

#include "spark/render/Export.h"
#include "spark/render/FrameBuffer.h"
#include "spark/render/InputAttachementMapping.h"
#include "spark/render/RenderPipeilne.h"
#include "spark/render/RenderTarget.h"
#include "spark/render/StateResource.h"

#include <span>

namespace spark::render
{
    /**
     * \brief Interface for a render pass.
     */
    class SPARK_RENDER_EXPORT IRenderPass : public virtual IStateResource
    {
    public:
        ~IRenderPass() noexcept override = default;

        /**
         * \brief Begins the render pass.
         * \param buffer The index of the back buffer to render to. Typically, the values returned by \ref ISwapChain::swapBackBuffer() is used.
         *
         * \throws base::RenderPassAlreadyStartedException if the render pass has already been started.
         * \throws base::ArgumentOutOfRangeException if \p buffer is out of range.
         */
        virtual void begin(unsigned int buffer) = 0;

        /**
         * \brief Ends the render pass.
         * \note If the frame buffer has a present render target, this causes the render pass to synchronize with the swap chain and issue a present command.
         *
         * \throws base::RenderPassNotStartedException if the render pass has not been started.
         */
        virtual void end() const = 0;

        /**
         * \brief Gets the active frame buffer from the render pass.
         * \return A \ref IFrameBuffer used as back buffer by the render pass.
         *
         * \throws k2::base::RenderPassNotStartedException if the render pass has not been started.
         */
        [[nodiscard]] const IFrameBuffer& activeFrameBuffer() const { return genericActiveFrameBuffer(); }

        /**
         * \brief Gets all frame buffers owned by the render pass.
         * \return A \ref std::vector of \ref IFrameBuffer owned by the render pass.
         */
        [[nodiscard]] std::vector<const IFrameBuffer*> frameBuffers() const noexcept { return genericFrameBuffers(); }

        /**
         * \brief Resets the frame buffers owned by the render pass.
         * \param new_render_area The size of the render area the frame buffers should be resized to.
         */
        virtual void resizeFrameBuffers(const math::Vector2<unsigned int>& new_render_area) = 0;

        /**
         * \brief Gets all render pipelines owned by the render pass.
         * \return A \ref std::vector of \ref IRenderPipeline owned by the render pass.
         */
        [[nodiscard]] std::vector<const IRenderPipeline*> pipelines() const noexcept { return genericPipelines(); }

        /**
         * \brief Gets the render target at the specified \p location.
         * \param location The location of the render target to get.
         * \return The \ref RenderTarget at the specified \p location.
         */
        [[nodiscard]] virtual const RenderTarget& renderTarget(unsigned int location) const = 0;

        /**
         * \brief Gets all render targets the render pass is rendering to.
         * \return A \ref std::span of \ref RenderTarget the render pass is rendering to.
         */
        [[nodiscard]] virtual std::span<const RenderTarget> renderTargets() const noexcept = 0;

        /**
         * \brief Checks if one of the render targets is used for presentation in a \ref ISwapChain.
         * \return `true` if one of the render targets is used for presentation in a \ref ISwapChain, `false` otherwise.
         */
        [[nodiscard]] virtual bool hasPresentRenderTarget() const noexcept = 0;

        /**
         * \brief Gets the number of samples the render targets are sampled with.
         * \return A \ref MultiSamplingLevel value representing the number of samples the render targets are sampled with.
         */
        [[nodiscard]] virtual MultiSamplingLevel multiSamplingLevel() const noexcept = 0;

        /**
         * \brief Resolves the input attachments mapped to the render pass and updates them on the descriptor set provided with \p descriptor_set.
         * \param descriptor_set The \ref IDescriptorSet to update the input attachments on.
         */
        void updateAttachments(const IDescriptorSet& descriptor_set) const { genericUpdateAttachments(descriptor_set); }

    private:
        [[nodiscard]] virtual const IFrameBuffer& genericActiveFrameBuffer() const = 0;
        [[nodiscard]] virtual std::vector<const IFrameBuffer*> genericFrameBuffers() const noexcept = 0;
        [[nodiscard]] virtual std::vector<const IRenderPipeline*> genericPipelines() const noexcept = 0;
        virtual void genericUpdateAttachments(const IDescriptorSet& descriptor_set) const = 0;
    };
}
