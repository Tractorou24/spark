#include "spark/core/platforms/SfmlWindow.h"
#include "spark/core/Application.h"
#include "spark/core/Window.h"

#include "spark/events/KeyEvents.h"
#include "spark/events/MouseEvents.h"
#include "spark/events/WindowEvents.h"
#include "spark/log/Logger.h"

#include "SFML/Window/Event.hpp"

namespace spark::core
{
    std::unique_ptr<Window> Window::Create(const WindowSpecification& settings)
    {
        return std::make_unique<SfmlWindow>(settings);
    }

    SfmlWindow::SfmlWindow(const WindowSpecification& settings)
        : m_data {settings.title, settings.width, settings.height, nullptr}
    {
        log::info(R"(Creating window "{}" ({}px * {}px))", m_data.title, m_data.width, m_data.height);
        m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_data.width, m_data.height), m_data.title);
    }

    void SfmlWindow::close()
    {
        log::info("Closing window");
        m_window->clear();
        m_window->close();
    }

    void SfmlWindow::onUpdate()
    {
        sf::Event event = {};
        while (m_window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                {
                    events::WindowCloseEvent window_close_event;
                    m_data.eventCallback(window_close_event);
                    m_window->close();
                    break;
                }
            case sf::Event::Resized:
                {
                    events::WindowResizeEvent window_resize_event(event.size.width, event.size.height);
                    m_data.eventCallback(window_resize_event);
                    m_data.width = event.size.width;
                    m_data.height = event.size.height;

                    sf::View view = m_window->getView();
                    view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                    m_window->setView(view);
                    break;
                }
            case sf::Event::KeyPressed:
                {
                    events::KeyPressedEvent key_pressed_event(static_cast<base::KeyCode>(event.key.code));
                    m_data.eventCallback(key_pressed_event);
                    break;
                }
            case sf::Event::KeyReleased:
                {
                    events::KeyReleasedEvent key_released_event(static_cast<base::KeyCode>(event.key.code));
                    m_data.eventCallback(key_released_event);
                    break;
                }
            case sf::Event::MouseWheelScrolled:
                {
                    events::MouseScrolledEvent mouse_scrolled_event(event.mouseWheelScroll.delta);
                    m_data.eventCallback(mouse_scrolled_event);
                    break;
                }
            case sf::Event::MouseButtonPressed:
                {
                    events::MouseButtonPressedEvent mouse_button_pressed_event(static_cast<base::MouseCode>(event.mouseButton.button));
                    m_data.eventCallback(mouse_button_pressed_event);
                    break;
                }
            case sf::Event::MouseButtonReleased:
                {
                    events::MouseButtonReleasedEvent mouse_button_released_event(static_cast<base::MouseCode>(event.mouseButton.button));
                    m_data.eventCallback(mouse_button_released_event);
                    break;
                }
            case sf::Event::MouseMoved:
                {
                    events::MouseMovedEvent mouse_moved_event(event.mouseMove.x, event.mouseMove.y);
                    m_data.eventCallback(mouse_moved_event);
                    break;
                }
            default:
                {
                    log::warning("Unhandled event type: {}", static_cast<int>(event.type));
                    break;
                }
            }
        }
    }

    void SfmlWindow::onRender()
    {
        m_window->clear();
        Render2D<engine::Scene>::exec(Application::Instance()->scene());
        m_window->display();
    }

    uint32_t SfmlWindow::width() const
    {
        return m_data.width;
    }

    uint32_t SfmlWindow::height() const
    {
        return m_data.height;
    }

    math::Vector2<uint32_t> SfmlWindow::size() const
    {
        return {m_data.width, m_data.height};
    }

    void* SfmlWindow::nativeWindow() const
    {
        return m_window.get();
    }

    void SfmlWindow::setEventCallback(const std::function<void(events::Event&)>& callback)
    {
        m_data.eventCallback = callback;
    }
}
