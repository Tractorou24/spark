#include "gtest/gtest.h"

#include "spark/patterns/Signal.h"
#include "spark/patterns/Slot.h"

#include "spark/base/Macros.h"

namespace spark::patterns::testing
{
    TEST(SignalShould, defaultConstruct)
    {
        // If a signal is default constructed, it should be empty
        Signal<> signal;
    }

    TEST(SignalShould, connectFromASlot)
    {
        // Given signal and slot 
        Slot<> slot;
        Signal<> signal;

        // When connecting the slot to the signal
        signal.connect(slot);

        // Then, it should be connected
        EXPECT_TRUE(slot.isConnected());
    }

    TEST(SignalShould, connectFromAFunction)
    {
        // Given a std::function
        const std::function f = [](const int i) { SPARK_UNUSED(i); };
        Signal<int> signal;

        // When connecting the function to a signal
        const std::size_t key = signal.connect(f);

        // Then, it should be connected
        EXPECT_TRUE(signal.isConnected(key));
    }

    TEST(SignalShould, connectFromALambda)
    {
        // Given a signal
        Signal<> signal;

        // When connecting a lambda to the signal
        const std::size_t key = signal.connect([] { });

        // Then, it should be connected
        EXPECT_TRUE(signal.isConnected(key));
    }

    TEST(SignalShould, disconnectFromAGivenSlot)
    {
        // Given a signal and a slot connected to it
        Slot<> slot;
        Signal<> signal;
        signal.connect(slot);

        // When disconnecting the slot from the signal
        signal.disconnect(slot);

        // Then, it should be disconnected
        EXPECT_FALSE(slot.isConnected());
    }

    TEST(SignalShould, disconnectFromAGivenKey)
    {
        // Given a signal and a slot connected to it
        Slot<> slot;
        Signal<> signal;
        const std::size_t key = signal.connect(slot);

        // When disconnecting the slot from the signal
        signal.disconnect(key);

        // Then, it should be disconnected
        EXPECT_FALSE(slot.isConnected());
    }

    TEST(SignalShould, disconnectAllSlots)
    {
        // Given a signal and a slot connected to it
        Slot<> slot, slot2;
        Signal<> signal;
        signal.connect(slot);
        signal.connect(slot2);

        // When disconnecting all slots from the signal
        signal.clear();

        // Then, it should be disconnected
        EXPECT_FALSE(slot.isConnected());
        EXPECT_FALSE(slot2.isConnected());
    }

    TEST(SignalShould, emitSignalWithEmit)
    {
        // Given a signal and a slot connected to it
        bool called = false;
        Signal<> signal;
        signal.connect([&called] { called = true; });

        // When emitting the signal
        signal.emit();

        // Then, the slot should be called
        EXPECT_TRUE(called);
    }

    TEST(SignalShould, emitSignalWithOperator)
    {
        // Given a signal and a slot connected to it
        bool called = false;
        Signal<> signal;
        signal.connect([&called] { called = true; });

        // When emitting the signal
        signal();

        // Then, the slot should be called
        EXPECT_TRUE(called);
    }

    TEST(SignalShould, perfectlyForwardArguments)
    {
        // Given a signal and a slot connected to it
        int val = 0;
        Signal<int&> signal;
        signal.connect([](int& i) { i++; });

        // When emitting the signal
        signal.emit(val);

        // Then, the slot should be called
        EXPECT_EQ(val, 1);
    }

    TEST(SignalShould, returnConnectedSlotKeys)
    {
        // Given a signal and slots connected to it
        Signal<> signal;
        Slot<> slot, slot2;
        const std::size_t key1 = signal.connect(slot);
        const std::size_t key2 = signal.connect(slot2);

        // When getting the connected slot keys
        const auto keys = signal.connectedKeys();

        // Then, the keys should be returned
        EXPECT_TRUE(std::ranges::find(keys, key1) != keys.end());
        EXPECT_TRUE(std::ranges::find(keys, key2) != keys.end());
    }

    TEST(SignalShould, returnConnectedSlots)
    {
        // Given a signal and slots connected to it
        Signal<> signal;
        Slot<> slot, slot2;
        signal.connect(slot);
        signal.connect(slot2);

        // When getting the connected slots
        const auto slots = signal.connectedSlots();

        // Then, the slots should be returned
        EXPECT_TRUE(std::ranges::find(slots, &slot) != slots.end());
        EXPECT_TRUE(std::ranges::find(slots, &slot2) != slots.end());
    }
}
