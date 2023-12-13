#pragma once

#include "spark/render/Export.h"
#include "spark/render/Format.h"

#include "spark/base/Macros.h"
#include "spark/math/Vector4.h"

#include <string>

namespace spark::render
{
    /**
     * \brief Describes the type of a render target.
     */
    enum class RenderTargetType
    {
        /// \brief Represents a color target.
        Color = 0x00000001,

        /// \brief Represents a depth/stencil target.
        DepthStencil = 0x00000002,

        /// \brief Represents a color target that should be presented.
        /// \note This is similar to <see cref="RenderTargetType::Color" />, but is used to optimize the memory layout of the target for it to be pushed to a swap chain.
        Present = 0x00000004
    };

    /**
     * \brief Specifies a blend factor.
     */
    enum class BlendFactor
    {
        Zero                     = 0,
        One                      = 1,
        SourceColor              = 2,
        OneMinusSourceColor      = 3,
        DestinationColor         = 4,
        OneMinusDestinationColor = 5,
        SourceAlpha              = 6,
        OneMinusSourceAlpha      = 7,
        DestinationAlpha         = 8,
        OneMinusDestinationAlpha = 9,
        ConstantColor            = 10,
        OneMinusConstantColor    = 11,
        ConstantAlpha            = 12,
        OneMinusConstantAlpha    = 13,
        SourceAlphaSaturate      = 14,
        Source1Color             = 15,
        OneMinusSource1Color     = 16,
        Source1Alpha             = 17,
        OneMinusSource1Alpha     = 18
    };

    /**
     * \brief Specifies a write mask for a color buffer.
     */
    enum class WriteMask
    {
        /// Write into the red channel.
        R = 0x01,

        /// Write into the green channel.
        G = 0x02,

        /// Write into the blue channel.
        B = 0x04,

        /// Write into the alpha channel.
        A = 0x08
    };

    SPARK_DEFINE_ENUM_FLAGS(WriteMask)

    /**
     * \brief Specifies a blend operation.
     */
    enum class BlendOperation
    {
        Add             = 0x01,
        Subtract        = 0x02,
        ReverseSubtract = 0x03,
        Minimum         = 0x04,
        Maximum         = 0x05
    };

    /**
     * \brief Represents a render target, which is an abstract view of the output of a \ref RenderPass.
     * A render target represents one output of a render pass, stored within an \ref IImage. It is contained by a \ref RenderPass, that contains 
     * the \ref FrameBuffer, that stores the actual render target image resource.
     */
    class SPARK_RENDER_EXPORT IRenderTarget
    {
    public:
        virtual ~IRenderTarget() noexcept = default;

        /**
         * \brief Describes the blend state of a render target.
         */
        struct BlendState
        {
        public:
            /// \brief Specifies, if the target should be blended (default: false).
            bool enable = false;

            /// \brief The blend factor for the source color channels (default: BlendFactor::One).
            BlendFactor sourceColor {BlendFactor::One};

            /// \brief The blend factor for the destination color channels (default: BlendFactor::Zero).
            BlendFactor destinationColor {BlendFactor::Zero};

            /// \brief The blend factor for the source alpha channel (default: BlendFactor::One).
            BlendFactor sourceAlpha {BlendFactor::One};

            /// \brief The blend factor for the destination alpha channels (default: BlendFactor::Zero).
            BlendFactor destinationAlpha {BlendFactor::Zero};

            /// \brief The blend operation for the color channels (default: BlendOperation::Add).
            BlendOperation colorOperation {BlendOperation::Add};

            /// \brief The blend operation for the alpha channel (default: BlendOperation::Add).
            BlendOperation alphaOperation {BlendOperation::Add};

            /// \brief The channel write mask, determining which channels are written to (default: WriteMask::R | WriteMask::G | WriteMask::B | WriteMask::A).
            WriteMask writeMask {WriteMask::R | WriteMask::G | WriteMask::B | WriteMask::A};
        };

        /**
         * \brief Gets the name of the render target.
         * \return A \ref std::string containing the name of the render target.
         */
        [[nodiscard]] virtual std::string name() const noexcept = 0;

        /**
         * \brief Gets the location of the render target output attachment within the fragment shader.
         * \return The location of the render target output attachment within the fragment shader.
         *
         * The locations of all render targets of a frame buffer must be within a continuous domain, starting at 0. A frame buffer validates the render target locations
         * when it is initialized and will raise an exception, if a location is either not mapped or assigned multiple times.
         */
        [[nodiscard]] virtual unsigned int location() const noexcept = 0;

        /**
         * \brief Gets the type of the render target.
         * \return A \ref RenderTargetType value describing the type of the render target.
         */
        [[nodiscard]] virtual RenderTargetType type() const noexcept = 0;

        /**
         * \brief Gets the internal format of the render target.
         * \return A \ref Format value describing the internal format of the render target.
         */
        [[nodiscard]] virtual Format format() const noexcept = 0;

        /**
         * \brief Checks if the render target should be cleared when the render pass is started.
         * \return `true`, if the render target should be cleared when the render pass is started, `false` otherwise.
         * \note If the \ref format() is a depth format, the depth buffer will be cleared. Otherwise, the color buffer will be cleared.
         */
        virtual bool clearBuffer() const noexcept = 0;

        /**
         * \brief Checks if the render target stencil should be cleared when the render pass is started.
         * \return `true`, if the render target stencil should be cleared when the render pass is started, `false` otherwise.
         * \note If the \ref format() is does not contain a stencil channel, this has no effect.
         */
        virtual bool clearStencil() const noexcept = 0;

        /**
         * \brief Gets the value the render target is cleared with, if \ref clearBuffer() or \ref clearStencil() returns `true`.
         * \return The value the render target is cleared with.
         *
         * If the \ref format() is a color format and \ref clearBuffer() is specified, this contains the clear color. However, if the format is a depth/stencil format,
         * the R and G channels contain the depth and stencil value to clear the buffer with. Note that the stencil buffer is only cleared, if \ref clearStencil() is specified and vice versa.
         */
        [[nodiscard]] virtual math::Vector4<float> clearValues() const noexcept = 0;

        /**
         * \brief Checks if the target should not be persistent for access after the render pass has finished.
         * \return `true`, if the target should not be persistent for access after the render pass has finished, `false` otherwise.
         *
         * A render target can be marked as volatile if it does not need to be accessed after the render pass has finished. This can be used to optimize away unnecessary GPU/CPU 
         * memory round-trips. For example a depth buffer may only be used as an input for the lighting stage of a deferred renderer, but is not required after this. So instead
         * of reading it from the GPU after the lighting pass has finished and then discarding it anyway, it can be marked as volatile in order to prevent it from being read from
         * the GPU memory again in the first place.
         */
        [[nodiscard]] virtual bool isVolatile() const noexcept = 0;

        /**
         * \brief Gets the render target blend state.
         * \return A \ref BlendState value describing the blend state of the render target.
         */
        [[nodiscard]] virtual BlendState blendState() const noexcept = 0;
    };
}
