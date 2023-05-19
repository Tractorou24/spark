#pragma once

namespace spark::events
{
    template <typename T, typename F>
    bool EventDispatcher::dispatch(const F& func)
    {
        if (m_event.getEventType() == T::GetStaticType())
        {
            m_event.handled = func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }
}
