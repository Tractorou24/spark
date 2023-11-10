#include "spark/core/Application.h"
#include "spark/core/Input.h"
#include "spark/core/Window.h"

#include "spark/base/Macros.h"
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

    Application::Application(ApplicationSpecification settings)
        : m_specification(std::move(settings))
    {
        WindowSpecification window_settings;
        window_settings.title = m_specification.name;

        m_window = Window::Create(window_settings);
        m_window->setEventCallback([this](events::Event& e) { onEvent(e); });
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
            m_window->onRender();
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

    ApplicationSpecification Application::getSettings() const
    {
        return m_specification;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    Window& Application::getWindow()
    {
        return *m_window;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    engine::Scene& Application::getScene()
    {
        return *m_scene;
    }

    void Application::setScene(std::shared_ptr<engine::Scene> scene)
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
                Input::keyPressedEvents.at(e.getKeyCode()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Key {} is not registered in the keyPressedEvents map", e.getKeyCode());
                return false;
            }
        });
        result |= dispatcher.dispatch<events::KeyReleasedEvent>([](const events::KeyReleasedEvent& e)
        {
            try
            {
                Input::keyReleasedEvents.at(e.getKeyCode()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Key {} is not registered in the keyReleasedEvents map", e.getKeyCode());
                return false;
            }
        });
        result |= dispatcher.dispatch<events::MouseButtonPressedEvent>([](const events::MouseButtonEvent& e)
        {
            try
            {
                Input::mousePressedEvents.at(e.getMouseButton()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Mouse button {} is not registered", e.getMouseButton());
                return false;
            }
        });
        result |= dispatcher.dispatch<events::MouseButtonReleasedEvent>([](const events::MouseButtonEvent& e)
        {
            try
            {
                Input::mouseReleasedEvents.at(e.getMouseButton()).emit();
                return true;
            } catch (const std::out_of_range&)
            {
                log::error("Mouse button {} is not registered", e.getMouseButton());
                return false;
            }
        });

        if (!result)
            log::warning("Failed to dispatch event {}", event.getRttiInstance().getClassName());
    }

    std::unique_ptr<Application> make_application(ApplicationSpecification settings)
    {
        if (Application::Instance())
            throw spark::base::DuplicatedApplicationException("There is already an instance of Application");

        auto app = std::unique_ptr<Application>(new Application(std::move(settings)));
        app->s_instance = app.get();
        return app;
    }
}
