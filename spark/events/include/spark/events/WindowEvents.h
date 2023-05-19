#pragma once

#include "spark/events/Event.h"

namespace spark::events
{
	/**
	 * \brief A class representing a window resize event.
	 */
	class SPARK_EVENTS_EXPORT WindowResizeEvent final : public Event
	{
		DECLARE_SPARK_RTTI(WindowResizeEvent, Event)
		DEFINE_EVENT_TYPE(EventType::WindowResize)
		DEFINE_EVENT_CATEGORY(EventCategoryApplication)

	public:
		/**
		 * \brief Instantiates a new WindowResizeEvent for the new size.
		 * \param width The new width of the window.
		 * \param height The new height of the window.
		 */
		explicit WindowResizeEvent(unsigned int width, unsigned int height);

		/**
		 * \brief Gets the new width of the window.
		 * \return The width of the window.
		 */
		[[nodiscard]] unsigned int getWidth() const;

		/**
		 * \brief Gets the new height of the window.
		 * \return The height of the window.
		 */
		[[nodiscard]] unsigned int getHeight() const;

	private:
		unsigned int m_width, m_height;
	};

	/**
	 * \brief A class representing a window close event.
	 */
	class SPARK_EVENTS_EXPORT WindowCloseEvent final : public Event
	{
		DECLARE_SPARK_RTTI(WindowCloseEvent, Event)
		DEFINE_EVENT_TYPE(EventType::WindowClose)
		DEFINE_EVENT_CATEGORY(EventCategoryApplication)

	public:
		/**
		 * \brief Instantiates a new WindowCloseEvent.
		 */
		explicit WindowCloseEvent() = default;
	};
}

IMPLEMENT_SPARK_RTTI(spark::events::WindowResizeEvent)

IMPLEMENT_SPARK_RTTI(spark::events::WindowCloseEvent)
