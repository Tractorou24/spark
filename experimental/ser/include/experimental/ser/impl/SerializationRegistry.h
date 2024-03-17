#pragma once

#include "spark/base/Exception.h"

namespace experimental::ser
{
    template <typename SerializerType, typename Key, typename BaseType>
    template <typename SerializableType>
    void SerializationRegistry<SerializerType, Key, BaseType>::registerType(const Key& key)
    {
        if (m_serializers.contains(key) || m_deserializers.contains(key))
            throw spark::base::BadArgumentException("Cannot register a type that is already registered");

        m_serializers[key] = [](SerializerType& ser, const BaseType& t)
        {
            experimental::ser::SerializerScheme<SerializerType, SerializableType>::serialize(ser, dynamic_cast<const SerializableType&>(t));
        };
        m_deserializers[key] = [](SerializerType& ser, BaseType& t)
        {
            experimental::ser::SerializerScheme<SerializerType, SerializableType>::deserialize(ser, dynamic_cast<SerializableType&>(t));
        };
    }

    template <typename SerializerType, typename Key, typename BaseType>
    typename SerializationRegistry<SerializerType, Key, BaseType>::template SerializationFn<BaseType> SerializationRegistry<SerializerType, Key, BaseType>::serializer(
        const Key& type) const
    {
        if (!m_serializers.contains(type))
            throw spark::base::BadArgumentException("Cannot get a serializer for a type that is not registered");
        return m_serializers.at(type);
    }

    template <typename SerializerType, typename Key, typename BaseType>
    typename SerializationRegistry<SerializerType, Key, BaseType>::template DeserializationFn<BaseType> SerializationRegistry<
        SerializerType, Key, BaseType>::deserializer(const Key& type) const
    {
        if (!m_deserializers.contains(type))
            throw spark::base::BadArgumentException("Cannot get a deserializer for a type that is not registered");
        return m_deserializers.at(type);
    }
}
