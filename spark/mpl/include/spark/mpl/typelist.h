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
    };
}
