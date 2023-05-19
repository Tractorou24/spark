#include "spark/ser/FileSerializer.h"

#include "spark/base/Exception.h"

#include "fmt/core.h"

namespace spark::ser
{
    FileSerializer::FileSerializer(const std::filesystem::path& filename, const bool is_reading)
        : BinarySerializer(is_reading)
    {
        m_file.open(filename, isReading ? (std::fstream::in | std::fstream::binary) : (std::fstream::out | std::fstream::trunc | std::fstream::binary));
        if (!m_file.is_open())
        {
            const std::string error_message = fmt::format("Can't open file {0} for {1}", filename.string(), is_reading ? "reading" : "writing");
            SPARK_BASE_THROW_EXCEPTION(spark::base::CouldNotOpenFileException, error_message.c_str());
        }
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
