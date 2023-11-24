#pragma once

#include "boost/preprocessor/stringize.hpp"
#include "boost/preprocessor/punctuation/comma_if.hpp"
#include "boost/preprocessor/punctuation/remove_parens.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/for_each_product.hpp"
#include "boost/preprocessor/seq/pop_front.hpp"
#include "boost/preprocessor/variadic/to_seq.hpp"

#include <array>
#include <vector>

namespace spark::rtti::details
{
    namespace globals
    {
        /**
         * \brief Returns if the string is a valid RTTI name at compile time or runtime
         * \param name The string to validate
         * \return True if all the characters are valid, else False
         */
        template <std::size_t N>
        constexpr bool validate_rtti_name(const std::array<char, N> name)
        {
            auto is_number = [](const char c) { return c >= '0' && c <= '9'; };
            auto is_uppercase = [](const char c) { return c >= 'A' && c <= 'Z'; };
            auto is_lowercase = [](const char c) { return c >= 'a' && c <= 'z'; };
            auto is_other = [](const char c) { return c == '_' || c == ':' || c == ',' || c == '*' || c == '&' || c == '<' || c == '>'; };

            for (const char c : name)
            {
                if (c == '\0') // If we find the null character, the string is complete and valid
                    return true;
                if (!is_number(c) && !is_uppercase(c) && !is_lowercase(c) && !is_other(c))
                    return false;
            }
            return false; // We did not find the null character, the string is invalid
        }

        /**
         * \brief Removes all spaces from a const char* at compile time
         * \tparam N The length of the string (can be automatically deduced)
         * \param s The string we want to remove spaces from
         * \return a The string without spaces
         */
        template <std::size_t N>
        constexpr auto convert_to_rtti_name(const char (&s)[N])
        {
            std::array<char, N> res = {};
            std::size_t i = 0;
            for (auto c : s)
                if (c != ' ')
                    res[i++] = c;
            return res;
        }
    }

    template <typename T>
    struct GetClassName;

    template <typename... ParentTypes>
    struct GetParentRtti
    {
        static std::vector<RttiBase*> exec()
        {
            return {&ParentTypes::classRtti()...};
        }
    };

    template <>
    struct GetParentRtti<>
    {
        static std::vector<RttiBase*> exec()
        {
            return {};
        }
    };
}

/**
 * \brief See \ref DECLARE_SPARK_RTTI \endref for more information
 */
#define DETAILS_DECLARE_SPARK_RTTI(...)                                                     \
public:                                                                                     \
    using ThisRttiType = spark::rtti::Rtti<__VA_ARGS__>;                                    \
    static ThisRttiType& classRtti()                                                        \
    {                                                                                       \
        return ThisRttiType::instance();                                                    \
    }                                                                                       \
    spark::rtti::RttiBase& rttiInstance() const override                                    \
    {                                                                                       \
        /* Returns the instance of the first type in the variadic arguments */              \
        return BOOST_PP_SEQ_ELEM(0, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))::classRtti();    \
    }

/**
 * \brief See \ref DEFINE_SPARK_RTTI \endref for more information
 */
#define DETAILS_SPARK_RTTI_REGISTER_NAME(ClassName)                                                                                 \
    template <>                                                                                                                     \
        struct spark::rtti::details::GetClassName<BOOST_PP_REMOVE_PARENS(ClassName)>                                                \
        {                                                                                                                           \
            constexpr static auto exec()                                                                                            \
            {                                                                                                                       \
                /* Takes any type, remove spaces and put it as string */                                                            \
                constexpr auto name = globals::convert_to_rtti_name(BOOST_PP_STRINGIZE(BOOST_PP_REMOVE_PARENS(ClassName)));         \
                static_assert(globals::validate_rtti_name(name));                                                                   \
                return name;                                                                                                        \
            }                                                                                                                       \
        };

/**
 * \brief Used by DETAILS_SPARK_RTTI_REGISTER_TPL_LOOP to create the list of template parameters.
 * \param r Internal parameter
 * \param data Data passed to the macro
 * \param i The index of the current element
 * \param elem The current element
 *
 * \return A comma and the element if the index is not 0, else only the element.
 */
#define DETAILS_SPARK_RTTI_TPL_COMMA_CHOICE(r, data, i, elem) BOOST_PP_COMMA_IF(i) elem

/**
 * \brief Used by DETAILS_SPARK_RTTI_REGISTER_TPL to create a specialization of GetClassName for each product of the template parameters.
 * \param r Internal parameter
 * \param Tokens A sequence of template parameters
 *
 * \return A specialization of GetClassName for the given template parameters.
 */
#define DETAILS_SPARK_RTTI_REGISTER_TPL_LOOP(r, Tokens) DETAILS_SPARK_RTTI_REGISTER_NAME((BOOST_PP_SEQ_HEAD(Tokens) < BOOST_PP_SEQ_FOR_EACH_I(DETAILS_SPARK_RTTI_TPL_COMMA_CHOICE, _, BOOST_PP_SEQ_POP_FRONT(Tokens)) >))

/**
 * \brief See \ref SPARK_RTTI_REGISTER_TPL \endref for more information
 */
#define DETAILS_SPARK_RTTI_REGISTER_TPL(ClassName, ...) BOOST_PP_SEQ_FOR_EACH_PRODUCT(DETAILS_SPARK_RTTI_REGISTER_TPL_LOOP, ((ClassName))BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
