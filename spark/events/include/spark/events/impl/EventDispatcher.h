#pragma once

namespace spark::events
{
    EventDispatcher::EventDispatcher(Event& event)
        : m_event(event) {}

    template <typename T, typename F>
    bool EventDispatcher::dispatch(const F& func)
    {
        if (m_event.eventType() == T::StaticType())
        {
            m_event.handled = func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }
}
