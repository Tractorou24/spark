#include "spark/events/EventDispatcher.h"

namespace spark::events
{
    EventDispatcher::EventDispatcher(Event& event)
	    : m_event(event)
    {
    }
}
