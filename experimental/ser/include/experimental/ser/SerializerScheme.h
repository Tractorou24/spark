#pragma once

#include "experimental/ser/details/SerializerScheme.h"

namespace experimental::ser
{
    /**
     * \brief A struct that defines the serialization and deserialization methods for a given type.
     * \tparam SerializerType The type of the serializer that will be used to serialize and deserialize the object.
     * \tparam SerializableType The type of the object that will be serialized and deserialized.
     *
     * This struct is used to define the serialization and deserialization methods for a given type with 2 methods:
     *      - static void serialize(SerializerType& serializer, const SerializableType& obj);
     *      - static void deserialize(SerializerType& serializer, SerializableType& obj);
     */
    template <typename SerializerType, typename SerializableType>
    struct SerializerScheme {};
}

/**
 * \brief Allows a class private members to be serialized and deserialized.
 */
#define SPARK_ALLOW_PRIVATE_SERIALIZATION template <typename SerializerType, typename SerializedType> friend struct experimental::ser::SerializerScheme;

/**
 * \brief Generates an empty serialization scheme for the given class.
 * \param ClassName The name of the class for which the serialization scheme will be generated.
 */
#define SPARK_DEFINE_EMPTY_SERIALIZER_SCHEME(ClassName)                     \
    template <typename SerializerType>                                      \
    struct experimental::ser::SerializerScheme<SerializerType, ClassName>   \
    {                                                                       \
        static void serialize(SerializerType&, const ClassName&) {}         \
        static void deserialize(SerializerType&, ClassName&) {}             \
    };

/**
 * \brief Generates the serialization scheme for a class not registered into the RTTI system.
 * \param TypeName The name of the class for which the serialization scheme will be generated.
 * \param ... The list of members of the class that will be serialized.
 *
 * \warning This macro must be placed in the global namespace only.
 */
#define SPARK_SERIALIZE_SIMPLE_CLASS(ClassName, ...) SPARK_SER_DETAILS_SERIALIZE_SIMPLE_CLASS(ClassName, __VA_ARGS__)

/**
 * \brief Generates the serialization scheme for a class registered into the RTTI system.
 * \param ClassName The name of the class for which the serialization scheme will be generated.
 * \param ... The list of members of the class that will be serialized.
 *
 * \warning This macro must be placed in the global namespace only.
 */
#define SPARK_SERIALIZE_RTTI_CLASS(ClassName, ...) SPARK_SER_DETAILS_SERIALIZE_RTTI_CLASS(ClassName, __VA_ARGS__)

#include "experimental/ser/impl/SerializerScheme.h"
