#pragma once

#include "spark/math/details/Vector.h"

namespace spark::math
{
    /**
     * \brief A vector with two components.
     * \tparam T The type of the components.
     */
    template <typename T>
    class Vector2 final : public details::Vector<Vector2, T, 2>
    {
    public:
        /// \brief Initializes a new \ref Vector2 with all components to their default value.
        constexpr Vector2();

        /**
         * \brief Initializes a new \ref Vector2 with components set to the given values.
         * \param x_value The value of the x component.
         * \param y_value The value of the y component.
         */
        constexpr Vector2(T x_value, T y_value) noexcept;
        ~Vector2() = default;

        constexpr Vector2(const Vector2& other);
        constexpr Vector2(Vector2&& other) noexcept;
        constexpr Vector2& operator=(const Vector2& other);
        constexpr Vector2& operator=(Vector2&& other) noexcept;

        /**
         * \brief Casts all components of the \ref Vector2 to the type To.
         * \tparam To Type of the components of the new \ref Vector2.
         * \return A \ref Vector2 of type To.
         */
        template <typename To>
        [[nodiscard]] constexpr Vector2<To> castTo() const noexcept;

    public:
        T x {};
        T y {};
    };
}

#include "spark/math/impl/Vector2.h"
