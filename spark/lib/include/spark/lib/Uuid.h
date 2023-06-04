#pragma once

#include "spark/lib/Export.h"

#include "spark/base/Endianness.h"

#include <string>

#include <cstdint>

namespace spark::lib
{
    /**
     * \brief A 128-bit universally unique identifier (UUID).
     *
     * Uses Uses SIMD (Single Instruction Multiple Data) with Intel's AVX2 instruction set to generate UUIDs.
     * UUIDs are stored as a 16-byte array, representing a 128-bit number in memory.
     *
     * See: https://www.rfc-editor.org/rfc/rfc4122
     */
    class SPARK_LIB_EXPORT Uuid final
    {
    public:
        /**
         * \brief Instantiates a new random uuid.
         */
        explicit Uuid();
        ~Uuid() = default;

        /**
         * \brief Instantiates a new uuid from a 128-bit number.
         * \param uuid The 128-bit number to use as uuid.
         */
        explicit Uuid(__m128i uuid);

        /**
         * \brief Instantiates a new uuid from a byte string.
         * \param bytes A raw array of bytes (size 16) containing the uuid.
         */
        explicit Uuid(const uint8_t* bytes);

        /**
         * \brief Instantiates a new uuid from a byte string.
         * \param bytes A std::string containing the uuid.
         */
        explicit Uuid(const std::string& bytes);

        Uuid(const Uuid& other);
        Uuid(Uuid&& other) noexcept;
        Uuid& operator=(const Uuid& other);
        Uuid& operator=(Uuid&& other) noexcept;

        bool operator==(const Uuid& other) const;
        bool operator!=(const Uuid& other) const;

        friend std::ostream& operator<<(std::ostream& os, const Uuid& uuid);

        /**
         * \brief Serializes the uuid to a byte std::string (16 bytes).
         * \return The std::string containing the 16 bytes of the uuid.
         */
        [[nodiscard]] std::string bytes() const;

        /**
         * \brief Converts the uuid to its string representation.
         * \return The std::string representation of the uuid.
         */
        [[nodiscard]] std::string str() const;

        /**
         * \brief Converts the uuid to its string representation.
         * \param res A char array of size 36 to store the result in.
         */
        void str(char* res) const;

        /**
         * \brief Hashes the uuid.
         * \param obj The uuid to hash.
         * \return The hashed representation of the uuid.
         */
        friend std::size_t hash_value(const Uuid& obj);

    private:
        void bytes(std::string& out) const;
        void bytes(char* bytes) const;

    private:
        alignas(128) std::uint8_t m_data[16];
    };
}

template <>
struct std::hash<spark::lib::Uuid>
{
    std::size_t operator()(const spark::lib::Uuid& obj) const noexcept
    {
        return hash_value(obj);
    }
};
