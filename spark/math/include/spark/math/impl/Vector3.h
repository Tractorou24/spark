#pragma once

namespace spark::math
{
    template <typename T>
    constexpr bool Vector3<T>::operator==(const Vector3& other) const noexcept
    {
        return x == other.x && y == other.y && z == other.z;
    }

    template <typename T>
    constexpr bool Vector3<T>::operator!=(const Vector3& other) const noexcept
    {
        return !(this == other);
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator+(const Vector3& other) const noexcept
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator-(const Vector3& other) const noexcept
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator*(const Vector3& other) const noexcept
    {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator/(const Vector3& other) const noexcept
    {
        return Vector3(x / other.x, y / other.y, z / other.z);
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator*(const T& value) const noexcept
    {
        return Vector3(x * value, y * value, z * value);
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator/(const T& value) const noexcept
    {
        return Vector3(x / value, y / value, z / value);
    }

    template <typename T>
    constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template <typename T>
    constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    template <typename T>
    constexpr Vector3<T>& Vector3<T>::operator*=(const Vector3& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template <typename T>
    constexpr Vector3<T>& Vector3<T>::operator/=(const T& value) noexcept
    {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    template <typename T>
    constexpr Vector3<T> Vector3<T>::operator-() const noexcept
    {
        return Vector3(-x, -y, -z);
    }

    template <typename T>
    constexpr void swap(Vector3<T>& lhs, Vector3<T>& rhs) noexcept
    {
        std::swap(lhs.x, rhs.x);
        std::swap(lhs.y, rhs.y);
        std::swap(lhs.z, rhs.z);
    }

    template <typename T>
    template <typename To>
    constexpr Vector3<To> Vector3<T>::castTo() const noexcept
    {
        return { static_cast<To>(x), static_cast<To>(y), static_cast<To>(z) };
    }
}
