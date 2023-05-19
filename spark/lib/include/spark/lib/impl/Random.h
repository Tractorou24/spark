#pragma once

namespace spark::lib
{
    template <typename T> requires std::is_floating_point_v<T>
    T Random::Number(const T min, const T max)
    {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(s_generator);
    }
}
