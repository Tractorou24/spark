#include "gtest/gtest.h"

#include "spark/mpl/typelist.h"

/**
 * \brief Wrapper around \ref EXPECT_EQ to convert a static_assert into a runtime check
 * \param val1 The first value to compare
 * \param val2 The second value to compare
 */
#define STATIC_EXPECT_EQ(val1, val2) EXPECT_EQ((std::integral_constant<decltype(val1), val1>::value), (std::integral_constant<decltype(val2), (val2)>::value))

/**
 * \brief Wrapper around \ref EXPECT_TRUE to convert a static_assert into a runtime check
 * \param val The value to compare
 */
#define STATIC_EXPECT_TRUE(...) EXPECT_EQ((std::integral_constant<decltype(__VA_ARGS__), __VA_ARGS__>::value), true)

/**
 * \brief Wrapper around \ref EXPECT_FALSE to convert a static_assert into a runtime check
 * \param val The value to compare
 */
#define STATIC_EXPECT_FALSE(...) EXPECT_EQ((std::integral_constant<decltype(__VA_ARGS__), __VA_ARGS__>::value), false)

/**
 * \brief Helper to check if 2 types are the same, enabling gtest output
 * \param val1 The first value to compare
 * \param val2 The second value to compare
 */
#define STATIC_EXPECT_SAME_TYPE(...) STATIC_EXPECT_TRUE(std::is_same_v<__VA_ARGS__>)

namespace spark::mpl::testing
{
    TEST(TypelistsShould, haveAWorkingSizeAttribute)
    {
        STATIC_EXPECT_EQ((spark::mpl::typelist<>::size), 0);
        STATIC_EXPECT_EQ((spark::mpl::typelist<int>::size), 1);
        STATIC_EXPECT_EQ((spark::mpl::typelist<int, float, double>::size), 3);
    }

    TEST(TypelistsShould, haveAWorkingEmptyAttribute)
    {
        STATIC_EXPECT_TRUE(spark::mpl::typelist<>::empty);
        STATIC_EXPECT_FALSE(spark::mpl::typelist<int>::empty);
    }

    TEST(TypelistsShould, haveAWorkingContainsAttribute)
    {
        STATIC_EXPECT_TRUE(spark::mpl::typelist<int>::contains<int>);
        STATIC_EXPECT_TRUE(spark::mpl::typelist<int, float, double>::contains<int>);
        STATIC_EXPECT_TRUE(spark::mpl::typelist<int, float, double>::contains<float>);
        STATIC_EXPECT_TRUE(spark::mpl::typelist<int, float, double>::contains<double>);
        STATIC_EXPECT_FALSE(spark::mpl::typelist<int>::contains<char>);
        STATIC_EXPECT_FALSE(spark::mpl::typelist<>::contains<int>);
    }

    TEST(TypelistsShould, haveAWorkingIndexOfAttribute)
    {
        STATIC_EXPECT_EQ((spark::mpl::typelist<int, float, char>::index_of<int>), 0);
        STATIC_EXPECT_EQ((spark::mpl::typelist<int, float, char>::index_of<float>), 1);
        STATIC_EXPECT_EQ((spark::mpl::typelist<int, float, char>::index_of<char>), 2);
    }

