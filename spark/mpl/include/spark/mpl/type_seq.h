#pragma once

namespace spark::mpl::type_seq
{
    // size
    template <typename... Ts>
    struct size
    {
        inline static constexpr std::size_t value = sizeof...(Ts);
    };

    template <typename... Ts>
    inline static constexpr std::size_t size_v = size<Ts...>::value;

    // empty
    template <typename... Ts>
    struct empty
    {
        inline static constexpr bool value = sizeof...(Ts) == 0;
    };

    template <typename... Ts>
    inline static constexpr bool empty_v = empty<Ts...>::value;

    // contains
    template <typename T, typename... Ts>
    struct contains {};

    template <typename T, typename Head, typename... Ts>
    struct contains<T, Head, Ts...>
    {
        inline static constexpr bool value = std::is_same_v<T, Head> || contains<T, Ts...>::value;
    };

    template <typename T>
    struct contains<T>
    {
        inline static constexpr bool value = false;
    };

    template <typename T, typename... Ts>
    inline static constexpr bool contains_v = contains<T, Ts...>::value;

    // index_of
    template <typename T, typename... Ts>
    struct index_of {};

    template <typename T, typename Head, typename... Ts>
    struct index_of<T, Head, Ts...>
    {
        inline static constexpr std::size_t value = std::is_same_v<T, Head> ? 0 : 1 + index_of<T, Ts...>::value;
    };

    template <typename T>
    struct index_of<T>
    {
        inline static constexpr std::size_t value = 0;
    };

    template <typename T, typename... Ts>
    inline static constexpr std::size_t index_of_v = index_of<T, Ts...>::value;

    // find
    template <typename T, typename... Ts>
    struct find {};

    template <template<typename...> typename C, typename T, typename Head, typename... Ts>
    struct find<T, C<Head, Ts...>>
    {
        inline static constexpr std::size_t value = find<T, C<Ts...>>::value + 1;
    };

    template <template<typename...> typename C, typename T, typename... Ts>
    struct find<T, C<T, Ts...>>
    {
        inline static constexpr std::size_t value = 0;
    };

    template <typename T, typename... Ts>
    inline static constexpr std::size_t find_v = find<T, Ts...>::value;

    // front
    template <typename... Ts>
    struct front {};

    template <typename Head, typename... Ts>
    struct front<Head, Ts...>
    {
        using type = Head;
    };

    template <typename... Ts>
    using front_t = typename front<Ts...>::type;

    // back
    template <typename... Ts>
    struct back {};

    template <typename Head, typename... Ts>
    struct back<Head, Ts...>
    {
        using type = typename back<Ts...>::type;
    };

    template <typename Head>
    struct back<Head>
    {
        using type = Head;
    };

    template <typename... Ts>
    using back_t = typename back<Ts...>::type;

    // push_front
    template <typename T, typename List>
    struct push_front {};

    template <template<typename...> typename C, typename T, typename... Ts>
    struct push_front<T, C<Ts...>>
    {
        using type = C<T, Ts...>;
    };

    template <typename T, typename... Ts>
    using push_front_t = typename push_front<T, Ts...>::type;

    // push_back
    template <typename T, typename... Ts>
    struct push_back {};

    template <template<typename...> typename C, typename T, typename... Ts>
    struct push_back<T, C<Ts...>>
    {
        using type = C<Ts..., T>;
    };

    template <typename T, typename... Ts>
    using push_back_t = typename push_back<T, Ts...>::type;

    // pop_front
    template <typename List>
    struct pop_front {};

    template <template<typename...> typename C, typename Head, typename... Ts>
    struct pop_front<C<Head, Ts...>>
    {
        using type = C<Ts...>;
    };

    template <typename... Ts>
    using pop_front_t = typename pop_front<Ts...>::type;

    // pop_back
    template <typename... Ts>
    struct pop_back {};

    template <template<typename...> typename C, typename Head, typename... Ts>
    struct pop_back<C<Head, Ts...>>
    {
        using type = typename push_front<Head, typename pop_back<C<Ts...>>::type>::type;
    };

    template <template<typename...> typename C, typename Head>
    struct pop_back<C<Head>>
    {
        using type = C<>;
    };

