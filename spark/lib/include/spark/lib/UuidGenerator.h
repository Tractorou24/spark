#pragma once

#include "spark/lib/Export.h"
#include "spark/lib/Uuid.h"

#include "spark/base/Macros.h"

#include <random>

namespace spark::lib
{
    /**
     * \brief Generates UUIDs from C++11 <random> module.
     */
    class SPARK_LIB_EXPORT UuidGenerator final
    {
    public:
        /**
         * \brief Instantiates a new uuid generator with a base seed.
         */
        explicit UuidGenerator();

        /**
         * \brief Instantiates a new uuid generator with a provided seed.
         * \param seed The seed to use for the random generator.
         */
        explicit UuidGenerator(uint64_t seed);

        /**
         * \brief Generates a new uuid.
         * \return The generated uuid.
         */
        Uuid generate();

    private:
        SPARK_WARNING_PUSH
        SPARK_DISABLE_MSVC_WARNING(4251)
        /*
         * class 'std::mt19937_64' needs to have dll-interface to be used by clients of class 'spark::base::UuidGenerator'
         * class 'std::uniform_int_distribution<uint64_t>' needs to have dll-interface to be used by clients of class 'spark::base::UuidGenerator'
         *
         * This is a false positive, the class is not exported but is from the STL and the warning can be safely ignored.
         */
        std::mt19937_64 m_generator;
        std::uniform_int_distribution<uint64_t> m_distribution;
        SPARK_WARNING_POP
    };
}
