#pragma once

#include <cstddef>

namespace spark::patterns
{
    template <typename... Args>
    class Signal;

    template <typename... Args>
    class Slot;
}

namespace spark::patterns::details
{
    template <typename... Args>
    class Connection final
    {
        friend class ::spark::patterns::Slot<Args...>;
        friend class ::spark::patterns::Signal<Args...>;

    public:
        explicit Connection(Signal<Args...>* signal, Slot<Args...>* slot, const std::size_t slot_key, const bool managed)
            : m_signal(signal), m_slot(slot), m_slotKey(slot_key), m_managed(managed) {}

        void releaseSlot()
        {
            m_slot->m_connection = nullptr;
            if (m_managed)
                delete m_slot;
        }

    private:
        Signal<Args...>* m_signal;
        Slot<Args...>* m_slot;
        std::size_t m_slotKey;
        bool m_managed = false;
    };
}
