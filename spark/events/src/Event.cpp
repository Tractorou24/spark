#include "spark/events/Event.h"

namespace spark::events
{
    bool Event::isInCategory(const EventCategory& category) const
    {
        return getEventCategoryFlags() & category;
    }
}
