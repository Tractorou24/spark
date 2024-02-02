#include "spark/events/KeyEvents.h"

namespace spark::events
{
    base::KeyCode KeyEvent::keyCode() const noexcept
    {
        return key;
    }

    KeyEvent::KeyEvent(const base::KeyCode key) noexcept
        : key(key) {}

    KeyPressedEvent::KeyPressedEvent(const base::KeyCode key, const bool is_repeat) noexcept
        : KeyEvent(key), m_isRepeat(is_repeat) {}

    bool KeyPressedEvent::isRepeat() const noexcept
    {
        return m_isRepeat;
    }

    KeyReleasedEvent::KeyReleasedEvent(const base::KeyCode key) noexcept
        : KeyEvent(key) {}

    KeyTypedEvent::KeyTypedEvent(const base::KeyCode key) noexcept
        : KeyEvent(key) {}
}
