#include "spark/ser/MemorySerializer.h"

#include "spark/base/Exception.h"

#include <cstring>

namespace spark::ser
{
    MemorySerializer::MemorySerializer()
        : BinarySerializer(false), m_readOffset(0) {}

    MemorySerializer::MemorySerializer(ContentType buffer)
        : BinarySerializer(true), m_readOffset(0), m_data(std::move(buffer)) {}

    MemorySerializer::ContentType MemorySerializer::getContent()
    {
        return m_data;
    }

    void MemorySerializer::readImpl(char* dest, const std::size_t size)
    {
        if (!isReading)
            SPARK_BASE_THROW_EXCEPTION(spark::base::WrongSerializerMode, "Can't read when in write mode");

        if (size + m_readOffset > m_data.size())
            SPARK_BASE_THROW_EXCEPTION(spark::base::OverflowException, "Can't read past the end of the buffer");

        std::memcpy(dest, &m_data[m_readOffset], size);
        m_readOffset += size;
    }

    void MemorySerializer::writeImpl(const char* src, const std::size_t size)
    {
        if (isReading)
            SPARK_BASE_THROW_EXCEPTION(spark::base::WrongSerializerMode, "Can't write when in read mode");

        const std::size_t offset = m_data.size();
        m_data.resize(offset + size);
        std::memcpy(&m_data[offset], src, size);
    }
}
