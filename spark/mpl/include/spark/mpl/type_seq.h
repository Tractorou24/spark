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
    struct index_of { };

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

    // front
    template <typename... Ts>
    struct front { };

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
}
