#pragma once

#include "spark/mpl/typelist.h"

#include "boost/preprocessor/seq/to_tuple.hpp"
#include "boost/preprocessor/tuple/rem.hpp"

namespace spark::base
{
    // BoostPP sequence of all primitive types
#   define SPARK_BASE_ALL_PRIMITIVE_TYPE_SEQ    (bool)                 \
                                                (char)                 \
                                                (short)                \
                                                (int)                  \
                                                (long long)            \
                                                (unsigned char)        \
                                                (unsigned short)       \
                                                (unsigned int)         \
                                                (unsigned long long)   \
                                                (float)                \
                                                (double)

    // BoostPP sequence of all scalar primitive types
#   define SPARK_BASE_ALL_SCALAR_PRIMITIVE_TYPE_SEQ (char)                 \
                                                    (short)                \
                                                    (int)                  \
                                                    (long long)            \
                                                    (unsigned char)        \
                                                    (unsigned short)       \
                                                    (unsigned int)         \
                                                    (unsigned long long)   \
                                                    (float)                \
                                                    (double)


    // BoostPP sequence of all integer types
#   define SPARK_BASE_ALL_INTEGER_PRIMITIVE_TYPE_SEQ    (char)                 \
                                                        (short)                \
                                                        (int)                  \
                                                        (long long)            \
                                                        (unsigned char)        \
                                                        (unsigned short)       \
                                                        (unsigned int)         \
                                                        (unsigned long long)

    // BoostPP sequence of all floating types
#   define SPARK_BASE_ALL_FLOATING_PRIMITIVE_TYPE_SEQ (float)(double)

    using AllPrimitiveTypesList = mpl::typelist<BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_SEQ_TO_TUPLE(SPARK_BASE_ALL_PRIMITIVE_TYPE_SEQ))>;
    using AllScalarPrimitiveTypesList = mpl::typelist<BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_SEQ_TO_TUPLE(SPARK_BASE_ALL_SCALAR_PRIMITIVE_TYPE_SEQ))>;
    using AllIntegerPrimitiveTypesList = mpl::typelist<BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_SEQ_TO_TUPLE(SPARK_BASE_ALL_INTEGER_PRIMITIVE_TYPE_SEQ))>;
    using AllFloatingPrimitiveTypesList = mpl::typelist<BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_SEQ_TO_TUPLE(SPARK_BASE_ALL_FLOATING_PRIMITIVE_TYPE_SEQ))>;
}
