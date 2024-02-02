#pragma once

namespace spark::math
{
    template <typename T>
    constexpr Vector4<T>::Vector4()
        : details::Vector<Vector4, T, 4>({std::ref(x), std::ref(y), std::ref(z), std::ref(w)}) {}

    template <typename T>
    constexpr Vector4<T>::Vector4(T value_x, T value_y, T value_z, T value_w) noexcept
        : details::Vector<Vector4, T, 4>({std::ref(x), std::ref(y), std::ref(z), std::ref(w)}),
          x(std::move(value_x)), y(std::move(value_y)), z(std::move(value_z)), w(std::move(value_w)) {}

    template <typename T>
    constexpr Vector4<T>::Vector4(const Vector4& other)
        : details::Vector<Vector4, T, 4>({std::ref(x), std::ref(y), std::ref(z), std::ref(w)}), x(other.x), y(other.y),
          z(other.z), w(other.w) {}

    template <typename T>
    constexpr Vector4<T>::Vector4(Vector4&& other) noexcept
        : details::Vector<Vector4, T, 4>({std::ref(x), std::ref(y), std::ref(z), std::ref(w)}), x(std::move(other.x)),
          y(std::move(other.y)), z(std::move(other.z)), w(std::move(other.w)) {}

    template <typename T>
    constexpr Vector4<T>& Vector4<T>::operator=(const Vector4& other)
    {
        if (this == &other)
            return *this;

        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    template <typename T>
    constexpr Vector4<T>& Vector4<T>::operator=(Vector4&& other) noexcept
    {
        if (this == &other)
            return *this;

        x = std::move(other.x);
        y = std::move(other.y);
        z = std::move(other.z);
        w = std::move(other.w);
        return *this;
    }

    template <typename T>
    template <typename To>
    constexpr Vector4<To> Vector4<T>::castTo() const noexcept
    {
        return Vector4<To>(static_cast<To>(x), static_cast<To>(y), static_cast<To>(z), static_cast<To>(w));
    }
}
