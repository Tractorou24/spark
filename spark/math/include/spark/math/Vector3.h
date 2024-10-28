#pragma once

#include "spark/math/details/Vector.h"

namespace spark::math
{
    /**
     * \brief A vector with three components.
     * \tparam T The type of three components.
     */
    template <typename T>
    class Vector3 final : public details::Vector<Vector3, T, 3>
    {
    public:
        /// \brief Initializes a new \ref Vector3 with all components to their default value.
        constexpr Vector3();

        /**
         * \brief Initializes a new \ref Vector3 with components set to the given values.
         * \param x_value The value of the x component.
         * \param y_value The value of the y component.
         * \param z_value The value of the z component.
         */
        constexpr Vector3(T x_value, T y_value, T z_value) noexcept;
        ~Vector3() = default;

        constexpr Vector3(const Vector3& other);
        constexpr Vector3(Vector3&& other) noexcept;
        constexpr Vector3& operator=(const Vector3& other);
        constexpr Vector3& operator=(Vector3&& other) noexcept;

        /**
         * \brief Casts all components of the \ref Vector3 to the type To.
         * \tparam To Type of the components of the new \ref Vector3.
         * \return A \ref Vector3 of type To.
         */
        template <typename To>
        [[nodiscard]] constexpr Vector3<To> castTo() const noexcept;

    public:
        T x {};
        T y {};
        T z {};
    };
}

#include "spark/math/impl/Vector3.h"

template <typename T>
struct std::hash<spark::math::Vector3<T>>
{
    std::size_t operator()(const spark::math::Vector3<T>& vec) const
    {
        return std::hash<T> {}(vec.x) ^ std::hash<T> {}(vec.y) ^ std::hash<T> {}(vec.z);
    }
};
