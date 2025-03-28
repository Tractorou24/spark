#pragma once

#include <array>
#include <cmath>
#include <functional>
#include <numeric>

namespace spark::math::details
{
    template <template <typename> typename Impl, typename T, std::size_t N>
    class Vector
    {
    public:
        /// \brief Initializes a new \ref Vector.
        explicit constexpr Vector(std::array<std::reference_wrapper<T>, N> values)
            : m_values(values) {}

        /**
         * \brief Compares two \ref Vector for equality.
         * \param other A \ref Vector to compare with.
         * \return `true` if this is equal to \p other `false` otherwise.
         */
        constexpr bool operator==(const Vector& other) const noexcept
        {
            for (std::size_t i = 0; i < m_values.size(); i++)
                if (m_values[i].get() != other.m_values[i].get())
                    return false;
            return true;
        }

        /**
         * \brief Compares two \ref Vector for inequality.
         * \param other A \ref Vector to compare with.
         * \return `true` if this is not equal to \p other `false` otherwise.
         */
        constexpr bool operator!=(const Vector& other) const noexcept
        {
            return !operator==(other);
        }

        /**
         * \brief Adds two \ref Vector.
         * \param other The \ref Vector to add.
         * \return A \ref Vector resulting of the sum of the two vectors.
         */
        constexpr Impl<T> operator+(const Impl<T>& other) const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = m_values[i] + other.m_values[i];
            return result;
        }

        /**
         * \brief Subtracts two \ref Vector.
         * \param other The \ref Vector to subtract.
         * \return A \ref Vector resulting of the subtraction of the two vectors.
         */
        constexpr Impl<T> operator-(const Impl<T>& other) const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = m_values[i] - other.m_values[i];
            return result;
        }

        /**
         * \brief Multiplies two \ref Vector.
         * \param other The \ref Vector to multiply with.
         * \return A \ref Vector resulting of the multiplication of the two vectors.
         */
        constexpr Impl<T> operator*(const Impl<T>& other) const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = m_values[i] * other.m_values[i];
            return result;
        }

        /**
         * \brief Divides two \ref Vector.
         * \param other The \ref Vector to divide with.
         * \return A \ref Vector resulting of the division of the two vectors.
         */
        constexpr Impl<T> operator/(const Impl<T>& other) const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = m_values[i] / other.m_values[i];
            return result;
        }

        /**
         * \brief Multiplies a \ref Vector by a value of type T
         * \param value The value of type T to multiply the vector with
         * \return A \ref Vector resulting of the multiplication with \p value.
         */
        constexpr Impl<T> operator*(const T& value) const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = m_values[i] * value;
            return result;
        }

        /**
         * \brief Divides a \ref Vector by a value of type T
         * \param value The value of type T to divide the vector with
         * \return A \ref Vector resulting of the division with \p value.
         */
        constexpr Impl<T> operator/(const T& value) const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = m_values[i] / value;
            return result;
        }

        /**
         * \brief Adds the value of \p other to the current vector.
         * \param other The \ref Vector to take values to add.
         * \return A reference to the modified \ref Vector.
         */
        constexpr Impl<T>& operator+=(const Vector& other) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                m_values[i] += other.m_values[i];
            return static_cast<Impl<T>&>(*this);
        }

        /**
         * \brief Subtracts the value of \p other to the current vector.
         * \param other The \ref Vector to take values to subtract.
         * \return A reference to the modified \ref Vector.
         */
        constexpr Impl<T>& operator-=(const Vector& other) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                m_values[i] -= other.m_values[i];
            return static_cast<Impl<T>&>(*this);
        }

        /**
         * \brief Multiplies the value of \p other to the current vector.
         * \param other The \ref Vector to take values to multiply with.
         * \return A reference to the modified \ref Vector.
         */
        constexpr Impl<T>& operator*=(const Vector& other) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                m_values[i] *= other.m_values[i];
            return static_cast<Impl<T>&>(*this);
        }

        /**
         * \brief Divides the value of \p other from the current vector.
         * \param value The value to divide each component with.
         * \return A reference to the modified \ref Vector.
         */
        constexpr Impl<T>& operator/=(const T& value) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
                m_values[i] /= value;
            return static_cast<Impl<T>&>(*this);
        }

        /**
         * \brief Negates all components of the current \ref Vector.
         * \return A \ref Vector with all components negated.
         */
        constexpr Impl<T> operator-() const noexcept
        {
            Impl<T> result;
            for (std::size_t i = 0; i < N; ++i)
                result.m_values[i].get() = -m_values[i];
            return result;
        }

        /**
         * \brief Computes the norm of the \ref Vector. (||v|| = sqrt(v1^2 + v2^2 + ... + vn^2))
         * \return The norm of the \ref Vector.
         */
        [[nodiscard]] constexpr T norm() const noexcept
        {
            static_assert(std::is_floating_point_v<T>, "The norm can only be computed for floating point types.");

            T value {};
            for (std::size_t i = 0; i < N; ++i)
                value += m_values[i].get() * m_values[i].get();
            return std::sqrt(value);
        }

        /**
         * \brief Gets the normalized \ref Vector. (v / ||v||)
         * \return The normalized \ref Vector.
         */
        [[nodiscard]] constexpr Impl<T> normalized() const noexcept
        {
            return *this / norm();
        }

        /**
         * \brief Computes the dot product between the current \ref Vector and \p other.
         * \param other The \ref Vector to compute the dot product with.
         * \return The value of the dot product between this and \p other.
         */
        [[nodiscard]] constexpr T dot(const Impl<T>& other) const noexcept
        {
            T value {};
            for (std::size_t i = 0; i < N; ++i)
                value += m_values[i].get() * other.m_values[i].get();
            return value;
        }

        /**
         * \brief Swaps two \ref Vector.
         * \param lhs The first vector.
         * \param rhs The second vector.
         */
        friend constexpr void swap(Vector& lhs, Vector& rhs) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                T temp = lhs.m_values[i];
                lhs.m_values[i].get() = rhs.m_values[i].get();
                rhs.m_values[i].get() = temp;
            }
        }

    private:
        std::array<std::reference_wrapper<T>, N> m_values;
    };
}
