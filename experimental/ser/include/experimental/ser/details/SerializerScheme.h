#pragma once

#include "spark/mpl/index_list.h"
#include "spark/rtti/HasRtti.h"

#include "boost/preprocessor/seq/for_each.hpp"

namespace spark::ser::details
{
#define SPARK_SER_DETAILS_SERIALIZE(r, data, elem) serializer << obj.elem;
#define SPARK_SER_DETAILS_DESERIALIZE(r, data, elem) serializer >> obj.elem;

#define SPARK_SER_DETAILS_SERIALIZE_SIMPLE_CLASS(ClassName, ...)                                            \
    template<typename SerializerType>                                                                       \
    struct spark::ser::SerializerScheme<SerializerType, ClassName>                                          \
    {                                                                                                       \
        static void serialize(SerializerType& serializer, const ClassName& obj)                             \
        {                                                                                                   \
            BOOST_PP_SEQ_FOR_EACH(SPARK_SER_DETAILS_SERIALIZE, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))    \
        }                                                                                                   \
                                                                                                            \
        static void deserialize(SerializerType& serializer, ClassName& obj)                                 \
        {                                                                                                   \
            BOOST_PP_SEQ_FOR_EACH(SPARK_SER_DETAILS_DESERIALIZE, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))  \
        }                                                                                                   \
    };

#define SPARK_SER_DETAILS_SERIALIZE_RTTI_CLASS(ClassName, ...)                                                                  \
    template<typename SerializerType>                                                                                           \
    struct spark::ser::SerializerScheme<SerializerType, ClassName>                                                              \
    {                                                                                                                           \
        static_assert(std::is_base_of_v<spark::rtti::HasRtti, ClassName>, "The class must be registered into the RTTI.");       \
                                                                                                                                \
        static void serialize(SerializerType& serializer, const ClassName& obj)                                                 \
        {                                                                                                                       \
            if constexpr (ClassName::ThisRttiType::ParentTypes::size > 0)                                                       \
                spark::mpl::for_sequence(std::make_index_sequence<ClassName::ThisRttiType::ParentTypes::size> {},               \
                         [&](auto i) { serializer << static_cast<const ClassName::ThisRttiType::ParentTypes::at<i>&>(obj); });  \
            BOOST_PP_SEQ_FOR_EACH(SPARK_SER_DETAILS_SERIALIZE, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                        \
        }                                                                                                                       \
                                                                                                                                \
        static void deserialize(SerializerType& serializer, ClassName& obj)                                                     \
        {                                                                                                                       \
            if constexpr (ClassName::ThisRttiType::ParentTypes::size > 0)                                                       \
                spark::mpl::for_sequence(std::make_index_sequence<ClassName::ThisRttiType::ParentTypes::size> {},               \
                         [&](auto i) { serializer >> static_cast<ClassName::ThisRttiType::ParentTypes::at<i>&>(obj); });        \
            BOOST_PP_SEQ_FOR_EACH(SPARK_SER_DETAILS_DESERIALIZE, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                      \
        }                                                                                                                       \
    };
}
