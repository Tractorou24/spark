#pragma once

#include <utility>

namespace spark::math
{
    /**
     * \brief A vector with two components.
     * \tparam T The type of the components.
     */
    template <typename T>
    class Vector2 final
    {
    public:
        constexpr Vector2() = default;

        constexpr Vector2(T x, T y) noexcept
            : x(std::move(x)), y(std::move(y)) {}

        ~Vector2() = default;

        constexpr Vector2(const Vector2& other) = default;
        constexpr Vector2(Vector2&& other) noexcept = default;
        constexpr Vector2& operator=(const Vector2& other) = default;
        constexpr Vector2& operator=(Vector2&& other) noexcept = default;

        // Comparison operators
        constexpr auto operator<=>(const Vector2& other) const noexcept = default;

        // Arithmetic operators
        constexpr Vector2 operator+(const Vector2& other) const noexcept;
        constexpr Vector2 operator-(const Vector2& other) const noexcept;
        constexpr Vector2 operator*(const Vector2& other) const noexcept;
        constexpr Vector2 operator/(const Vector2& other) const noexcept;
        constexpr Vector2 operator*(const T& value) const noexcept;
        constexpr Vector2 operator/(const T& value) const noexcept;

        // Compound assignment operators
        constexpr Vector2& operator+=(const Vector2& other) noexcept;
        constexpr Vector2& operator-=(const Vector2& other) noexcept;
        constexpr Vector2& operator*=(const Vector2& other) noexcept;
        constexpr Vector2& operator/=(const T& value) noexcept;

        // Unary operators
        constexpr Vector2 operator-() const noexcept;
        constexpr Vector2 operator+() const noexcept;

        // Misc
        friend constexpr void swap(Vector2& lhs, Vector2& rhs) noexcept
        {
            std::swap(lhs.x, rhs.x);
            std::swap(lhs.y, rhs.y);
        }

        [[nodiscard]] constexpr T distance(const Vector2& other) const noexcept;
        [[nodiscard]] constexpr T angle(const Vector2& other) const noexcept;

        template<typename To>
        [[nodiscard]] constexpr Vector2<To> castTo() const noexcept;

    public:
        T x, y;
    };
}

#include "spark/math/impl/Vector2.h"
