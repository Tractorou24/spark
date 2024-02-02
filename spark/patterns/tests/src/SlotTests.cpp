#include "gtest/gtest.h"

#include "spark/patterns/Signal.h"
#include "spark/patterns/Slot.h"

#include "spark/base/Macros.h"

namespace spark::patterns::testing
{
    TEST(SlotShould, defaultConstruct)
    {
        // If a slot is default constructed, it should be empty
        const Slot<> slot;
    }

    TEST(SlotShould, constructFromAFunction)
    {
        // Given a std::function with and without arguments
        const std::function f = [] {};
        const std::function f2 = [](const int i) { SPARK_UNUSED(i); };

        // When constructing a slot, then it should not throw
        EXPECT_NO_THROW(auto slot = Slot(f));
        EXPECT_NO_THROW(auto slot2 = Slot(f2));
    }

    TEST(SlotShould, constructFromALambda)
    {
        // When constructing a slot from a lambda, then it should not throw
        EXPECT_NO_THROW(auto slot = Slot<>([] { }));
        EXPECT_NO_THROW(auto slot2 = Slot<int>([](const int i) { SPARK_UNUSED(i); }));
    }

    TEST(SlotShould, constructFromAMemberFunction)
    {
        struct S
        {
            static void F() {}
            static void F2(const int i) { SPARK_UNUSED(i); }
        };

        // When constructing a slot from a member function, then it should not throw
        EXPECT_NO_THROW(auto slot = Slot<>(S::F));
        EXPECT_NO_THROW(auto slot2 = Slot<int>(S::F2));
    }

    TEST(SlotShould, connect)
    {
        // Given a signal and a slot
        Signal<> signal;
        Slot<> slot([] {});

        // When connecting the slot to the signal
        signal.connect(slot);

        // Then the slot should be connected
        EXPECT_TRUE(slot.isConnected());
    }

    TEST(SlotShould, disconnect)
    {
        // Given a slot connected to a signal
        Signal<> signal;
        const auto slot = std::make_unique<Slot<>>([] {});
        signal.connect(slot.get());

        // When disconnecting the slot
        slot->disconnect();

        // Then the slot should be disconnected
        EXPECT_FALSE(slot->isConnected());
    }
}
