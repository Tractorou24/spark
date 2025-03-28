#include "spark/math/Vector3.h"

#include "gtest/gtest.h"

#include <cmath>

namespace spark::math::testing
{
    TEST(Vector3Should, beDefaultConstructible)
    {
        const Vector3<int> v;
        EXPECT_EQ(v.x, 0);
        EXPECT_EQ(v.y, 0);
        EXPECT_EQ(v.z, 0);
    }

    TEST(Vector3Should, beConstructibleWithValues)
    {
        const Vector3<int> v(1, 2, 3);
        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
    }

    TEST(Vector3Should, beCopyConstructible)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(v);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }

    TEST(Vector3Should, beMoveConstructible)
    {
        Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(std::move(v));
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }

    TEST(Vector3Should, beCopyAssignable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2 = v;
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }

    TEST(Vector3Should, beMoveAssignable)
    {
        Vector3<int> v(1, 2, 3);
        const Vector3<int> v2 = std::move(v);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }

    TEST(Vector3Should, beSelfAssignable)
    {
        Vector3<int> v(1, 2, 3);
        const auto& v_ref = v;

        v = v_ref;

        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
    }

    TEST(Vector3Should, beSelfMoveAssignable)
    {
        Vector3<int> v(1, 2, 3);
        auto& v_ref = v;

        v = std::move(v_ref);

        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
    }

    TEST(Vector3Should, beEquallyComparable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(1, 2, 3);
        EXPECT_EQ(v, v2);
    }

    TEST(Vector3Should, notBeEquallyComparable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(10, 11, 12);
        EXPECT_NE(v, v2);
    }

    TEST(Vector3Should, beSummable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(3, 2, 1);
        const Vector3<int> v3 = v + v2;
        EXPECT_EQ(v3.x, 4);
        EXPECT_EQ(v3.y, 4);
        EXPECT_EQ(v3.z, 4);
    }

    TEST(Vector3Should, beSubtractable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(3, 2, 1);
        const Vector3<int> v3 = v - v2;
        EXPECT_EQ(v3.x, -2);
        EXPECT_EQ(v3.y, 0);
        EXPECT_EQ(v3.z, 2);
    }

    TEST(Vector3Should, beMultipliable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(3, 2, 1);
        const Vector3<int> v3 = v * v2;
        EXPECT_EQ(v3.x, 3);
        EXPECT_EQ(v3.y, 4);
        EXPECT_EQ(v3.z, 3);
    }

    TEST(Vector3Should, beDivisible)
    {
        const Vector3<int> v(4, 2, 3);
        const Vector3<int> v2(2, 2, 1);
        const Vector3<int> v3 = v / v2;
        EXPECT_EQ(v3.x, 2);
        EXPECT_EQ(v3.y, 1);
        EXPECT_EQ(v3.z, 3);
    }

    TEST(Vector3Should, beMultipliableByValue)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2 = v * 2;
        EXPECT_EQ(v2.x, 2);
        EXPECT_EQ(v2.y, 4);
        EXPECT_EQ(v2.z, 6);
    }

    TEST(Vector3Should, beDivisibleByValue)
    {
        const Vector3<int> v(2, 4, 6);
        const Vector3<int> v2 = v / 2;
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }

    TEST(Vector3Should, beNegatable)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2 = -v;
        EXPECT_EQ(v2.x, -1);
        EXPECT_EQ(v2.y, -2);
        EXPECT_EQ(v2.z, -3);
    }

    TEST(Vector3Should, computeNorm)
    {
        const Vector3<float> v(3, 4, 5);
        const auto norm = v.norm();
        EXPECT_FLOAT_EQ(norm, std::sqrt(50.f));
    }

    TEST(Vector3Should, computeNormalized)
    {
        const Vector3<float> v(3, 4, 5);
        const auto normalized = v.normalized();
        EXPECT_FLOAT_EQ(normalized.x, 3.f / std::sqrt(50.f));
        EXPECT_FLOAT_EQ(normalized.y, 4.f / std::sqrt(50.f));
        EXPECT_FLOAT_EQ(normalized.z, 5.f / std::sqrt(50.f));
    }

    TEST(Vecctor3Should, computeDot)
    {
        const Vector3<int> v(1, 2, 3);
        const Vector3<int> v2(3, 2, 1);
        const auto dot = v.dot(v2);
        EXPECT_EQ(dot, 10);
    }

    TEST(Vector3Should, beSwappable)
    {
        Vector3<int> v(1, 2, 3);
        Vector3<int> v2(3, 2, 1);

        swap(v, v2);

        EXPECT_EQ(v.x, 3);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 1);
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }

    TEST(Vector3Should, beCastableToOtherType)
    {
        const Vector3<float> v(1.5f, 2.5f, 3.5f);
        const Vector3<int> v2 = v.castTo<int>();
        EXPECT_EQ(v2.x, 1);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 3);
    }
}