    template <typename... Ts>
    using pop_back_t = typename pop_back<Ts...>::type;

    // at
    template <std::size_t N, typename List>
    struct at {};

    template <template<typename...> typename C, std::size_t N, typename Head, typename... Ts>
    struct at<N, C<Head, Ts...>>
    {
        using type = typename at<N - 1, C<Ts...>>::type;
    };

    template <template<typename...> typename C, typename Head, typename... Ts>
    struct at<0, C<Head, Ts...>>
    {
        using type = Head;
    };

    template <std::size_t N, typename... Ts>
    using at_t = typename at<N, Ts...>::type;

    // erase
    template <typename T, typename... Ts>
    struct erase {};

    template <template<typename...> typename C, typename T, typename... Ts>
    struct erase<T, C<T, Ts...>>
    {
        using type = C<Ts...>;
    };

    template <template<typename...> typename C, typename T, typename Head, typename... Ts>
    struct erase<T, C<Head, Ts...>>
    {
        using type = typename push_front<Head, typename erase<T, C<Ts...>>::type>::type;
    };

    template <typename T, typename... Ts>
    using erase_t = typename erase<T, Ts...>::type;

    // erase_at
    template <std::size_t N, typename... Ts>
    struct erase_at {};

    template <template<typename...> typename C, std::size_t N, typename Head, typename... Ts>
    struct erase_at<N, C<Head, Ts...>>
    {
        using type = typename push_front<Head, typename erase_at<N - 1, C<Ts...>>::type>::type;
    };

    template <template<typename...> typename C, typename Head, typename... Ts>
    struct erase_at<0, C<Head, Ts...>>
    {
        using type = C<Ts...>;
    };

    template <std::size_t N, typename... Ts>
    using erase_at_t = typename erase_at<N, Ts...>::type;

    // insert_at
    template <std::size_t N, typename T, typename... Ts>
    struct insert_at {};

    template <template<typename...> typename C, std::size_t N, typename T, typename Head, typename... Ts>
    struct insert_at<N, T, C<Head, Ts...>>
    {
        using type = typename push_front<Head, typename insert_at<N - 1, T, C<Ts...>>::type>::type;
    };

    template <template<typename...> typename C, typename T, typename Head, typename... Ts>
    struct insert_at<0, T, C<Head, Ts...>>
    {
        using type = typename push_front<T, C<Head, Ts...>>::type;
    };

    template <template<typename...> typename C, std::size_t N, typename T>
    struct insert_at<N, T, C<>>
    {
        using type = C<T>;
    };

    template <std::size_t N, typename T, typename... Ts>
    using insert_at_t = typename insert_at<N, T, Ts...>::type;

    // replace
    template <typename T, typename U, typename... Ts>
    struct replace {};

    template <template<typename...> typename C, typename T, typename U, typename Head, typename... Ts>
    struct replace<T, U, C<Head, Ts...>>
    {
        using type = typename push_front<Head, typename replace<T, U, C<Ts...>>::type>::type;
    };

    template <template<typename...> typename C, typename T, typename U, typename Head, typename... Ts>
    struct replace<T, U, C<T, Head, Ts...>>
    {
        using type = typename push_front<U, typename replace<T, U, C<Head, Ts...>>::type>::type;
    };

    template <template<typename...> typename C, typename T, typename U, typename... Ts>
    struct replace<T, U, C<T, Ts...>>
    {
        using type = typename replace<T, U, C<U, Ts...>>::type;
    };

    template <template<typename...> typename C, typename T, typename U>
    struct replace<T, U, C<>>
    {
        using type = C<>;
    };

    template <typename T, typename U, typename... Ts>
    using replace_t = typename replace<T, U, Ts...>::type;

    // replace_at
    template <std::size_t N, typename T, typename... Ts>
    struct replace_at {};

    template <template<typename...> typename C, std::size_t N, typename T, typename Head, typename... Ts>
    struct replace_at<N, T, C<Head, Ts...>>
    {
        using type = typename push_front<Head, typename replace_at<N - 1, T, C<Ts...>>::type>::type;
    };

    template <template<typename...> typename C, typename T, typename Head, typename... Ts>
    struct replace_at<0, T, C<Head, Ts...>>
    {
        using type = typename push_front<T, C<Ts...>>::type;
    };

