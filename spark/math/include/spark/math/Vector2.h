#pragma once

namespace spark::math
{
    /**
     * \brief A vector with two components.
     * \tparam T The type of the components.
     */
    template <typename T>
    class Vector2 final
    {
    public:
        /// \brief Initializes a new \ref Vector2 with all components to their default value.
        constexpr Vector2() = default;

        /**
         * \brief Initializes a new \ref Vector2 with components set to the given values.
         * \param x The value of the x component.
         * \param y The value of the y component.
         */
        constexpr Vector2(T x, T y) noexcept
            : x(std::move(x)), y(std::move(y)) {}

        ~Vector2() = default;

        constexpr Vector2(const Vector2& other) = default;
        constexpr Vector2(Vector2&& other) noexcept = default;
        constexpr Vector2& operator=(const Vector2& other) = default;
        constexpr Vector2& operator=(Vector2&& other) noexcept = default;

        /**
         * \brief Compares two \ref Vector2 for equality.
         * \param other A \ref Vector2 to compare with.
         * \return `true` if this is equal to \p other, `false` otherwise.
         */
        constexpr bool operator==(const Vector2& other) const noexcept;

        /**
         * \brief Compares two \ref Vector2 for inequality.
         * \param other A \ref Vector2 to compare with.
         * \return `true` if this is equal to \p other, `false` otherwise.
         */
        constexpr bool operator!=(const Vector2& other) const noexcept;

        /**
         * \brief Adds two \ref Vector2.
         * \param other The \ref Vector2 to add.
         * \return A \ref Vector2 resulting of the sum of the two vectors.
         */
        constexpr Vector2 operator+(const Vector2& other) const noexcept;

        /**
         * \brief Subtracts two \ref Vector2.
         * \param other The \ref Vector2 to subtract.
         * \return A \ref Vector2 resulting of the subtraction of the two vectors.
         */
        constexpr Vector2 operator-(const Vector2& other) const noexcept;

        /**
         * \brief Multiplies two \ref Vector2.
         * \param other The \ref Vector2 to multiply with.
         * \return A \ref Vector2 resulting of the multiplication of the two vectors.
         */
        constexpr Vector2 operator*(const Vector2& other) const noexcept;

        /**
         * \brief Divides two \ref Vector2.
         * \param other The \ref Vector2 to divide with.
         * \return A \ref Vector2 resulting of the division of the two vectors.
         */
        constexpr Vector2 operator/(const Vector2& other) const noexcept;

        /**
         * \brief Multiplies a \ref Vector2 by a value of type T
         * \param value The value of type T to multiply the vector with
         * \return A \ref Vector2 resulting of the multiplication with \p value.
         */
        constexpr Vector2 operator*(const T& value) const noexcept;

        /**
         * \brief Divides a \ref Vector2 by a value of type T
         * \param value The value of type T to divide the vector with
         * \return A \ref Vector2 resulting of the division with \p value.
         */
        constexpr Vector2 operator/(const T& value) const noexcept;

        /**
         * \brief Adds the value of \p other to the current vector.
         * \param other The \ref Vector2 to take values to add.
         * \return A reference to the modified \ref Vector2.
         */
        constexpr Vector2& operator+=(const Vector2& other) noexcept;

        /**
         * \brief Subtracts the value of \p other to the current vector.
         * \param other The \ref Vector2 to take values to subtract.
         * \return A reference to the modified \ref Vector2.
         */
        constexpr Vector2& operator-=(const Vector2& other) noexcept;

        /**
         * \brief Multiplies the value of \p other to the current vector.
         * \param other The \ref Vector2 to take values to multiply with.
         * \return A reference to the modified \ref Vector2.
         */
        constexpr Vector2& operator*=(const Vector2& other) noexcept;

        /**
         * \brief Divides the value of \p other from the current vector.
         * \param value The value to divide each component with.
         * \return A reference to the modified \ref Vector2.
         */
        constexpr Vector2& operator/=(const T& value) noexcept;

        /**
         * \brief Negates all components of the current \ref Vector2.
         * \return A \ref Vector2 with all components negated.
         */
        constexpr Vector2 operator-() const noexcept;

        /**
         * \brief Swaps two \ref Vector2.
         * \param lhs The first vector.
         * \param rhs The second vector.
         */
        friend constexpr void swap(Vector2& lhs, Vector2& rhs) noexcept;

        /**
         * \brief Casts all components of the \ref Vector2 to the type To.
         * \tparam To Type of the components of the new \ref Vector2.
         * \return A \ref Vector2 of type To.
         */
        template <typename To>
        [[nodiscard]] constexpr Vector2<To> castTo() const noexcept;

    public:
        T x {};
        T y {};
    };
}

#include "spark/math/impl/Vector2.h"
