#pragma once

#include "spark/events/Event.h"
#include "spark/events/Export.h"

#include "spark/base/KeyCodes.h"

namespace spark::events
{
    /**
     * \brief A base class for all key events.
     */
    class SPARK_EVENTS_EXPORT KeyEvent : public Event
    {
        DECLARE_SPARK_RTTI(KeyEvent, Event)
        DEFINE_EVENT_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard)

    public:
        /**
         * \brief Gets the key code of the key that triggered this event.
         * \return The key code of the key that triggered this event.
         */
        [[nodiscard]] base::KeyCode keyCode() const noexcept;

    protected:
        explicit KeyEvent(base::KeyCode key) noexcept;

        base::KeyCode key;
    };

    /**
     * \brief A class that represents a key pressed event.
     */
    class SPARK_EVENTS_EXPORT KeyPressedEvent final : public KeyEvent
    {
        DECLARE_SPARK_RTTI(KeyPressedEvent, KeyEvent)
        DEFINE_EVENT_TYPE(EventType::KeyPressed)

    public:
        /**
         * \brief Instantiates a new KeyPressedEvent for the given key.
         * \param key The pressed key.
         * \param is_repeat The repeat flag.
         */
        explicit KeyPressedEvent(base::KeyCode key, bool is_repeat = false) noexcept;

        /**
         * \brief Gets the repeat flag.
         * \return A boolean indicating whether the key is being held down.
         */
        [[nodiscard]] bool isRepeat() const noexcept;

    private:
        bool m_isRepeat;
    };

    /**
     * \brief A class that represents a key released event.
     */
    class SPARK_EVENTS_EXPORT KeyReleasedEvent final : public KeyEvent
    {
        DECLARE_SPARK_RTTI(KeyReleasedEvent, KeyEvent)
        DEFINE_EVENT_TYPE(EventType::KeyReleased)

    public:
        /**
         * \brief Instantiates a new KeyReleasedEvent for the given key.
         * \param key The released key.
         */
        explicit KeyReleasedEvent(base::KeyCode key) noexcept;
    };

    /**
     * \brief A class that represents a key typed event.
     */
    class SPARK_EVENTS_EXPORT KeyTypedEvent final : public KeyEvent
    {
        DECLARE_SPARK_RTTI(KeyTypedEvent, KeyEvent)
        DEFINE_EVENT_TYPE(EventType::KeyTyped)

    public:
        /**
         * \brief Instantiates a new KeyTypedEvent for the given key.
         * \param key The typed key.
         */
        explicit KeyTypedEvent(base::KeyCode key) noexcept;
    };
}

IMPLEMENT_SPARK_RTTI(spark::events::KeyEvent)

IMPLEMENT_SPARK_RTTI(spark::events::KeyPressedEvent)

IMPLEMENT_SPARK_RTTI(spark::events::KeyReleasedEvent)

IMPLEMENT_SPARK_RTTI(spark::events::KeyTypedEvent)