    template <template<typename...> typename C, std::size_t N, typename T>
    struct replace_at<N, T, C<>>
    {
        using type = C<>;
    };

    template <std::size_t N, typename T, typename... Ts>
    using replace_at_t = typename replace_at<N, T, Ts...>::type;

    // reverse
    template <typename... Ts>
    struct reverse {};

    template <template<typename...> typename C, typename... TypesToAdd, typename Head, typename... Ts>
    struct reverse<C<Head, Ts...>, TypesToAdd...>
    {
        using type = typename reverse<C<Ts...>, Head, TypesToAdd...>::type;
    };

    template <template<typename...> typename C, typename... TypesToAdd>
    struct reverse<C<>, TypesToAdd...>
    {
        using type = C<TypesToAdd...>;
    };

    template <typename... Ts>
    using reverse_t = typename reverse<Ts...>::type;

    // filter
    template <template <typename> typename Predicate, typename... Ts>
    struct filter {};

    template <template<typename...> typename C, template <typename> typename Predicate, typename Head, typename... Ts>
    struct filter<Predicate, C<Head, Ts...>>
    {
        using type = std::conditional_t<Predicate<Head>::value, typename push_front<Head, typename filter<Predicate, C<Ts...>>::type>::type, typename filter<
                                            Predicate, C<Ts...>>::type>;
    };

    template <template<typename...> typename C, template <typename> typename Predicate>
    struct filter<Predicate, C<>>
    {
        using type = C<>;
    };

    template <template <typename> typename Predicate, typename... Ts>
    using filter_t = typename filter<Predicate, Ts...>::type;

    // convert
    template <template <typename...> typename T, typename... Ts>
    struct convert
    {
        using type = T<Ts...>;
    };

    template <template <typename...> typename T, typename... Ts>
    using convert_t = typename convert<T, Ts...>::type;

    // transform
    template <template <typename> typename F, typename... Ts>
    struct transform {};

    template <template <typename...> typename C, template <typename> typename F, typename Head, typename... Ts>
    struct transform<F, C<Head, Ts...>>
    {
        using type = typename push_front<typename F<Head>::type, typename transform<F, C<Ts...>>::type>::type;
    };

    template <template <typename...> typename C, template <typename> typename F>
    struct transform<F, C<>>
    {
        using type = C<>;
    };

    template <template <typename> typename F, typename... Ts>
    using transform_t = typename transform<F, Ts...>::type;

    // concat
    template <typename... Lists>
    struct concat {};

    template <template <typename...> class C, typename... Ts>
    struct concat<C<Ts...>>
    {
        using type = C<Ts...>;
    };

    template <template <typename...> class C, typename... Ts, typename... Us>
    struct concat<C<Ts...>, C<Us...>>
    {
        using type = C<Ts..., Us...>;
    };

    template <typename T, typename U, typename V, typename... O>
    struct concat<T, U, V, O...>
    {
        using type = typename concat<typename concat<T, U>::type, typename concat<V, O...>::type>::type;
    };

    template <typename... Lists>
    using concat_t = typename concat<Lists...>::type;

    // flatten
    template <typename List>
    struct flatten {};

    template <template <typename...> class C, typename... Ts>
    struct flatten<C<Ts...>>
    {
        using type = typename concat<Ts...>::type;
    };

    template <template <typename...> class C>
    struct flatten<C<>>
    {
        using type = C<>;
    };

    template <typename List>
    using flatten_t = typename flatten<List>::type;

    // match
    template <template<typename, typename> typename Matcher, typename L1, typename L2>
    struct match {};

    template <template <typename, typename> typename Matcher, template <typename...> typename C, typename T, typename U, typename... Ts, typename... Us>
    struct match<Matcher, C<T, Ts...>, C<U, Us...>>
    {
        static constexpr bool value = Matcher<T, U>::value && match<Matcher, C<Ts...>, C<Us...>>::value;
    };

    template <template <typename, typename> typename Matcher, template <typename...> typename C>
    struct match<Matcher, C<>, C<>>
    {
        static constexpr bool value = true;
    };

    template <template<typename, typename> typename Matcher, typename L1, typename L2>
    inline static constexpr bool match_v = match<Matcher, L1, L2>::value;
}
