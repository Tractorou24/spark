#pragma once

#include "spark/render/Export.h"

#include "spark/math/Rectangle.h"

#include <memory>

namespace spark::render
{
    /**
     * \brief Interface representing a scissor.
     */
    class SPARK_RENDER_EXPORT IScissor
    {
    public:
        virtual ~IScissor() noexcept = default;

        /**
         * \brief Get the rectangle defining the scissor region.
         * \return A \ref math::Rectangle<float> defining the scissor region.
         */
        [[nodiscard]] virtual math::Rectangle<float> rectangle() const noexcept = 0;

        /**
         * \brief Set the rectangle defining the scissor region.
         * \param rectangle A \ref math::Rectangle<float> defining the scissor region.
         */
        virtual void setRectangle(const math::Rectangle<float>& rectangle) noexcept = 0;
    };

    /**
     * \brief Implementation of \ref IScissor.
     */
    class SPARK_RENDER_EXPORT Scissor final : public IScissor
    {
    public:
        /**
         * \brief Initializes a new scissor. 
         * \param rectangle The rectangle defining the scissor region.
         */
        explicit Scissor(const math::Rectangle<float>& rectangle = {}) noexcept;
        ~Scissor() noexcept override;

        Scissor(const Scissor& other) = delete;
        Scissor(Scissor&& other) noexcept = delete;
        Scissor& operator=(const Scissor& other) = delete;
        Scissor& operator=(Scissor&& other) noexcept = delete;

        /// \copydoc IScissor::rectangle()
        [[nodiscard]] math::Rectangle<float> rectangle() const noexcept override;

        /// \copydoc IScissor::setRectangle()
        void setRectangle(const math::Rectangle<float>& rectangle) noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
