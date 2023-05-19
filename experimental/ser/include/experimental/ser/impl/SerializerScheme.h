#pragma once

#include "spark/base/BaseType.h"

#include "boost/preprocessor/seq/for_each.hpp"

/**
 * \brief Generates the serialization scheme for a primitive type.
 * \param r Internal parameter.
 * \param data Data passed to the macro.
 * \param PrimitiveType The primitive type for which the serialization scheme will be generated.
 */
#define SPARK_SER_DETAILS_SERIALIZE_PRIMITIVE_TYPE(r, data, PrimitiveType)                                  \
template<typename SerializerType>                                                                           \
struct spark::ser::SerializerScheme<SerializerType, PrimitiveType>                                          \
{                                                                                                           \
    static void serialize(SerializerType& serializer, const PrimitiveType& obj) { serializer.write(obj); }  \
    static void deserialize(SerializerType& serializer, PrimitiveType& obj) { serializer.read(obj); }       \
};

// Generate the serialization scheme for all primitive types.
BOOST_PP_SEQ_FOR_EACH(SPARK_SER_DETAILS_SERIALIZE_PRIMITIVE_TYPE, _, SPARK_BASE_ALL_PRIMITIVE_TYPE_SEQ)
BOOST_PP_SEQ_FOR_EACH(SPARK_SER_DETAILS_SERIALIZE_PRIMITIVE_TYPE, _, (unsigned long int)(long double)(std::string))

#undef SPARK_SER_DETAILS_SERIALIZE_PRIMITIVE_TYPE
