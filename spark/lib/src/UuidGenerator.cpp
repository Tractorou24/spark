#include "spark/lib/UuidGenerator.h"

// ReSharper disable CppClangTidyBugproneNarrowingConversions

namespace spark::lib
{
    UuidGenerator::UuidGenerator()
        : m_generator(std::mt19937_64(std::random_device()())),
          m_distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    UuidGenerator::UuidGenerator(const uint64_t seed)
        : m_generator(std::mt19937_64(seed)), m_distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    Uuid UuidGenerator::generate()
    {
        // The two masks set the uuid version (4) and variant (1)
        const __m128i and_mask = _mm_set_epi64x(0xFFFFFFFFFFFFFF3Full, 0xFF0FFFFFFFFFFFFFull);
        const __m128i or_mask = _mm_set_epi64x(0x0000000000000080ull, 0x0040000000000000ull);
        const __m128i n = _mm_set_epi64x(m_distribution(m_generator), m_distribution(m_generator));
        const __m128i uuid = _mm_or_si128(_mm_and_si128(n, and_mask), or_mask);

        return Uuid(uuid);
    }
}
