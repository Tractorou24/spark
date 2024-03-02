#pragma once

namespace experimental::ser
{
    template <typename SerializerType>
    BinarySerializer<SerializerType>::BinarySerializer(const bool is_reading)
        : isReading(is_reading) {}

    template <typename SerializerType>
    template <typename SerializableType>
    void BinarySerializer<SerializerType>::read(SerializableType& dest)
    {
        this->type().readImpl(reinterpret_cast<char*>(&dest), sizeof(SerializableType));
    }

    template <typename SerializerType>
    void BinarySerializer<SerializerType>::read(std::string& dest)
    {
        std::size_t size;
        *this >> size;
        dest.resize(size);
        for (char& c : dest)
            *this >> c;
    }

    template <typename SerializerType>
    template <typename SerializableType>
    void BinarySerializer<SerializerType>::write(const SerializableType& src)
    {
        this->type().writeImpl(reinterpret_cast<const char*>(&src), sizeof(SerializableType));
    }

    template <typename SerializerType>
    void BinarySerializer<SerializerType>::write(const std::string& src)
    {
        *this << src.size();
        for (const char c : src)
            *this << c;
    }
}
