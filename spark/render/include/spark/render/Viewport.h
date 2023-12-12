#pragma once

#include "spark/render/Export.h"

#include "spark/math/Rectangle.h"

#include <memory>

namespace spark::render
{
    /**
     * \brief Interface representing a viewport.
     */
    class SPARK_RENDER_EXPORT IViewport
    {
    public:
        virtual ~IViewport() noexcept = default;

        /**
         * \brief Gets the rectangle defining the dimensions of the viewport.
         * \return A \ref math::Rectangle<float> representing the viewport.
         */
        [[nodiscard]] virtual math::Rectangle<float> rectangle() const noexcept = 0;

        /**
         * \brief Sets the rectangle defining the dimensions of the viewport.
         * \param rectangle A \ref math::Rectangle<float> representing the new viewport dimensions.
         */
        virtual void setRectangle(const math::Rectangle<float>& rectangle) noexcept = 0;

        /**
         * \brief Get the minimum depth of the viewport.
         * \return The minimum depth of the viewport.
         */
        [[nodiscard]] virtual float minDepth() const noexcept = 0;

        /**
         * \brief Sets the minimum depth of the viewport.
         * \param min_depth The new minimum depth of the viewport.
         */
        virtual void setMinDepth(float min_depth) noexcept = 0;

        /**
         * \brief Gets the maximum depth of the viewport.
         * \return The maximum depth of the viewport.
         */
        [[nodiscard]] virtual float maxDepth() const noexcept = 0;

        /**
         * \brief Sets the maximum depth of the viewport.
         * \param max_depth The new maximum depth of the viewport.
         */
        virtual void setMaxDepth(float max_depth) noexcept = 0;
    };

    /**
     * \brief Implementation of \ref IViewport.
     */
    class SPARK_RENDER_EXPORT Viewport final : public IViewport
    {
    public:
        /**
         * \brief Initializes a new \ref Viewport.
         * \param rectangle The rectangle defining the dimensions of the viewport.
         * \param min_depth The minimum depth of the viewport.
         * \param max_depth The maximum depth of the viewport.
         */
        explicit Viewport(const math::Rectangle<float>& rectangle = {}, float min_depth = 0.0f, float max_depth = 1.0f) noexcept;
        ~Viewport() noexcept override;

        Viewport(const Viewport& other) = delete;
        Viewport(Viewport&& other) noexcept;
        Viewport& operator=(const Viewport& other) = delete;
        Viewport& operator=(Viewport&& other) noexcept;

        /// \copydoc IViewport::rectangle()
        [[nodiscard]] math::Rectangle<float> rectangle() const noexcept override;

        /// \copydoc IViewport::setRectangle()
        void setRectangle(const math::Rectangle<float>& rectangle) noexcept override;

        /// \copydoc IViewport::minDepth()
        [[nodiscard]] float minDepth() const noexcept override;

        /// \copydoc IViewport::setMinDepth()
        void setMinDepth(float min_depth) noexcept override;

        /// \copydoc IViewport::maxDepth()
        [[nodiscard]] float maxDepth() const noexcept override;

        /// \copydoc IViewport::setMaxDepth()
        void setMaxDepth(float max_depth) noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
