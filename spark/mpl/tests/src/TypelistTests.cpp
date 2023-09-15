#include "gtest/gtest.h"

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
