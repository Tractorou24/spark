#pragma once

#include "spark/mpl/type_seq.h"

namespace spark::mpl
{
    template <typename... Ts>
    struct typelist;

    // Base struct contains values and methods available in all typelists specializations
    template <typename... Ts>
    struct typelist_base
    {
        /**
         * \brief Number of elements in the typelist
         */
        inline static constexpr std::size_t size = type_seq::size_v<Ts...>;

        /**
         * \brief Is the typelist empty
         */
        inline static constexpr bool empty = type_seq::empty_v<Ts...>;

        /**
         * \brief Is the typelist contains the type T
         * \tparam T Type to check
         */
        template <typename T>
        inline static constexpr bool contains = type_seq::contains_v<T, Ts...>;

        /**
         * \brief Add a type at the front of the typelist
         * \tparam T Type to add
         */
        template <typename T>
        using push_front = type_seq::push_front_t<T, typelist<Ts...>>;

        /**
         * \brief Add a type at the back of the typelist
         * \tparam T Type to add
         */
        template <typename T>
        using push_back = type_seq::push_back_t<T, typelist<Ts...>>;

        /**
         * \brief Removes all types from the typelist
         */
        using clear = typelist<>;
    };

    // Specialization for empty typelist
    template <>
    struct typelist<> : typelist_base<> { };

    // Specialization for typelist with at least one type
    template <typename First, typename... Ts>
    struct typelist<First, Ts...> : typelist_base<First, Ts...>
    {
        /**
         * \brief Index of the type T in the typelist
         * \tparam T Type to search
         */
        template <typename T>
        inline static constexpr std::size_t index_of = type_seq::index_of_v<T, First, Ts...>;

        /**
         * \brief The first type of the typelist
         */
        using front = type_seq::front_t<First, Ts...>;

        /**
         * \brief The last type of the typelist
         */
        using back = type_seq::back_t<First, Ts...>;

        /**
         * \brief Remove the first type of the typelist
         */
        using pop_front = type_seq::pop_front_t<typelist<First, Ts...>>;

        /**
         * \brief Remove the last type of the typelist
         */
        using pop_back = type_seq::pop_back_t<typelist<First, Ts...>>;
            
        /**
         * \brief Get the type at the index N
         * \tparam N Index
         */
        template <std::size_t N>
        using at = type_seq::at_t<N, typelist<First, Ts...>>;

        /**
         * \brief Erase the type T from the typelist
         * \tparam T Type to erase
         */
        template <typename T>
        using erase = type_seq::erase_t<T, typelist<First, Ts...>>;

        /**
         * \brief Erase the type at index N from the typelist
         * \tparam N Index
         */
        template <std::size_t N>
        using erase_at = type_seq::erase_at_t<N, typelist<First, Ts...>>;

        /**
         * \brief Insert a type at the index N of the typelist
         * \tparam N Index
         * \tparam T Type to insert
         */
        template <std::size_t N, typename T>
        using insert_at = type_seq::insert_at_t<N, T, typelist<First, Ts...>>;
    };
}
