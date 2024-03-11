#include "spark/core/Application.h"
#include "spark/core/Input.h"
#include "spark/core/Scene.h"
#include "spark/core/Window.h"

#include "spark/events/EventDispatcher.h"
#include "spark/events/KeyEvents.h"
#include "spark/events/MouseEvents.h"
#include "spark/events/WindowEvents.h"
#include "spark/lib/Clock.h"
#include "spark/log/Logger.h"

namespace spark::core
{
    Application* Application::s_instance = nullptr;

    Application* Application::Instance()
    {
        return s_instance;
    }

    Application::Application(const Settings& settings)
    {
        const Window::Settings window_settings =
        {
            .title = settings.name,
            .size = settings.size,
            .eventCallback = [this](events::Event& event) { onEvent(event); }
        };

        m_window = std::make_unique<Window>(window_settings);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Application::run()
    {
        lib::Clock update_timer;
        while (m_isRunning)
        {
            const float dt = update_timer.restart<std::chrono::seconds>();
            // Update
            m_window->onUpdate();
            m_scene->onUpdate(dt);

            // Render
            m_scene->onRender();
            m_window->renderer().render();
        }

        // Unload scene and close window (app is not running anymore, close() was called)
        if (m_scene)
            m_scene->onUnload();
        m_window->close();
    }

    void Application::close()
    {
        m_isRunning = false;
        log::info("Closing application");
    }

    Application::Settings Application::settings() const
    {
        return m_settings;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    Window& Application::window()
    {
        return *m_window;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    core::Scene& Application::scene()
    {
        return *m_scene;
    }

    Application::Registries& Application::registries()
    {
        return m_registries;
    }

    void Application::setScene(std::shared_ptr<core::Scene> scene)
    {
        if (m_scene)
            m_scene->onUnload();
        m_scene = std::move(scene);
        m_scene->onLoad();
    }

    void Application::onEvent(events::Event& event)
    {
        auto dispatcher = events::EventDispatcher(event);

        bool result = false;
        result |= dispatcher.dispatch<events::WindowCloseEvent>([this](events::WindowCloseEvent&)
        {
            close();
            return true;
        });
        result |= dispatcher.dispatch<events::MouseMovedEvent>([](events::MouseMovedEvent&)
        {
            Input::mouseMovedEvent.emit();
            return true;
        });
        result |= dispatcher.dispatch<events::KeyPressedEvent>([](const events::KeyPressedEvent& e)
        {
            try
            {
                Input::keyPressedEvents.at(e.keyCode()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Key {} is not registered in the keyPressedEvents map", e.keyCode());
                return false;
            }
        });
        result |= dispatcher.dispatch<events::KeyReleasedEvent>([](const events::KeyReleasedEvent& e)
        {
            try
            {
                Input::keyReleasedEvents.at(e.keyCode()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Key {} is not registered in the keyReleasedEvents map", e.keyCode());
                return false;
            }
        });
        result |= dispatcher.dispatch<events::MouseButtonPressedEvent>([](const events::MouseButtonEvent& e)
        {
            try
            {
                Input::mousePressedEvents.at(e.button()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Mouse button {} is not registered", e.button());
                return false;
            }
        });
        result |= dispatcher.dispatch<events::MouseButtonReleasedEvent>([](const events::MouseButtonEvent& e)
        {
            try
            {
                Input::mouseReleasedEvents.at(e.button()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Mouse button {} is not registered", e.button());
                return false;
            }
        });

        if (!result)
            log::warning("Failed to dispatch event {}", event.rttiInstance().className());
    }
}
