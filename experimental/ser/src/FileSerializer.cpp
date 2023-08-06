#include "spark/ser/FileSerializer.h"

#include "spark/base/Exception.h"

#include <format>

namespace spark::ser
{
    FileSerializer::FileSerializer(const std::filesystem::path& filename, const bool is_reading)
        : BinarySerializer(is_reading)
    {
        m_file.open(filename, isReading ? (std::fstream::in | std::fstream::binary) : (std::fstream::out | std::fstream::trunc | std::fstream::binary));
        if (!m_file.is_open())
            throw spark::base::CouldNotOpenFileException(std::format("Can't open file {0} for {1}", filename.string(), is_reading ? "reading" : "writing"));
    }

    void FileSerializer::readImpl(char* dest, const std::streamsize size)
    {
        m_file.read(dest, size);
    }

    void FileSerializer::writeImpl(const char* src, const std::streamsize size)
    {
        m_file.write(src, size);
        m_file.flush();
    }
}
