#pragma once

#include "experimental/ser/AbstractSerializer.h"

#include <string>

namespace experimental::ser
{
    /**
     * \brief A serializer that serializes and deserializes objects to and from memory.
     * \tparam SerializerType The type of the serializer that will be used to serialize and deserialize the object.
     */
    template <typename SerializerType>
    class BinarySerializer : public AbstractSerializer<SerializerType>
    {
    public:
        /**
         * \brief Instantiates a new binary serializer.
         * \param is_reading A boolean that indicates whether the serializer is reading or writing.
         */
        explicit BinarySerializer(bool is_reading);
        virtual ~BinarySerializer() = default;

        BinarySerializer(const BinarySerializer& other) = delete;
        BinarySerializer(BinarySerializer&& other) noexcept = default;
        BinarySerializer& operator=(const BinarySerializer& other) = delete;
        BinarySerializer& operator=(BinarySerializer&& other) noexcept = default;

        /**
         * \brief Reads the given object from the serializer.
         * \tparam SerializableType The type of the object that will be read.
         * \param dest The object that will be read from the serializer.
         */
        template <typename SerializableType>
        void read(SerializableType& dest);

        /**
         * \brief Reads the given string from the serializer. Specialization of the read method for strings.
         * \param dest The string that will be read from the serializer.
         */
        void read(std::string& dest);

        /**
         * \brief Writes the given object to the serializer.
         * \tparam SerializableType The type of the object that will be written.
         * \param src The object that will be written to the serializer.
         */
        template <typename SerializableType>
        void write(const SerializableType& src);

        /**
         * \brief Writes the given string to the serializer. Specialization of the write method for strings.
         * \param src The string that will be written to the serializer.
         */
        void write(const std::string& src);

    protected:
        bool isReading;
    };
}

#include "experimental/ser/impl/BinarySerializer.h"
