#include "spark/math/Vector2.h"

#include "gtest/gtest.h"

#include <cmath>

namespace spark::math::testing
{
    TEST(Vector2Should, beDefaultConstructible)
    {
        const Vector2<int> v;
        EXPECT_EQ(v.x, 0);
        EXPECT_EQ(v.y, 0);
    }

    TEST(Vector2Should, beConstructibleWithValues)
    {
        const Vector2<int> v(1, 2);
        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
    }

    TEST(Vector2Should, beCopyConstructible)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(v);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
    }

    TEST(Vector2Should, beMoveConstructible)
    {
        Vector2<int> v(1, 2);
        const Vector2<int> v2(std::move(v));
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
    }

    TEST(Vector2Should, beCopyAssignable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2 = v;
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
    }

    TEST(Vector2Should, beMoveAssignable)
    {
        Vector2<int> v(1, 2);
        const Vector2<int> v2 = std::move(v);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
    }

    TEST(Vector2Should, beSelfAssignable)
    {
        Vector2<int> v(1, 2);
        const auto& v_ref = v;

        v = v_ref;

        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
    }

    TEST(Vector2Should, beSelfMoveAssignable)
    {
        Vector2<int> v(1, 2);
        auto& v_ref = v;

        v = std::move(v_ref);

        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
    }

    TEST(Vector2Should, beEquallyComparable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(1, 2);
        EXPECT_EQ(v, v2);
    }

    TEST(Vector2Should, notBeEquallyComparable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(2, 1);
        EXPECT_NE(v, v2);
    }

    TEST(Vector2Should, beSummable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(2, 1);
        const Vector2<int> v3 = v + v2;
        EXPECT_EQ(v3.x, 3);
        EXPECT_EQ(v3.y, 3);
    }

    TEST(Vector2Should, beSubtractable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(2, 1);
        const Vector2<int> v3 = v - v2;
        EXPECT_EQ(v3.x, -1);
        EXPECT_EQ(v3.y, 1);
    }

    TEST(Vector2Should, beMultipliable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(2, 1);
        const Vector2<int> v3 = v * v2;
        EXPECT_EQ(v3.x, 2);
        EXPECT_EQ(v3.y, 2);
    }

    TEST(Vector2Should, beDivisible)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2(2, 1);
        const Vector2<int> v3 = v / v2;
        EXPECT_EQ(v3.x, 0);
        EXPECT_EQ(v3.y, 2);
    }

    TEST(Vector2Should, beMultipliableByValue)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2 = v * 2;
        EXPECT_EQ(v2.x, 2);
        EXPECT_EQ(v2.y, 4);
    }

    TEST(Vector2Should, beDivisibleByValue)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2 = v / 2;
        EXPECT_EQ(v2.x, 0);
        EXPECT_EQ(v2.y, 1);
    }

    TEST(Vector2Should, beNegatable)
    {
        const Vector2<int> v(1, 2);
        const Vector2<int> v2 = -v;
        EXPECT_EQ(v2.x, -1);
        EXPECT_EQ(v2.y, -2);
    }

    TEST(Vector2Should, computeNorm)
    {
        const Vector2<float> v(3, 4);
        const auto norm = v.norm();
        EXPECT_FLOAT_EQ(norm, std::sqrt(3 * 3 + 4 * 4.f));
    }

    TEST(Vector2Should, beSwappable)
    {
        Vector2<int> v(1, 2);
        Vector2<int> v2(2, 1);

        swap(v, v2);

        EXPECT_EQ(v.x, 2);
        EXPECT_EQ(v.y, 1);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
    }

    TEST(Vector2Should, beCastableToOtherType)
    {
        const Vector2<float> v(1.5f, 2.5f);
        const Vector2<int> v2 = v.castTo<int>();
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
    }
}
