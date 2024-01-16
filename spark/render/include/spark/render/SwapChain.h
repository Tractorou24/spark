#pragma once

#include "spark/render/Export.h"
#include "spark/render/FrameBuffer.h"
#include "spark/render/Image.h"
#include "spark/render/Format.h"

#include "spark/math/Vector2.h"

namespace spark::render
{
    /**
     * \brief Interface for a swap chain, a chain of multiple \ref IImage instances that can be presented using a \ref ISurface.
     */
    class SPARK_RENDER_EXPORT ISwapChain
    {
    public:
        virtual ~ISwapChain() noexcept = default;

        /**
         * \brief Gets the swap chain's surface format.
         * \return A \ref Format describing the swap chain's surface format.
         */
        [[nodiscard]] virtual Format surfaceFormat() const noexcept = 0;

        /**
         * \brief Gets the number of images in the swap chain.
         * \return The number of images in the swap chain.
         */
        [[nodiscard]] virtual unsigned int buffers() const noexcept = 0;

        /**
         * \brief Gets the swap chain's render area.
         * \return A \ref math::Vector2<unsigned int> describing the swap chain's render area.
         */
        [[nodiscard]] virtual math::Vector2<unsigned int> renderArea() const noexcept = 0;

        /**
         * \brief Gets the swap chain's current image for the given back buffer.
         * \param back_buffer Index of the back buffer to get the image for.
         * \return A \ref IImage pointer to the swap chain's current image for the given back buffer.
         *
         * \throws base::ArgumentOutOfRangeException If \p back_buffer is out of range.
         */
        [[nodiscard]] virtual const IImage* image(unsigned int back_buffer) const = 0;

        /**
         * \brief Gets all the swap chain's images.
         * \return A \ref std::vector containing pointers to all the swap chain's images.
         */
        [[nodiscard]] std::vector<const IImage*> images() const noexcept { return genericImages(); }

        /**
         * \brief Queues a present that gets executed after \refitem frame_buffer signals that it's done rendering.
         * \param frame_buffer The frame buffer for which the present should wait.
         */
        void present(const IFrameBuffer& frame_buffer) const noexcept { genericPresent(frame_buffer); }

        /**
         * \brief Gets all the swap chain's supported surface formats.
         * \return A \ref std::vector containing all the swap chain's supported surface formats.
         */
        [[nodiscard]] virtual std::vector<Format> surfaceFormats() const noexcept = 0;

        /**
         * \brief Recreates the swap chain with the given parameters. All frame buffers that use this swap chain will be invalidated and recreated.
         *
         * There is no guarantee that:
         *     - the swap chain images will end up in the exact format, as specified by \p surface_format. If the format itself is not
         *       supported, a compatible format may be looked up. If the lookup fails, the method may raise an exception.
         *     - that the number of images returned by \ref ISwapChain::images() matches the number specified in \p buffers. A swap chain may 
         *       require a minimum number of images or may constraint a maximum number of images. In both cases, \p buffers will be clamped.
         *
         * \param surface_format A \ref Format describing the swap chain's surface format.
         * \param render_area A \ref math::Vector2<unsigned int> describing the swap chain's frame buffers size.
         * \param buffers The number of buffers in the swap chain.
         */
        virtual void reset(Format surface_format, math::Vector2<unsigned int> render_area, unsigned int buffers) noexcept = 0;

        /**
         * \brief Swaps the front buffer with the next back buffer in order.
         * \return The new front buffer after the swap.
         */
        [[nodiscard]] virtual unsigned int swapBackBuffer() const noexcept = 0;

    private:
        /// @{
        /// \brief Private method used to allow replacement of the generic methods by custom types.
        [[nodiscard]] virtual std::vector<const IImage*> genericImages() const noexcept = 0;
        virtual void genericPresent(const IFrameBuffer& frame_buffer) const noexcept = 0;
        ///@}
    };

    /**
     * \brief Represents a swap chain, i.e. a chain of multiple \ref IImage instances, that can be presented to a \ref ISurface.
     * \tparam ImageType Type of the swap chain's images. (inherits from \ref IImage)
     * \tparam FrameBufferType Type of the frame buffer. (inherits from \ref IFrameBuffer)
     */
    template <typename ImageType, typename FrameBufferType>
    class SwapChain : public ISwapChain
    {
    public:
        using image_type = ImageType;
        using frame_buffer_type = FrameBufferType;

    public:
        /// \copydoc ISwapChain::images()
        [[nodiscard]] virtual std::vector<const image_type*> images() const noexcept = 0;

        /// \copydoc ISwapChain::present()
        virtual void present(const frame_buffer_type& frame_buffer) const noexcept = 0;

    private:
        [[nodiscard]] std::vector<const IImage*> genericImages() const noexcept final
        {
            auto tmp = images();
            std::vector<const IImage*> images_vector;
            images_vector.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(images_vector), [](const auto& image) { return static_cast<const IImage*>(image); });
            return images_vector;
        }

        void genericPresent(const IFrameBuffer& frame_buffer) const noexcept final { present(static_cast<const frame_buffer_type&>(frame_buffer)); }
    };
}
