#include "spark/events/WindowEvents.h"

namespace spark::events
{
	WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height)
		: m_width(width), m_height(height)
	{
	}

	unsigned int WindowResizeEvent::getWidth() const
	{
		return m_width;
	}

	unsigned int WindowResizeEvent::getHeight() const
	{
		return m_height;
	}
}
