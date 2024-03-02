#pragma once

#include "experimental/ser/SerializerScheme.h"

namespace experimental::ser
{
    template <typename SerializerType>
    template <typename SerializableType>
    SerializerType& AbstractSerializer<SerializerType>::operator<<(const SerializableType& obj)
    {
        auto& serializer = type();
        SerializerScheme<SerializerType, SerializableType>::serialize(serializer, obj);
        return serializer;
    }

    template <typename SerializerType>
    template <typename SerializableType>
    SerializerType& AbstractSerializer<SerializerType>::operator>>(SerializableType& obj)
    {
        auto& serializer = type();
        SerializerScheme<SerializerType, SerializableType>::deserialize(serializer, obj);
        return serializer;
    }

    template <typename SerializerType>
    SerializerType& AbstractSerializer<SerializerType>::type()
    {
        return *static_cast<SerializerType*>(this);
    }
}
