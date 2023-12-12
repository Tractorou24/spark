#pragma once

#include "spark/math/Vector2.h"

namespace spark::math
{
    /**
     * \brief Represents a rectangle in 2D space.
     * \tparam T The type of the rectangle's components.

     * A rectangle is defined by a position (top left) and an extent (width and height).
     */
    template <typename T>
    class Rectangle
    {
    public:
        /// \brif Initializes a \ref Rectangle with position and extent set to {0, 0}.
        constexpr Rectangle() = default;

        /**
         * \brief Initializes a \ref Rectangle with the given position and extent.
         * \param position The position of the rectangle (top left corner coordinates).
         * \param extent The extent of the rectangle (width and height).
         */
        explicit Rectangle(const Vector2<T>& position, const Vector2<T>& extent);

        /**
         * \brief Initializes a \ref Rectangle with the given position and extent.
         * \param x The x coordinate of the rectangle's top left corner.
         * \param y The y coordinate of the rectangle's top left corner.
         * \param width The width of the rectangle.
         * \param height The height of the rectangle.
         */
        explicit Rectangle(T x, T y, T width, T height);

        /**
         * \brief Compares two \ref Rectangle for equality.
         * \param other A \ref Rectangle to compare with.
         * \return `true` if this is equal to \p other `false` otherwise.
         */
        constexpr bool operator==(const Rectangle& other) const noexcept;

        /**
         * \brief Compares two \ref Rectangle for inequality.
         * \param other A \ref Rectangle to compare with.
         * \return `true` if this is not equal to \p other `false` otherwise.
         */
        constexpr bool operator!=(const Rectangle& other) const noexcept;

        /**
         * \brief Casts the rectangle to a rectangle with a different component type.
         * \tparam To The type of the new rectangle's components.
         * \return A \ref Rectangle with the same position and extent as this rectangle, but with components of type \p To.
         */
        template <typename To>
        constexpr Rectangle<To> castTo() const;

    public:
        spark::math::Vector2<T> position = {};
        spark::math::Vector2<T> extent = {};
    };
}

#include "spark/math/impl/Rectangle.h"
