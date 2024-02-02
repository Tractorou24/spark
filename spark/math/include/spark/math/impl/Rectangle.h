#pragma once

namespace spark::math
{
    template <typename T>
    Rectangle<T>::Rectangle(const Vector2<T>& position, const Vector2<T>& extent)
        : position(position)
        , extent(extent) {}

    template <typename T>
    Rectangle<T>::Rectangle(T x, T y, T width, T height)
        : position(x, y), extent(width, height) {}

    template <typename T>
    constexpr bool Rectangle<T>::operator==(const Rectangle& other) const noexcept
    {
        return position == other.position && extent == other.extent;
    }

    template <typename T>
    constexpr bool Rectangle<T>::operator!=(const Rectangle& other) const noexcept
    {
        return !(*this == other);
    }

    template <typename T>
    template <typename To>
    constexpr Rectangle<To> Rectangle<T>::castTo() const
    {
        return Rectangle<To>(position.template castTo<To>(), extent.template castTo<To>());
    }
}
