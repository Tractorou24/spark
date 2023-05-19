#include "spark/events/MouseEvents.h"

namespace spark::events
{
    MouseMovedEvent::MouseMovedEvent(const int x, const int y)
        : m_mouseX(x), m_mouseY(y) { }

    int MouseMovedEvent::getX() const
    {
        return m_mouseX;
    }

    int MouseMovedEvent::getY() const
    {
        return m_mouseY;
    }

    MouseScrolledEvent::MouseScrolledEvent(const float offset)
        : m_offset(offset) { }

    float MouseScrolledEvent::getOffset() const
    {
        return m_offset;
    }

    base::MouseCode MouseButtonEvent::getMouseButton() const
    {
        return m_button;
    }

    MouseButtonEvent::MouseButtonEvent(const base::MouseCode button)
        : m_button(button) { }

    MouseButtonPressedEvent::MouseButtonPressedEvent(const base::MouseCode button)
        : MouseButtonEvent(button) { }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(const base::MouseCode button)
        : MouseButtonEvent(button) { }
}
