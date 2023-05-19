#pragma once

#include "spark/events/Event.h"
#include "spark/events/Export.h"

#include "spark/base/MouseCodes.h"

namespace spark::events
{
    /**
     * \brief A class representing a mouse moved event.
     */
    class SPARK_EVENTS_EXPORT MouseMovedEvent final : public Event
    {
        DECLARE_SPARK_RTTI(MouseMovedEvent, Event)
        DEFINE_EVENT_TYPE(EventType::MouseMoved)
        DEFINE_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

    public:
        /**
         * \brief Instantiates a new MouseMovedEvent for the given mouse position.
         * \param x The x position of the mouse.
         * \param y The y position of the mouse.
         */
        explicit MouseMovedEvent(int x, int y);

        /**
         * \brief Gets the x position of the mouse.
         * \return An int representing the x position of the mouse.
         */
        [[nodiscard]] int getX() const;

        /**
         * \brief Gets the y position of the mouse.
         * \return An int representing the y position of the mouse.
         */
        [[nodiscard]] int getY() const;

    private:
        int m_mouseX, m_mouseY;
    };

    /**
     * \brief A class representing a mouse scrolled event.
     */
    class SPARK_EVENTS_EXPORT MouseScrolledEvent final : public Event
    {
        DECLARE_SPARK_RTTI(MouseScrolledEvent, Event)
        DEFINE_EVENT_TYPE(EventType::MouseScrolled)
        DEFINE_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

    public:
        /**
         * \brief Instantiates a new MouseScrolledEvent for the given offsets.
         * \param offset The offset of the mouse wheel from the previous position.
         */
        explicit MouseScrolledEvent(float offset);

        /**
         * \brief Gets the scroll offset.
         * \return A float representing the scroll offset from the previous position.
         */
        [[nodiscard]] float getOffset() const;

    private:
        float m_offset;
    };

    /**
     * \brief A base class for all mouse button events.
     */
    class SPARK_EVENTS_EXPORT MouseButtonEvent : public Event
    {
        DECLARE_SPARK_RTTI(MouseButtonEvent, Event)
        DEFINE_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

    public:
        /**
         * \brief Gets the mouse button that triggered this event.
         * \return A spark::base::MouseCode representing the mouse button that triggered this event.
         */
        [[nodiscard]] base::MouseCode getMouseButton() const;

    protected:
        explicit MouseButtonEvent(base::MouseCode button);

    private:
        base::MouseCode m_button;
    };

    /**
     * \brief A class representing a mouse button pressed event.
     */
    class SPARK_EVENTS_EXPORT MouseButtonPressedEvent final : public MouseButtonEvent
    {
        DECLARE_SPARK_RTTI(MouseButtonPressedEvent, MouseButtonEvent)
        DEFINE_EVENT_TYPE(EventType::MouseButtonPressed)

    public:
        /**
         * \brief Instantiates a new MouseButtonPressedEvent for the given mouse button.
         * \param button The pressed mouse button.
         */
        explicit MouseButtonPressedEvent(base::MouseCode button);
    };

    /**
     * \brief A class representing a mouse button released event.
     */
    class SPARK_EVENTS_EXPORT MouseButtonReleasedEvent final : public MouseButtonEvent
    {
        DECLARE_SPARK_RTTI(MouseButtonReleasedEvent, MouseButtonEvent)
        DEFINE_EVENT_TYPE(EventType::MouseButtonReleased)

    public:
        /**
         * \brief Instantiates a new MouseButtonReleasedEvent for the given mouse button.
         * \param button The released mouse button.
         */
        explicit MouseButtonReleasedEvent(base::MouseCode button);
    };
}

IMPLEMENT_SPARK_RTTI(spark::events::MouseMovedEvent)

IMPLEMENT_SPARK_RTTI(spark::events::MouseScrolledEvent)

IMPLEMENT_SPARK_RTTI(spark::events::MouseButtonEvent)

IMPLEMENT_SPARK_RTTI(spark::events::MouseButtonPressedEvent)

IMPLEMENT_SPARK_RTTI(spark::events::MouseButtonReleasedEvent)
