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

        /**
         * \brief Convert the typelist with the function F
         * \tparam T Type to convert to
         */
        template <template <typename...> typename T>
        using convert = type_seq::convert_t<T, Ts...>;
    };

    // Specialization for empty typelist
    template <>
    struct typelist<> : typelist_base<> {};

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
         * \brief Find the index of the type T in the typelist
         * \tparam T Type to search
         */
        template <typename T>
        inline static constexpr std::size_t find = type_seq::find_v<T, Ts...>;

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

        /**
         * \brief Replace the type T by the type U in the typelist
         * \tparam T Type to replace
         * \tparam U Type to insert
         */
        template <typename T, typename U>
        using replace = type_seq::replace_t<T, U, typelist<First, Ts...>>;

        /**
         * \brief Replace the type at index N by the type T in the typelist
         * \tparam N Index
         * \tparam T Type to insert
         */
        template <std::size_t N, typename T>
        using replace_at = type_seq::replace_at_t<N, T, typelist<First, Ts...>>;

        /**
         * \brief Reverse the typelist
         */
        using reverse = type_seq::reverse_t<typelist<First, Ts...>>;

        /**
         * \brief Filter the typelist with the predicate F
         * \tparam F Predicate taking a type as parameter and having a static constexpr value member as result
         */
        template <template <typename> typename F>
        using filter = type_seq::filter_t<F, typelist<First, Ts...>>;

        /**
         * \brief Convert the typelist with the function F
         * \tparam T Type to convert to
         */
        template <template <typename...> typename T>
        using convert = type_seq::convert_t<T, First, Ts...>;

        /**
         * \brief Apply the function F to each type of the typelist
         * \tparam F A function taking a type as template parameter and having a type using as result
         */
        template <template <typename> typename F>
        using transform = type_seq::transform_t<F, typelist<First, Ts...>>;
    };

    /**
     * \brief Concatenate given typelists to one
     * \tparam Lists Typelists to concatenate
     */
    template <typename... Lists>
    using typelist_concat = type_seq::concat_t<Lists...>;

    /**
     * \brief Flattens a multi level typelist to a single level typelist
     * \tparam List T Typelist to flatten
     */
    template <typename List>
    using typelist_flatten = type_seq::flatten_t<List>;

    /**
     * \brief Checks if elements of the typelists at the same index match each other using \p Matcher.
     * \tparam Matcher The matcher to use and having a static constexpr bool value member
     * \tparam L1 The first typelist
     * \tparam L2 The second typelist
     */
    template <template <typename, typename> typename Matcher, typename L1, typename L2>
    static constexpr bool typelist_match = type_seq::match_v<Matcher, L1, L2>;
}
