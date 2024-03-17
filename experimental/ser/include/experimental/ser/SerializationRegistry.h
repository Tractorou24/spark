#pragma once

#include <functional>
#include <unordered_map>

namespace experimental::ser
{
    /**
     * \brief Registry to hold serialization and deserialization functions for a given type.
     * \tparam SerializerType The type of the serializer to use.
     * \tparam Key The type of the key to use to identify the type.
     * \tparam BaseType The common type for all types that will be registered.
     */
    template <typename SerializerType, typename Key, typename BaseType>
    class SerializationRegistry
    {
        template <typename SerializableType>
        using SerializationFn = std::function<void(SerializerType&, const SerializableType&)>;

        template <typename SerializableType>
        using DeserializationFn = std::function<void(SerializerType&, SerializableType&)>;

    public:
        /**
         * \brief Registers a type with the given key. The corresponding @ref SerializerScheme specialization must be before this call.
         * \tparam SerializableType The type to register.
         * \param key The key to use to identify the type.
         *
         * \throws base::BadArgumentExceptio if the type has already been registered.
         */
        template <typename SerializableType>
        void registerType(const Key& key);

        /**
         * \brief Gets the serialization function for the given type.
         * \param type The key to use to identify the type.
         * \return The @ref SerializationFn function for the given type.
         *
         * \throws base::BadArgumentException if the type has not been registered.
         */
        [[nodiscard]] SerializationFn<BaseType> serializer(const Key& type) const;

        /**
         * \brief Gets the deserialization function for the given type.
         * \param type The key to use to identify the type.
         * \return The @ref DeserializationFn for the given type.
         */
        [[nodiscard]] DeserializationFn<BaseType> deserializer(const Key& type) const;

    private:
        std::unordered_map<Key, SerializationFn<BaseType>> m_serializers;
        std::unordered_map<Key, DeserializationFn<BaseType>> m_deserializers;
    };
}

#include "experimental/ser/impl/SerializationRegistry.h"
