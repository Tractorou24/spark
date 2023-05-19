#pragma once

#include "spark/events/Event.h"
#include "spark/events/Export.h"

namespace spark::events
{
    /**
     * \brief A class that dispatches events to the appropriate event handler.
     */
    class SPARK_EVENTS_EXPORT EventDispatcher final
    {
    public:
        /**
         * \brief Instantiates a new EventDispatcher for the given event.
         * \param event the event to dispatch.
         */
        explicit EventDispatcher(Event& event);
        ~EventDispatcher() = default;

        EventDispatcher(const EventDispatcher& other) = delete;
        EventDispatcher(EventDispatcher&& other) noexcept = delete;
        EventDispatcher& operator=(const EventDispatcher& other) = delete;
        EventDispatcher& operator=(EventDispatcher&& other) noexcept = delete;

        /**
         * \brief Dispatches the event to the appropriate event handler.
         * \tparam T The type of the event to dispatch.
         * \tparam F The type of the event handler (automatically deduced).
         * \param func The event handler to call.
         * \return A boolean indicating whether the event was dispatched or not.
         */
        template <typename T, typename F>
        [[nodiscard]] bool dispatch(const F& func);

    private:
        Event& m_event;
    };
}

#include "spark/events/impl/EventDispatcher.h"
