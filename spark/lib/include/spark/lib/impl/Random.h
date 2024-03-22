#pragma once
#include "spark/base/Macros.h"

namespace spark::lib
{
    template <typename T> requires std::is_floating_point_v<T>
    T Random::Number(const T min, const T max)
    {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(s_generator);
    }

    template <typename C> requires std::ranges::range<C>
    typename C::value_type Random::ElementInRange(const C& range)
    {
        SPARK_ASSERT(std::ranges::size(range) > 0 && "Cannot get a random element in an empty range.")

        std::uniform_int_distribution<std::size_t> distribution(0, std::ranges::size(range) - 1);
        return range[distribution(s_generator)];
    }
}
