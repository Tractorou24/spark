#include "spark/math/Rectangle.h"
#include "spark/math/Vector2.h"

#include "gtest/gtest.h"

namespace spark::math::testing
{
    TEST(RectangleShould, beDefaultConstructible)
    {
        const Rectangle<int> rectangle;
        EXPECT_EQ(rectangle.position, Vector2(0, 0));
        EXPECT_EQ(rectangle.extent, Vector2(0, 0));
    }

    TEST(RectangleShould, beConstructibleWithPositionAndExtent)
    {
        const Rectangle<int> rectangle({1, 2}, {3, 4});
        EXPECT_EQ(rectangle.position, Vector2(1, 2));
        EXPECT_EQ(rectangle.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beConstructibleWithPositionAndSize)
    {
        const Rectangle<int> rectangle(1, 2, 3, 4);
        EXPECT_EQ(rectangle.position, Vector2(1, 2));
        EXPECT_EQ(rectangle.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beCopyConstructible)
    {
        const Rectangle<int> rectangle({1, 2}, {3, 4});
        const Rectangle<int> copy(rectangle);
        EXPECT_EQ(copy.position, Vector2(1, 2));
        EXPECT_EQ(copy.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beMoveConstructible)
    {
        Rectangle<int> rectangle({1, 2}, {3, 4});
        const Rectangle<int> copy(std::move(rectangle));
        EXPECT_EQ(copy.position, Vector2(1, 2));
        EXPECT_EQ(copy.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beCopyAssignable)
    {
        const Rectangle<int> rectangle({1, 2}, {3, 4});
        const Rectangle<int> copy = rectangle;
        EXPECT_EQ(copy.position, Vector2(1, 2));
        EXPECT_EQ(copy.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beMoveAssignable)
    {
        Rectangle<int> rectangle({1, 2}, {3, 4});
        const Rectangle<int> copy = std::move(rectangle);
        EXPECT_EQ(copy.position, Vector2(1, 2));
        EXPECT_EQ(copy.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beSelfAssignable)
    {
        Rectangle<int> rectangle({1, 2}, {3, 4});
        const auto& ref = rectangle;

        rectangle = ref;

        EXPECT_EQ(rectangle.position, Vector2(1, 2));
        EXPECT_EQ(rectangle.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beSelfMoveAssignable)
    {
        Rectangle<int> rectangle({1, 2}, {3, 4});
        auto& ref = rectangle;

        rectangle = std::move(ref);

        EXPECT_EQ(rectangle.position, Vector2(1, 2));
        EXPECT_EQ(rectangle.extent, Vector2(3, 4));
    }

    TEST(RectangleShould, beEqualityComparable)
    {
        const Rectangle<int> rectangle({1, 2}, {3, 4});
        const Rectangle<int> rectangle2({ 1, 2 }, { 3, 4 });
        EXPECT_EQ(rectangle, rectangle2);
    }

    TEST(RectangleShould, notBeEquallyComparable)
    {
        const Rectangle<int> rectangle({1, 2}, {3, 4});
        const Rectangle<int> rectangle2({ 10, 20 }, { 30, 40 });
        EXPECT_NE(rectangle, rectangle2);
    }

    TEST(RectangleShould, beCastableToOtherTypes)
    {
        const Rectangle<float> rectangle({1.5f, 2.5f}, {3.2f, 4.6f});
        const Rectangle<int> rectangle2 = rectangle.castTo<int>();
        EXPECT_EQ(rectangle2.position, Vector2(1, 2));
        EXPECT_EQ(rectangle2.extent, Vector2(3, 4));
    }
}
