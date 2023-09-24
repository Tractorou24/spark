#pragma once

#include "spark/patterns/Slot.h"

#include <algorithm>
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
    void Signal<Args...>::operator()(Args&&... args) const
    {
        emit(std::forward<Args>(args)...);
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
        auto it = std::ranges::find_if(m_connections, [key](const auto& connection) { return connection.second.m_slotKey == key; });
        if (it != m_connections.end())
        {
            it->second.releaseSlot();
            m_connections.erase(it);
        }
    }

    template <typename... Args>
    void Signal<Args...>::disconnect(Slot<Args...>* slot)
    {
        if (slot && slot->isConnected() && slot->m_connection->m_signal == this)
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

    template<typename... Args>
    bool Signal<Args...>::isConnected(const std::size_t key) const
    {
        const auto keys = getConnectedKeys();
        return std::ranges::find(keys, key) != keys.end();
    }

    template <typename... Args>
    std::vector<std::size_t> Signal<Args...>::getConnectedKeys() const
    {
        std::vector<std::size_t> keys;
        keys.reserve(m_connections.size());
        for (const auto& connection : m_connections | std::views::values)
            keys.push_back(connection.m_slotKey);
        return keys;
    }

    template <typename... Args>
    std::vector<const Slot<Args...>*> Signal<Args...>::getConnectedSlots() const
    {
        std::vector<const Slot<Args...>*> slots;
        for (const auto& connection : m_connections | std::views::values)
            slots.push_back(connection.m_slot);
        return slots;
    }

    template <typename... Args>
    void Signal<Args...>::emit(Args&&... args) const
    {
        /*
         * When emitting, we need to make sure that:
         * - the program does not crash if a slot is destroyed during the emit
         * - the program does not crash if the object that emits the signal is destroyed during the emit
         *
         * So, we copy the connections into a vector, and then iterate over the vector to avoid any iterator invalidation and check if the connection still exists.
         */

        auto keys_view = m_connections | std::views::keys;
        std::vector<std::size_t> keys = { keys_view.begin(), keys_view.end() };

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
