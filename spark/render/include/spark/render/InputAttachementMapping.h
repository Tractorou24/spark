#pragma once

#include "spark/render/RenderTarget.h"

namespace spark::render
{
    /**
     * \brief Represents the source for an input attachment mapping.
     * \tparam FrameBufferType Type of the frame buffer. (inherits from \ref IFrameBuffer)
     *
     * This interface is implemented by a \ref IRenderPass to return the frame buffer for a given back buffer. It is called by a \ref IFrameBuffer 
     * during initialization or re-creation, in order to resolve input attachment dependencies.
     */
    template <typename FrameBufferType>
    class InputAttachmentMappingSource
    {
    public:
        using FrameBuffer = FrameBufferType;

    public:
        virtual ~InputAttachmentMappingSource() noexcept = default;

        /**
         * \brief Gets the frame buffer with the index provided in \p buffer.
         * \return The \ref IFrameBuffer with the index provided in \p buffer.
         *
         * \throws base::ArgumentOutOfRangeException if \p buffer does not maps to a valid frame buffer.
         */
        [[nodiscard]] virtual const FrameBufferType& frameBuffer(unsigned buffer) const = 0;
    };

    template <typename InputAttachmentMappingSourceType>
    class InputAttachmentMapping
    {
    public:
        using InputAttachmentMappingSource = InputAttachmentMappingSourceType;

    public:
        virtual ~InputAttachmentMapping() noexcept = default;

        /**
         * \brief Gets the source of the input attachment render target
         * \return The \ref InputAttachmentMappingSource of the input attachment render target.
         */
        [[nodiscard]] virtual const InputAttachmentMappingSource* source() const noexcept = 0;

        /**
         * \brief Gets a reference to the render target mapped to this input attachment.
         * \return A \ref RenderTarget mapped to this input attachment.
         */
        [[nodiscard]] virtual const RenderTarget& renderTarget() const noexcept = 0;

        /**
         * \brief Gets the location of the input attachment, the render target will be bound to.
         * \return The location of the input attachment, the render target will be bound to.
         *
         * The locations of all input attachments for a frame buffer must be within a continuous domain, starting at `0`. A frame buffer validates the locations
         * when it is initialized and will raise an exception, if a location is either not mapped or assigned multiple times.
         */
        [[nodiscard]] virtual unsigned location() const noexcept = 0;
    };
}