    TEST(TypelistsShould, haveAWorkingFrontMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::front, int);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<char, double>::front, char);
    }

    TEST(TypelistsShould, haveAWorkingBackMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<char>::back, char);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<char, double>::back, double);
    }

    TEST(TypelistsShould, haveAWorkingPushFrontMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<>::push_front<char>, spark::mpl::typelist<char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float>::push_front<char>, spark::mpl::typelist<char, int, float>);
    }

    TEST(TypelistsShould, haveAWorkingPushBackMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<>::push_back<char>, spark::mpl::typelist<char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float>::push_back<char>, spark::mpl::typelist<int, float, char>);
    }

    TEST(TypelistsShould, haveAWorkingPopFrontMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::pop_front, spark::mpl::typelist<double, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::pop_front, spark::mpl::typelist<>);
    }

    TEST(TypelistsShould, haveAWorkingPopBackMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::pop_back, spark::mpl::typelist<int, double>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::pop_back, spark::mpl::typelist<>);
    }

    TEST(TypelistsShould, haveAWorkingAtMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::at<0>, int);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float>::at<1>, float);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float, char>::at<1>, float);
    }

    TEST(TypelistsShould, haveAWorkingClearMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<>::clear, spark::mpl::typelist<>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::clear, spark::mpl::typelist<>);
    }

    TEST(TypelistsShould, haveAWorkingEraseMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::erase<int>, spark::mpl::typelist<double, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::erase<double>, spark::mpl::typelist<int, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::erase<char>, spark::mpl::typelist<int, double>);
    }

    TEST(TypelistsShould, haveAWorkingEraseAtMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float, char>::erase_at<0>, spark::mpl::typelist<float, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float, char>::erase_at<1>, spark::mpl::typelist<int, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float, char>::erase_at<2>, spark::mpl::typelist<int, float>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<double>::erase_at<0>, spark::mpl::typelist<>);
    }

    TEST(TypelistsShould, haveAWorkingInsertAtMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::insert_at<0, void>, spark::mpl::typelist<void, int>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float>::insert_at<1, void>, spark::mpl::typelist<int, void, float>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, float>::insert_at<5, void>, spark::mpl::typelist<int, float, void>);
    }

    TEST(TypelistsShould, haveAWorkingReplaceMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::replace<int, void>, spark::mpl::typelist<void, double, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double>::replace<double, void>, spark::mpl::typelist<int, void>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::replace<int, void>, spark::mpl::typelist<void>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::replace<char, void>, spark::mpl::typelist<int>);
    }

    TEST(TypelistsShould, haveAWorkingReplaceAtMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::replace_at<0, void>, spark::mpl::typelist<void, double, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double>::replace_at<1, void>, spark::mpl::typelist<int, void>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::replace_at<0, void>, spark::mpl::typelist<void>);
    }

    TEST(TypelistsShould, haveAWorkingReverseMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::reverse, spark::mpl::typelist<int>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::reverse, spark::mpl::typelist<char, double, int>);
    }

    TEST(TypelistsShould, haveAWorkingFilterMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char, float>::filter<std::is_integral>, spark::mpl::typelist<int, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int>::filter<std::is_floating_point>, spark::mpl::typelist<>);
    }

    TEST(TypelistsShould, haveAWorkingConvertMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::convert<std::tuple>, std::tuple<int, double, char>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<>::convert<std::tuple>, std::tuple<>);
    }

    TEST(TypelistsShould, haveAWorkingTransformMethod)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist<int, double, char>::transform<std::add_pointer>, spark::mpl::typelist<int*, double*, char*>);
    }

    TEST(TypelistsShould, beConcatenable)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist_concat<spark::mpl::typelist<int, float>, spark::mpl::typelist<>>, spark::mpl::typelist<int, float>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist_concat<spark::mpl::typelist<int, double>, spark::mpl::typelist<void>>, spark::mpl::typelist<int, double, void>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist_concat<spark::mpl::typelist<int>, spark::mpl::typelist<void, float>>, spark::mpl::typelist<int, void, float>);
    }

    TEST(TypelistsShould, beFlattenable)
    {
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist_concat<spark::mpl::typelist<int>, spark::mpl::typelist<>, spark::mpl::typelist<long>>, spark::mpl::typelist<int, long>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist_concat<spark::mpl::typelist<int>>, spark::mpl::typelist<int>);
        STATIC_EXPECT_SAME_TYPE(spark::mpl::typelist_concat<spark::mpl::typelist<>>, spark::mpl::typelist<>);
    }

    TEST(TypelistsShould, beMatchable)
    {
        EXPECT_TRUE((spark::mpl::typelist_match<std::is_same, spark::mpl::typelist<int, float>, spark::mpl::typelist<int, float>>));
        EXPECT_FALSE((spark::mpl::typelist_match<std::is_same, spark::mpl::typelist<int, float>, spark::mpl::typelist<int, double>>));
        EXPECT_TRUE((spark::mpl::typelist_match<std::is_same, spark::mpl::typelist<>, spark::mpl::typelist<>>));
    }
}
