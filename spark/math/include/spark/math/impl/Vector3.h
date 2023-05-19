#pragma once

namespace spark::math
{
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
    constexpr Vector3<T> Vector3<T>::operator+() const noexcept
    {
        return *this;
    }
}
