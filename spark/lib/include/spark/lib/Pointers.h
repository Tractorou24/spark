#pragma once

#include <memory>

namespace spark::lib
{
    /**
     * \brief Transforms a \ref std::unique_ptr to a \ref std::shared_ptr.
     * \tparam T Underlying type of the pointer.
     * \param ptr The \ref std::unique_ptr to convert.
     * \return A \ref std::shared_ptr with the value of old moved \p ptr.
     */
    template <typename T>
    constexpr std::shared_ptr<T> as_shared(std::unique_ptr<T>&& ptr)
    {
        return std::shared_ptr<T>(std::move(ptr));
    }

    /**
     * \brief Creates a new instance of \ref std::unique_ptr whose stored pointer is obtained from \p old stored pointer using a static_cast.
     * \tparam To The underling type to convert to.
     * \tparam From The underlying type of the pointer to convert. (automatically deduced)
     * \param old The pointer to cast.
     * \return The \ref std::unique_ptr of type `To`
     */
    template <typename To, typename From>
    constexpr std::unique_ptr<To> static_unique_pointer_cast(std::unique_ptr<From>&& old)
    {
        return std::unique_ptr<To>(static_cast<To*>(old.release()));
    }

    /**
     * \brief Creates a new instance of \ref std::unique_ptr whose stored pointer is obtained from \p old stored pointer using a reinterpret_cast.
     * \tparam To The underling type to convert to.
     * \tparam From The underlying type of the pointer to convert. (automatically deduced)
     * \param old The pointer to cast.
     * \return The \ref std::unique_ptr of type `To`
     */
    template <typename To, typename From>
    constexpr std::unique_ptr<To> reinterpret_unique_pointer_cast(std::unique_ptr<From>&& old)
    {
        return std::unique_ptr<To>(reinterpret_cast<To*>(old.release()));
    }

    /**
     * \brief Creates a new instance of \ref std::unique_ptr whose stored pointer is obtained from \p old stored pointer using a dynamic_cast.
     * \tparam To The underling type to convert to.
     * \tparam From The underlying type of the pointer to convert. (automatically deduced)
     * \param old The pointer to cast.
     * \return The \ref std::unique_ptr of type `To`
     */
    template <typename To, typename From>
    constexpr std::unique_ptr<To> dynamic_unique_pointer_cast(std::unique_ptr<From>&& old)
    {
        return std::unique_ptr<To>(dynamic_cast<To*>(old.release()));
    }
}
