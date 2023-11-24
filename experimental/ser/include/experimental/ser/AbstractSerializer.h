#pragma once

namespace spark::ser
{
    /**
     * \brief A base class for all serializers.
     * \tparam SerializerType The type of the serializer that will be used to serialize and deserialize the object.
     */
    template <typename SerializerType>
    class AbstractSerializer
    {
    public:
        /**
         * \brief Serializes the given object.
         * \tparam SerializableType The type of the object that will be serialized.
         * \param obj The object that will be serialized.
         * \return The serializer that was used to serialize the object.
         */
        template <typename SerializableType>
        SerializerType& operator<<(const SerializableType& obj);

        /**
         * \brief Deserializes the given object.
         * \tparam SerializableType The type of the object that will be deserialized.
         * \param obj The object that will be deserialized.
         * \return The serializer that was used to deserialize the object.
         */
        template <typename SerializableType>
        SerializerType& operator>>(SerializableType& obj);

    protected:
        /**
         * \brief Gets the concrete type of the serializer.
         * \return The concrete serializer type.
         */
        SerializerType& type();
    };
}

#include "spark/ser/impl/AbstractSerializer.h"
