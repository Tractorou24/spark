#pragma once

#include "spark/math/details/Vector.h"

namespace spark::math
{
    /**
     * \brief A vector with four components.
     * \tparam T The type of the four components.
     */
    template <typename T>
    class Vector4 final : public details::Vector<Vector4, T, 4>
    {
    public:
        /// \brief Initializes a new \ref Vector4 with all components to their default value.
        constexpr Vector4();

        /**
         * \brief Initializes a new \ref Vector4 with components set to the given values.
         * \param value_w The value of the w component.
         * \param value_x The value of the x component.
         * \param value_y The value of the y component.
         * \param value_z The value of the z component.
         */
        constexpr Vector4(T value_w, T value_x, T value_y, T value_z) noexcept;
        ~Vector4() = default;

        constexpr Vector4(const Vector4& other);
        constexpr Vector4(Vector4&& other) noexcept;
        constexpr Vector4& operator=(const Vector4& other);
        constexpr Vector4& operator=(Vector4&& other) noexcept;

        /**
         * \brief Casts all components of the \ref Vector4 to the type To.
         * \tparam To Type of the components of the new \ref Vector4.
         * \return A \ref Vector4 of type To.
         */
        template <typename To>
        [[nodiscard]] constexpr Vector4<To> castTo() const noexcept;

    public:
        T w {};
        T x {};
        T y {};
        T z {};
    };
}

#include "spark/math/impl/Vector4.h"
