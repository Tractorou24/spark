#pragma once

#include "spark/events/Export.h"

#include "spark/base/Macros.h"
#include "spark/rtti/HasRtti.h"

namespace spark::events
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None                     = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define DEFINE_EVENT_TYPE(type) public:                                                                   \
                                    static EventType StaticType() { return type; }                        \
                                    virtual EventType eventType() const override { return StaticType(); } \
                                private:

#define DEFINE_EVENT_CATEGORY(category) public:                                                                  \
                                            virtual int eventCategoryFlags() const override { return category; } \
                                        private:

    /**
     * \brief A base class for all events in SPARK.
     */
    class SPARK_EVENTS_EXPORT Event : public rtti::HasRtti
    {
        DECLARE_SPARK_RTTI(Event)

    public:
        explicit Event() = default;
        ~Event() override = default;

        Event(const Event& other) = default;
        Event(Event&& other) noexcept = default;
        Event& operator=(const Event& other) = default;
        Event& operator=(Event&& other) noexcept = default;

        /**
         * \brief Gets the type of the event.
         * \return An EventType enum value corresponding to the type of the event.
         */
        [[nodiscard]] virtual EventType eventType() const = 0;

        /**
         * \brief Gets the category flags of the event.
         * \return An int combining the enum values of the corresponding categories of the event.
         */
        [[nodiscard]] virtual int eventCategoryFlags() const = 0;

        /**
         * \brief Checks if the event is in the specified category.
         * \param category The category to check.
         * \return A boolean value indicating whether the event is in the specified category.
         */
        [[nodiscard]] bool isInCategory(const EventCategory& category) const;

    public:
        bool handled = false;
    };
}

IMPLEMENT_SPARK_RTTI(spark::events::Event)
