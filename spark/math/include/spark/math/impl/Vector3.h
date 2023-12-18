#pragma once

namespace spark::math
{
    template <typename T>
    constexpr Vector3<T>::Vector3()
        : details::Vector<Vector3, T, 3>({std::ref(x), std::ref(y), std::ref(z)}) {}

    template <typename T>
    constexpr Vector3<T>::Vector3(T x_value, T y_value, T z_value) noexcept
        : details::Vector<Vector3, T, 3>({std::ref(x), std::ref(y), std::ref(z)}), x(std::move(x_value)), y(std::move(y_value)), z(std::move(z_value)) {}

    template <typename T>
    constexpr Vector3<T>::Vector3(const Vector3& other)
        : details::Vector<Vector3, T, 3>({std::ref(x), std::ref(y), std::ref(z)}), x(other.x), y(other.y), z(other.z) {}

    template <typename T>
    constexpr Vector3<T>::Vector3(Vector3&& other) noexcept
        : details::Vector<Vector3, T, 3>({std::ref(x), std::ref(y), std::ref(z)}), x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)) {}

    template <typename T>
    constexpr Vector3<T>& Vector3<T>::operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    template <typename T>
    constexpr Vector3<T>& Vector3<T>::operator=(Vector3&& other) noexcept
    {
        x = std::move(other.x);
        y = std::move(other.y);
        z = std::move(other.z);
        return *this;
    }

    template <typename T>
    template <typename To>
    constexpr Vector3<To> Vector3<T>::castTo() const noexcept
    {
        return {static_cast<To>(x), static_cast<To>(y), static_cast<To>(z)};
    }
}
