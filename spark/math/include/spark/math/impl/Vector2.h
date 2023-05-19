#pragma once

#include <cmath>

namespace spark::math
{
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
    constexpr Vector2<T> Vector2<T>::operator+() const noexcept
    {
        return *this;
    }

    template <typename T>
    constexpr T Vector2<T>::distance(const Vector2& other) const noexcept
    {
        return static_cast<T>(std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2)));
    }

    template <typename T>
    constexpr T Vector2<T>::angle(const Vector2& other) const noexcept
    {
        return std::atan2(other.y - y, other.x - x);
    }

    template <typename T>   
    template<typename To>
    constexpr Vector2<To> Vector2<T>::castTo() const noexcept
    {
        return {static_cast<To>(x), static_cast<To>(y)};
    }
}
