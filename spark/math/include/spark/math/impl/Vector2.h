#pragma once

namespace spark::math
{
    template <typename T>
    constexpr Vector2<T>::Vector2()
        : details::Vector<Vector2, T, 2>({std::ref(x), std::ref(y)}) {}

    template <typename T>
    constexpr Vector2<T>::Vector2(T x_value, T y_value) noexcept
        : details::Vector<Vector2, T, 2>({std::ref(x), std::ref(y)}), x(x_value), y(y_value) {}

    template <typename T>
    constexpr Vector2<T>::Vector2(const Vector2& other)
        : details::Vector<Vector2, T, 2>({std::ref(x), std::ref(y)}), x(other.x), y(other.y) { }

    template <typename T>
    constexpr Vector2<T>::Vector2(Vector2&& other) noexcept
        : details::Vector<Vector2, T, 2>({std::ref(x), std::ref(y)}), x(std::move(other.x)), y(std::move(other.y)) { }

    template <typename T>
    constexpr Vector2<T>& Vector2<T>::operator=(const Vector2& other)
    {
        if (this == &other)
            return *this;

        x = other.x;
        y = other.y;
        return *this;
    }

    template <typename T>
    constexpr Vector2<T>& Vector2<T>::operator=(Vector2&& other) noexcept
    {
        if (this == &other)
            return *this;

        x = std::move(other.x);
        y = std::move(other.y);
        return *this;
    }

    template <typename T>
    template <typename To>
    constexpr Vector2<To> Vector2<T>::castTo() const noexcept
    {
        return Vector2<To>(static_cast<To>(x), static_cast<To>(y));
    }
}
