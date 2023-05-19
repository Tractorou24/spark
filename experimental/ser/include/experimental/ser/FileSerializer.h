#pragma once

#include "spark/ser/BinarySerializer.h"
#include "spark/ser/Export.h"

#include <filesystem>
#include <fstream>

namespace spark::ser
{
    /**
     * \brief A serializer that stores data in a file.
     */
    class SPARK_SER_EXPORT FileSerializer final : public BinarySerializer<FileSerializer>
    {
        template <typename SerializerType, typename SerializableType>
        friend struct SerializerScheme;

        friend class BinarySerializer;

    public:
        /**
         * \brief Instantiates a new FileSerializer.
         * \param filename The path to the file to use.
         * \param is_reading A boolean indicating whether the serializer should be used for reading or writing.
         */
        explicit FileSerializer(const std::filesystem::path& filename, bool is_reading);

    private:
        /**
         * \brief Reads the given amount of bytes from the file.
         * \param dest A pointer to the destination buffer.
         * \param size The amount of bytes to read. Must be less than or equal to the size of the destination buffer.
         */
        void readImpl(char* dest, std::streamsize size);

        /**
         * \brief Writes the given amount of bytes to the file.
         * \param src A pointer to the source buffer.
         * \param size The amount of bytes to write. Must be less than or equal to the size of the source buffer.
         */
        void writeImpl(const char* src, std::streamsize size);

    private:
        std::fstream m_file;
    };
}
