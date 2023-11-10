#include "spark/lib/Uuid.h"
#include "spark/lib/details/Uuid.h"
#include "spark/lib/UuidGenerator.h"

#include "spark/base/Endianness.h"

#include <ostream>

// ReSharper disable CppClangTidyCppCoreGuidelinesProTypeMemberInit

namespace spark::lib
{
    Uuid::Uuid()
    {
        static UuidGenerator generator;
        *this = generator.generate();
    }

    Uuid::Uuid(const __m128i uuid)
    {
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), uuid);
    }

    Uuid::Uuid(const uint8_t* bytes)
    {
        const __m128i x = _mm_loadu_si128(reinterpret_cast<const __m128i*>(bytes));
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), x);
    }

    Uuid::Uuid(const std::string& bytes)
    {
        const __m128i x = betole128(_mm_loadu_si128(reinterpret_cast<const __m128i*>(bytes.data())));
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), x);
    }

    Uuid::Uuid(const Uuid& other)
    {
        const __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(other.m_data));
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), x);
    }

    Uuid::Uuid(Uuid&& other) noexcept
    {
        const __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(other.m_data));
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), x);
    }

    Uuid& Uuid::operator=(const Uuid& other)
    {
        if (&other == this)
            return *this;

        const __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(other.m_data));
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), x);

        return *this;
    }

    Uuid& Uuid::operator=(Uuid&& other) noexcept
    {
        if (&other == this)
            return *this;

        const __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(other.m_data));
        _mm_store_si128(reinterpret_cast<__m128i*>(m_data), x);

        return *this;
    }

    bool Uuid::operator==(const Uuid& other) const
    {
        const __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(m_data));
        const __m128i y = _mm_load_si128(reinterpret_cast<const __m128i*>(other.m_data));
        const __m128i neq = _mm_xor_si128(x, y);

        return _mm_test_all_zeros(neq, neq);
    }

    bool Uuid::operator!=(const Uuid& other) const
    {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const Uuid& uuid)
    {
        os << uuid.str();
        return os;
    }

    std::string Uuid::bytes() const
    {
        std::string mem;
        bytes(mem);
        return mem;
    }

    std::string Uuid::str() const
    {
        std::string mem;
        mem.resize(36);
        str(mem.data());
        return mem;
    }

    void Uuid::str(char* res) const
    {
        const __m128i x = _mm_load_si128(reinterpret_cast<const __m128i*>(m_data));
        details::m128itos(x, res);
    }

    std::size_t hash_value(const Uuid& obj)
    {
        const uint64_t a = *reinterpret_cast<const uint64_t*>(obj.m_data);
        const uint64_t b = *reinterpret_cast<const uint64_t*>(&obj.m_data[8]);
        return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
    }

    void Uuid::bytes(std::string& out) const
    {
        out.resize(sizeof(m_data));
        bytes(out.data());
    }

    void Uuid::bytes(char* bytes) const
    {
        const __m128i x = betole128(_mm_load_si128(reinterpret_cast<const __m128i*>(m_data)));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(bytes), x);
    }
}
