#include "spark/core/platforms/SfmlWindow.h"
#include "spark/core/Application.h"
#include "spark/core/Window.h"

#include "spark/base/MouseCodes.h"
#include "spark/events/KeyEvents.h"
#include "spark/events/MouseEvents.h"
#include "spark/events/WindowEvents.h"
#include "spark/log/Logger.h"

#include "SFML/Window/Event.hpp"

namespace spark::core
{
    std::unique_ptr<Window> Window::Create(const Window::Settings& settings)
    {
        return std::make_unique<SfmlWindow>(settings);
    }

    SfmlWindow::SfmlWindow(const Window::Settings& settings)
        : m_settings(settings)
    {
        log::info(R"(Creating window "{}" ({}px * {}px))", settings.title, settings.size.x, settings.size.y);
        m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(settings.size.x, settings.size.y), settings.title);
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
                    m_settings.eventCallback(window_close_event);
                    m_window->close();
                    break;
                }
            case sf::Event::Resized:
                {
                    events::WindowResizeEvent window_resize_event(event.size.width, event.size.height);
                    m_settings.eventCallback(window_resize_event);
                    m_settings.size.x = event.size.width;
                    m_settings.size.y = event.size.height;

                    sf::View view = m_window->getView();
                    view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                    m_window->setView(view);
                    break;
                }
            case sf::Event::KeyPressed:
                {
                    events::KeyPressedEvent key_pressed_event(static_cast<base::KeyCode>(event.key.code));
                    m_settings.eventCallback(key_pressed_event);
                    break;
                }
            case sf::Event::KeyReleased:
                {
                    events::KeyReleasedEvent key_released_event(static_cast<base::KeyCode>(event.key.code));
                    m_settings.eventCallback(key_released_event);
                    break;
                }
            case sf::Event::MouseWheelScrolled:
                {
                    events::MouseScrolledEvent mouse_scrolled_event(event.mouseWheelScroll.delta);
                    m_settings.eventCallback(mouse_scrolled_event);
                    break;
                }
            case sf::Event::MouseButtonPressed:
                {
                    events::MouseButtonPressedEvent mouse_button_pressed_event(static_cast<base::MouseCode>(event.mouseButton.button));
                    m_settings.eventCallback(mouse_button_pressed_event);
                    break;
                }
            case sf::Event::MouseButtonReleased:
                {
                    events::MouseButtonReleasedEvent mouse_button_released_event(static_cast<base::MouseCode>(event.mouseButton.button));
                    m_settings.eventCallback(mouse_button_released_event);
                    break;
                }
            case sf::Event::MouseMoved:
                {
                    events::MouseMovedEvent mouse_moved_event(event.mouseMove.x, event.mouseMove.y);
                    m_settings.eventCallback(mouse_moved_event);
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

    math::Vector2<unsigned int> SfmlWindow::size() const
    {
        return m_settings.size;
    }

    void* SfmlWindow::nativeWindow() const
    {
        return m_window.get();
    }
}
