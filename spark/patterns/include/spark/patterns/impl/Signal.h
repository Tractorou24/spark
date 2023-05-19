#pragma once

#include "spark/patterns/Slot.h"

#include <ranges>

namespace spark::patterns
{
    template <typename... Args>
    Signal<Args...>::Signal()
        : m_connections(), m_sequence(0) { }

    template <typename... Args>
    Signal<Args...>::Signal(Signal&& signal) noexcept
    {
        move(&signal);
    }

    template <typename... Args>
    Signal<Args...>::~Signal()
    {
        clear();
    }

    template <typename... Args>
    Signal<Args...>& Signal<Args...>::operator=(Signal&& signal) noexcept
    {
        move(&signal);
        return *this;
    }

    template <typename... Args>
    void Signal<Args...>::operator()(Args... args) const
    {
        emit(args...);
    }

    template <typename... Args>
    std::size_t Signal<Args...>::connect(Slot<Args...>* slot)
    {
        return connect(slot, false);
    }

    template <typename... Args>
    std::size_t Signal<Args...>::connect(Slot<Args...>& slot)
    {
        return connect(&slot, false);
    }

    template <typename... Args>
    std::size_t Signal<Args...>::connect(Slot<Args...>&& slot)
    {
        return connect(new Slot<Args...>(std::move(slot)), true);
    }

    template <typename... Args>
    std::size_t Signal<Args...>::connect(const std::function<void(Args...)>& callback)
    {
        return connect(new Slot<Args...>(callback), true);
    }

    template <typename... Args>
    std::size_t Signal<Args...>::connect(std::function<void(Args...)>&& callback)
    {
        return connect(new Slot<Args...>(std::move(callback)), true);
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(std::size_t key)
    {
        if (auto it = m_connections.find(key); it != m_connections.end())
        {
            it->second.releaseSlot();
            m_connections.erase(it);
        }
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(Slot<Args...>* slot)
    {
        if (slot && slot->m_connection && slot->m_connection->m_signal == this)
            disconnect(slot->m_connection->m_slotKey);
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(Slot<Args...>& slot)
    {
        disconnect(&slot);
    }

    template <typename... Args>
    void Signal<Args...>::clear()
    {
        for (auto& connection : m_connections | std::views::values)
            connection.releaseSlot();
        m_connections.clear();
    }

    template <typename... Args>
    void Signal<Args...>::emit(Args... args) const
    {
        /*
         * When emitting, we need to make sure that:
         * - the program does not crash if a slot is destroyed during the emit
         * - the program does not crash if the object that emits the signal is destroyed during the emit
         *
         * So, we copy the connections into a vector, and then iterate over the vector to avoid any iterator invalidation and check if the connection still exists.
         */

        std::vector<std::size_t> keys;
        keys.reserve(m_connections.size());
        std::ranges::transform(m_connections, std::back_inserter(keys), [](const auto& connection) { return connection.first; });

        for (const auto& key : keys)
        {
            if (m_connections.contains(key))
            {
                const auto& connection = m_connections.at(key);
                if (connection.m_slot->m_callback)
                    m_connections.at(key).m_slot->m_callback(std::forward<Args>(args)...);
            }
        }
    }

    template <typename... Args>
    std::size_t Signal<Args...>::connect(Slot<Args...>* slot, bool managed)
    {
        if (!slot)
            return 0;

        if (slot->m_connection)
        {
            if (slot->m_connection->m_signal == this)
                return slot->m_connection->m_slotKey;
            slot->disconnect();
        }

        const std::size_t key = ++m_sequence;
        auto res = m_connections.insert({key - 1, details::Connection(this, slot, key, managed)});
        slot->m_connection = &res.first->second;
        return m_sequence;
    }

    template <typename... Args>
    void Signal<Args...>::move(Signal* signal)
    {
        clear();
        m_connections = std::move(signal->m_connections);
        m_sequence = signal->m_sequence;

        for (auto& connection : m_connections | std::views::values)
            connection.m_signal = this;

        signal->m_sequence = 0;
        signal->m_connections.clear();
    }
}
