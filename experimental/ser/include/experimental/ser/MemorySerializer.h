#pragma once

#include "experimental/ser/BinarySerializer.h"
#include "experimental/ser/Export.h"

#include <vector>

namespace experimental::ser
{
    /**
     * \brief A serializer that stores data in memory.
     */
    class EXPERIMENTAL_SER_EXPORT MemorySerializer final : public BinarySerializer<MemorySerializer>
    {
        template <typename SerializerType, typename SerializableType>
        friend struct SerializerScheme;

        friend class BinarySerializer;

    public:
        /**
         * \brief Represents the type of the content stored in the serializer.
         */
        using ContentType = std::vector<char>;

    public:
        /**
         * \brief Instantiates a new MemorySerializer without any content.
         */
        explicit MemorySerializer();

        /**
         * \brief Instantiates a new MemorySerializer with the given content.
         * \param buffer A vector of bytes to use as the starter content of the serializer.
         */
        explicit MemorySerializer(ContentType buffer);
        ~MemorySerializer() override = default;

        MemorySerializer(const MemorySerializer& other) = delete;
        MemorySerializer(MemorySerializer&& other) noexcept = default;
        MemorySerializer& operator=(const MemorySerializer& other) = delete;
        MemorySerializer& operator=(MemorySerializer&& other) noexcept = default;

        /**
         * \brief Gets the content of the serializer.
         * \return A vector of bytes representing the content of the serializer.
         */
        [[nodiscard]] ContentType content();

    private:
        /**
         * \brief Reads the given amount of bytes from the buffer.
         * \param dest A pointer to the destination buffer.
         * \param size The amount of bytes to read. Must be less than or equal to the size of the destination buffer.
         */
        void readImpl(char* dest, std::size_t size);

        /**
         * \brief Writes the given amount of bytes to the buffer.
         * \param src A pointer to the source buffer.
         * \param size The amount of bytes to write. Must be less than or equal to the size of the source buffer.
         */
        void writeImpl(const char* src, std::size_t size);

    private:
        std::size_t m_readOffset;
        ContentType m_data;
    };
}
