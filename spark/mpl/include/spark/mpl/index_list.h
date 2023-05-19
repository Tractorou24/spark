#pragma once

#include <utility>

namespace spark::mpl
{
    template <typename T, T... S, typename F>
    constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
    {
        (f(std::integral_constant<T, S> {}), ...);
    }
}
