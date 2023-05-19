#pragma once

#include "spark/lib/Export.h"

#include <random>

namespace spark::lib
{
    /**
     * \brief A class that provides random generation.
     */
    class SPARK_LIB_EXPORT Random final
    {
    public:
        template <typename T> requires std::is_floating_point_v<T>
        static T Number(T min, T max);

    private:
        static std::random_device s_randomDevice;
        static std::mt19937 s_generator;
    };
}

#include "spark/lib/impl/Random.h"
