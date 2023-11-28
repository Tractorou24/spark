#pragma once

#include "boost/preprocessor/seq/enum.hpp"

#define SPARK_BASE_MOUSECODES_VALUES (Left)(Right)(Middle)(XButton1)(XButton2)(Unknown)

namespace spark::base
{
    using MouseCode = unsigned short;

    namespace MouseCodes
    {
        enum : MouseCode
        {
            BOOST_PP_SEQ_ENUM(SPARK_BASE_MOUSECODES_VALUES)
        };
    }
}
