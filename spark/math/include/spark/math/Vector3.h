#pragma once

#include <utility>

namespace spark::math
{
    /**
     * \brief A vector with three components.
     * \tparam T The type of three components.
     */
    template <typename T>
    class Vector3 final
    {
    public:
        /// \brief Initializes a new \ref Vector3 with all components to their default value.
        constexpr Vector3() = default;

        /**
         * \brief Initializes a new \ref Vector3 with components set to the given values.
         * \param w The value of the w component.
         * \param x The value of the x component.
         * \param y The value of the y component.
         * \param z The value of the z component.
         */
        constexpr Vector3(T x, T y, T z) noexcept
            : x(std::move(x)), y(std::move(y)), z(std::move(z)) {}

        ~Vector3() = default;

        constexpr Vector3(const Vector3& other) = default;
        constexpr Vector3(Vector3&& other) noexcept = default;
        constexpr Vector3& operator=(const Vector3& other) = default;
        constexpr Vector3& operator=(Vector3&& other) noexcept = default;

        /**
         * \brief Compares two \ref Vector3 for equality.
         * \param other A \ref Vector3 to compare with.
         * \return `true` if this is equal to \p other, `false` otherwise.
         */
        constexpr bool operator==(const Vector3& other) const noexcept;

        /**
         * \brief Compares two \ref Vector3 for inequality.
         * \param other A \ref Vector3 to compare with.
         * \return `true` if this is equal to \p other, `false` otherwise.
         */
        constexpr bool operator!=(const Vector3& other) const noexcept;

        /**
         * \brief Adds two \ref Vector3.
         * \param other The \ref Vector3 to add.
         * \return A \ref Vector3 resulting of the sum of the two vectors.
         */
        constexpr Vector3 operator+(const Vector3& other) const noexcept;

        /**
         * \brief Subtracts two \ref Vector3.
         * \param other The \ref Vector3 to subtract.
         * \return A \ref Vector3 resulting of the subtraction of the two vectors.
         */
        constexpr Vector3 operator-(const Vector3& other) const noexcept;

        /**
         * \brief Multiplies two \ref Vector3.
         * \param other The \ref Vector3 to multiply with.
         * \return A \ref Vector3 resulting of the multiplication of the two vectors.
         */
        constexpr Vector3 operator*(const Vector3& other) const noexcept;

        /**
         * \brief Divides two \ref Vector3.
         * \param other The \ref Vector3 to divide with.
         * \return A \ref Vector3 resulting of the division of the two vectors.
         */
        constexpr Vector3 operator/(const Vector3& other) const noexcept;

        /**
         * \brief Multiplies a \ref Vector3 by a value of type T
         * \param value The value of type T to multiply the vector with
         * \return A \ref Vector3 resulting of the multiplication with \p value.
         */
        constexpr Vector3 operator*(const T& value) const noexcept;

        /**
         * \brief Divides a \ref Vector3 by a value of type T
         * \param value The value of type T to divide the vector with
         * \return A \ref Vector3 resulting of the division with \p value.
         */
        constexpr Vector3 operator/(const T& value) const noexcept;

        /**
         * \brief Adds the value of \p other to the current vector.
         * \param other The \ref Vector3 to take values to add.
         * \return A reference to the modified \ref Vector3.
         */
        constexpr Vector3& operator+=(const Vector3& other) noexcept;

        /**
         * \brief Subtracts the value of \p other to the current vector.
         * \param other The \ref Vector3 to take values to subtract.
         * \return A reference to the modified \ref Vector3.
         */
        constexpr Vector3& operator-=(const Vector3& other) noexcept;

        /**
         * \brief Multiplies the value of \p other to the current vector.
         * \param other The \ref Vector3 to take values to multiply with.
         * \return A reference to the modified \ref Vector3.
         */
        constexpr Vector3& operator*=(const Vector3& other) noexcept;

        /**
         * \brief Divides the value of \p other from the current vector.
         * \param value The value to divide each component with.
         * \return A reference to the modified \ref Vector3.
         */
        constexpr Vector3& operator/=(const T& value) noexcept;

        /**
         * \brief Negates all components of the current \ref Vector3.
         * \return A \ref Vector3 with all components negated.
         */
        constexpr Vector3 operator-() const noexcept;

        /**
         * \brief Swaps two \ref Vector3.
         * \param lhs The first vector.
         * \param rhs The second vector.
         */
        friend constexpr void swap(Vector3& lhs, Vector3& rhs) noexcept;

        /**
         * \brief Casts all components of the \ref Vector3 to the type To.
         * \tparam To Type of the components of the new \ref Vector3.
         * \return A \ref Vector3 of type To.
         */
        template <typename To>
        [[nodiscard]] constexpr Vector3<To> castTo() const noexcept;

    public:
        T x, y, z;
    };
}

#include "spark/math/impl/Vector3.h"
