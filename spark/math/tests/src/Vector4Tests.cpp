#include "spark/math/Vector4.h"

#include "gtest/gtest.h"

#include <cmath>

namespace spark::math::testing
{
    TEST(Vector4Should, beDefaultConstructible)
    {
        const Vector4<int> v;
        EXPECT_EQ(v.w, 0);
        EXPECT_EQ(v.x, 0);
        EXPECT_EQ(v.y, 0);
        EXPECT_EQ(v.z, 0);
    }

    TEST(Vector4Should, beConstructibleWithValues)
    {
        const Vector4<int> v(1, 2, 3, 4);
        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
        EXPECT_EQ(v.w, 4);
    }

    TEST(Vector4Should, beCopyConstructible)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(v);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }

    TEST(Vector4Should, beMoveConstructible)
    {
        Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(std::move(v));
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }

    TEST(Vector4Should, beCopyAssignable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2 = v;
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }

    TEST(Vector4Should, beMoveAssignable)
    {
        Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2 = std::move(v);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }

    TEST(Vector4Should, beSelfAssignable)
    {
        Vector4<int> v(1, 2, 3, 4);
        const auto& v_ref = v;

        v = v_ref;

        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
        EXPECT_EQ(v.w, 4);
    }

    TEST(Vector4Should, beSelfMoveAssignable)
    {
        Vector4<int> v(1, 2, 3, 4);
        auto& v_ref = v;

        v = std::move(v_ref);

        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
        EXPECT_EQ(v.w, 4);
    }

    TEST(Vector4Should, beEquallyComparable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(1, 2, 3, 4);
        EXPECT_EQ(v, v2);
    }

    TEST(Vector4Should, notBeEquallyComparable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(10, 11, 12, 15);
        EXPECT_NE(v, v2);
    }

    TEST(Vector4Should, beSummable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(4, 3, 2, 1);
        const Vector4<int> v3 = v + v2;
        EXPECT_EQ(v3.x, 5);
        EXPECT_EQ(v3.y, 5);
        EXPECT_EQ(v3.z, 5);
        EXPECT_EQ(v3.w, 5);
    }

    TEST(Vector4Should, beSubtractable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(4, 3, 2, 1);
        const Vector4<int> v3 = v - v2;
        EXPECT_EQ(v3.x, -3);
        EXPECT_EQ(v3.y, -1);
        EXPECT_EQ(v3.z, 1);
        EXPECT_EQ(v3.w, 3);
    }

    TEST(Vector4Should, beMultipliable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2(4, 3, 2, 1);
        const Vector4<int> v3 = v * v2;
        EXPECT_EQ(v3.x, 4);
        EXPECT_EQ(v3.y, 6);
        EXPECT_EQ(v3.z, 6);
        EXPECT_EQ(v3.w, 4);
    }

    TEST(Vector4Should, beDivisible)
    {
        const Vector4<int> v(9, 4, 2, 3);
        const Vector4<int> v2(3, 2, 2, 1);
        const Vector4<int> v3 = v / v2;
        EXPECT_EQ(v3.x, 3);
        EXPECT_EQ(v3.y, 2);
        EXPECT_EQ(v3.z, 1);
        EXPECT_EQ(v3.w, 3);
    }

    TEST(Vector4Should, beMultipliableByValue)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2 = v * 2;
        EXPECT_EQ(v2.x, 2);
        EXPECT_EQ(v2.y, 4);
        EXPECT_EQ(v2.z, 6);
        EXPECT_EQ(v2.w, 8);
    }

    TEST(Vector4Should, beDivisibleByValue)
    {
        const Vector4<int> v(2, 4, 6, 8);
        const Vector4<int> v2 = v / 2;
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }

    TEST(Vector4Should, beNegatable)
    {
        const Vector4<int> v(1, 2, 3, 4);
        const Vector4<int> v2 = -v;
        EXPECT_EQ(v2.x, -1);
        EXPECT_EQ(v2.y, -2);
        EXPECT_EQ(v2.z, -3);
        EXPECT_EQ(v2.w, -4);
    }

    TEST(Vector4Should, computeNorm)
    {
        const Vector4<float> v(1, 2, 3, 4);
        const auto norm = v.norm();
        EXPECT_FLOAT_EQ(norm, std::sqrt(1 + 4 + 9 + 16.f));
    }

    TEST(Vector4Should, beSwappable)
    {
        Vector4<int> v(1, 2, 3, 4);
        Vector4<int> v2(4, 3, 2, 1);

        swap(v, v2);

        EXPECT_EQ(v.x, 4);
        EXPECT_EQ(v.y, 3);
        EXPECT_EQ(v.z, 2);
        EXPECT_EQ(v.w, 1);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }

    TEST(Vector4Should, beCastableToOtherType)
    {
        const Vector4<float> v(1.5f, 2.5f, 3.5f, 4.5f);
        const Vector4<int> v2 = v.castTo<int>();
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
        EXPECT_EQ(v2.w, 4);
    }
}
