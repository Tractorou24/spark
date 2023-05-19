#pragma once

#include <utility>

namespace spark::math
{
    /**
     * \brief A vector with three components.
     * \tparam T The type of three components.
     */
    template <typename T>
    class Vector3 final
    {
    public:
        constexpr Vector3() = default;

        constexpr Vector3(T x, T y, T z) noexcept
            : x(std::move(x)), y(std::move(y)), z(std::move(z)) {}

        ~Vector3() = default;

        constexpr Vector3(const Vector3& other) = default;
        constexpr Vector3(Vector3&& other) noexcept = default;
        constexpr Vector3& operator=(const Vector3& other) = default;
        constexpr Vector3& operator=(Vector3&& other) noexcept = default;

        // Comparison operators
        constexpr auto operator<=>(const Vector3& other) const noexcept = default;

        // Arithmetic operators
        constexpr Vector3 operator+(const Vector3& other) const noexcept;
        constexpr Vector3 operator-(const Vector3& other) const noexcept;
        constexpr Vector3 operator*(const Vector3& other) const noexcept;
        constexpr Vector3 operator/(const Vector3& other) const noexcept;
        constexpr Vector3 operator*(const T& value) const noexcept;
        constexpr Vector3 operator/(const T& value) const noexcept;

        // Compound assignment operators
        constexpr Vector3& operator+=(const Vector3& other) noexcept;
        constexpr Vector3& operator-=(const Vector3& other) noexcept;
        constexpr Vector3& operator*=(const Vector3& other) noexcept;
        constexpr Vector3& operator/=(const T& value) noexcept;

        // Unary operators
        constexpr Vector3 operator-() const noexcept;
        constexpr Vector3 operator+() const noexcept;

        // Misc
        friend constexpr void swap(Vector3& lhs, Vector3& rhs) noexcept
        {
            std::swap(lhs.x, rhs.x);
            std::swap(lhs.y, rhs.y);
            std::swap(lhs.z, rhs.z);
        }

    public:
        T x, y, z;
    };
}

#include "spark/math/impl/Vector3.h"
