#pragma once

#include <cmath>

namespace spark::math
{
    template <typename T>
    constexpr bool Vector2<T>::operator==(const Vector2& other) const noexcept
    {
        return x == other.x && y == other.y;
    }

    template <typename T>
    constexpr bool Vector2<T>::operator!=(const Vector2& other) const noexcept
    {
        return !(this == other);
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator+(const Vector2& other) const noexcept
    {
        return Vector2(x + other.x, y + other.y);
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator-(const Vector2& other) const noexcept
    {
        return Vector2(x - other.x, y - other.y);
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator*(const Vector2& other) const noexcept
    {
        return Vector2(x * other.x, y * other.y);
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator/(const Vector2& other) const noexcept
    {
        return Vector2(x / other.x, y / other.y);
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator*(const T& value) const noexcept
    {
        return Vector2(x * value, y * value);
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator/(const T& value) const noexcept
    {
        return Vector2(x / value, y / value);
    }

    template <typename T>
    constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template <typename T>
    constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template <typename T>
    constexpr Vector2<T>& Vector2<T>::operator*=(const Vector2& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template <typename T>
    constexpr Vector2<T>& Vector2<T>::operator/=(const T& value) noexcept
    {
        x /= value;
        y /= value;
        return *this;
    }

    template <typename T>
    constexpr Vector2<T> Vector2<T>::operator-() const noexcept
    {
        return Vector2(-x, -y);
    }

    template <typename T>
    constexpr void swap(Vector2<T>& lhs, Vector2<T>& rhs) noexcept
    {
        std::swap(lhs.x, rhs.x);
        std::swap(lhs.y, rhs.y);
    }

    template <typename T>   
    template<typename To>
    constexpr Vector2<To> Vector2<T>::castTo() const noexcept
    {
        return {static_cast<To>(x), static_cast<To>(y)};
    }
}
