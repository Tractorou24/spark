#pragma once

namespace spark::patterns
{
    template <typename... Args>
    Slot<Args...>::Slot()
        : m_callback() { }

    template <typename... Args>
    Slot<Args...>::Slot(const std::function<void(Args...)>& f)
        : m_callback(f) { }

    template <typename... Args>
    Slot<Args...>::Slot(std::function<void(Args...)>&& f)
        : m_callback(f) { }

    template <typename... Args>
    template <class T>
    Slot<Args...>::Slot(T* target, void (T::*method)(Args...))
    {
        m_callback = [target, method](Args... args)
        {
            (target->*method)(args...);
        };
    }

    template <typename... Args>
    template <class T>
    Slot<Args...>::Slot(T* target, void (T::*method)(Args...) const)
    {
        m_callback = [target, method](Args... args)
        {
            (target->*method)(args...);
        };
    }

    template <typename... Args>
    Slot<Args...>::~Slot()
    {
        disconnect();
    }

    template <typename... Args>
    Slot<Args...>::Slot(const Slot& slot)
        : m_callback(slot.m_callback) { }

    template <typename... Args>
    Slot<Args...>::Slot(Slot&& slot) noexcept
    {
        move(&slot);
    }

    template <typename... Args>
    Slot<Args...>& Slot<Args...>::operator=(const Slot& slot)
    {
        if (this == &slot)
            return *this;

        m_callback = slot.m_callback;
        return *this;
    }

    template <typename... Args>
    Slot<Args...>& Slot<Args...>::operator=(Slot&& slot) noexcept
    {
        disconnect();
        move(&slot);
        return *this;
    }

    template <typename... Args>
    void Slot<Args...>::disconnect()
    {
        if (m_connection != nullptr)
            m_connection->m_signal->disconnect(this);
    }

    template <typename... Args>
    void Slot<Args...>::move(Slot* slot)
    {
        m_callback = std::move(slot->m_callback);
        m_connection = nullptr;

        if (slot->m_connection != nullptr)
        {
            m_connection = slot->m_connection;
            slot->m_connection->releaseSlot();
            m_connection->m_slot = this;
            m_connection->m_managed = false;
        }
    }
}
