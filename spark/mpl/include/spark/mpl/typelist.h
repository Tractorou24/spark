#pragma once

#include "spark/mpl/details/type_seq.h"

namespace spark::mpl
{
    template <typename... Types>
    struct typelist;

    template <typename... Types>
    struct typelist_base
    {
        static constexpr std::size_t size = sizeof...(Types);

        template <typename NewType>
        using push_back = details::type_seq::push_back<typelist<Types...>, NewType>;

        template <typename FirstType>
        using push_front = details::type_seq::push_front<typelist<Types...>, FirstType>;

        template <typename T>
        using contains = typename details::type_seq::contains<typelist<Types...>, T>::value;

        template <int Pos>
        using at = details::type_seq::at<typelist<Types...>, Pos>;

        template <int Pos>
        using remove = details::type_seq::remove<typelist<Types...>, Pos>;
    };

    template <>
    struct typelist<> : typelist_base<> {};

    template <typename FirstType, typename... OtherTypes>
    struct typelist<FirstType, OtherTypes...> : typelist_base<FirstType, OtherTypes...>
    {
        using front = details::TypeSeqFront<typelist<OtherTypes...>>;
        using back = details::TypeSeqBack<typelist<OtherTypes...>>;
        using pop_back = details::type_seq::pop_back<typelist<FirstType, OtherTypes...>>;
    };
}
