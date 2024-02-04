#include "spark/lib/Pointers.h"

#include "gtest/gtest.h"

namespace spark::lib::testing
{
    struct Base
    {
        virtual ~Base() = default;

        int i = 2;
    };

    struct Derived final : Base
    {
        float f = 3.2f;
    };

    TEST(AsSharedShould, correctlyConvertValidPointer)
    {
        // Given a unique_ptr to a valid piece of memory
        auto old = std::make_unique<int>(2);

        // When converting to a shared_ptr
        const auto ptr = lib::as_shared(std::move(old));

        // Then, the new pointer have the same value and the old one is null.
        EXPECT_EQ(*ptr, 2);
        EXPECT_EQ(old, nullptr);
    }

    TEST(AsSharedShould, returnANullSharedPointerWhenSourceIsNull)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<int> old = nullptr;

        // When converting to a shared_ptr
        const auto ptr = lib::as_shared(std::move(old));

        // Then, the new pointer is also null and no crashes happened.
        EXPECT_EQ(ptr, nullptr);
    }

    TEST(StaticUniquePointerCastShould, correctlyCastAPointer)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<Base> old = std::make_unique<Derived>();

        // When converting to a shared_ptr
        const auto ptr = lib::static_unique_pointer_cast<Derived>(std::move(old));

        // Then, the new pointer have the same value and the old one is null.
        EXPECT_EQ(ptr->i, 2);
        EXPECT_EQ(ptr->f, 3.2f);
        EXPECT_EQ(old, nullptr);
    }

    TEST(StaticUniquePointerCastShould, returnANullPointerWhenSourceIsNull)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<Base> old = nullptr;

        // When converting to a shared_ptr
        const auto ptr = lib::static_unique_pointer_cast<Derived>(std::move(old));

        // Then, the new pointer is also null and no crashes happened.
        EXPECT_EQ(ptr, nullptr);
    }

    TEST(ReinterpretUniquePointerCastShould, correctlyCastAPointer)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<Base> old = std::make_unique<Derived>();

        // When converting to a shared_ptr
        const auto ptr = lib::reinterpret_unique_pointer_cast<Derived>(std::move(old));

        // Then, the new pointer have the same value and the old one is null.
        EXPECT_EQ(ptr->i, 2);
        EXPECT_EQ(ptr->f, 3.2f);
        EXPECT_EQ(old, nullptr);
    }

    TEST(ReinterpretUniquePointerCastShould, returnANullPointerWhenSourceIsNull)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<Base> old = nullptr;

        // When converting to a shared_ptr
        const auto ptr = lib::reinterpret_unique_pointer_cast<Derived>(std::move(old));

        // Then, the new pointer is also null and no crashes happened.
        EXPECT_EQ(ptr, nullptr);
    }

    TEST(DynamicUniquePointerCastShould, correctlyCastAPointer)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<Base> old = std::make_unique<Derived>();

        // When converting to a shared_ptr
        const auto ptr = lib::dynamic_unique_pointer_cast<Derived>(std::move(old));

        // Then, the new pointer have the same value and the old one is null.
        EXPECT_EQ(ptr->i, 2);
        EXPECT_EQ(ptr->f, 3.2f);
        EXPECT_EQ(old, nullptr);
    }

    TEST(DynamicUniquePointerCastShould, returnANullPointerWhenSourceIsNull)
    {
        // Given a unique_ptr to a valid piece of memory
        std::unique_ptr<Base> old = nullptr;

        // When converting to a shared_ptr
        const auto ptr = lib::dynamic_unique_pointer_cast<Derived>(std::move(old));

        // Then, the new pointer is also null and no crashes happened.
        EXPECT_EQ(ptr, nullptr);
    }
}
