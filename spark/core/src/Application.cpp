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

#include "imgui.h"

#include <numeric>

namespace
{
    /**
     * \brief Draws a graph of the frames per second using ImGui
     * \param dt The delta time between the last frame and the current one
     */
    void draw_fps_graph([[maybe_unused]] const float dt)
    {
#ifndef SPARK_RELEASE
        static std::vector<float> fps_values(100);
        static float min_fps = 1000.0f;
        static float max_fps = 0.0f;
        static float avg_fps = 0.0f;

        const float fps = 1.0f / dt;
        max_fps = std::max(max_fps, fps);
        min_fps = std::min(min_fps, fps);
        avg_fps = std::accumulate(fps_values.begin(), fps_values.end(), 0.f) / 100;
        fps_values.push_back(fps);

        if (fps_values.size() >= 100)
            fps_values.erase(fps_values.begin());

        ImGui::Begin("Frame Data");
        ImGui::Text("FPS: %.2f", fps);
        ImGui::Text("Max FPS: %.2f", max_fps);
        ImGui::Text("Min FPS: %.2f", min_fps);
        ImGui::Text("Avg FPS: %.2f", avg_fps);
        ImGui::PlotLines("##FPS", fps_values.data(), static_cast<int>(fps_values.size()), 0, nullptr, 0.0f, 3000.f, ImVec2(0, 80));
        ImGui::End();
#endif
    }
}

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
            imgui::new_frame();

            // Update
            m_window->onUpdate();
            if (m_window->isMinimized())
                continue;

            m_scene->onUpdate(dt);

            // Render
            draw_fps_graph(dt);
            m_scene->onRender();
            m_window->renderer().render();

            details::GameObjectDeleter<GameObject>::DeleteMarkedObjects();
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
        if (m_scene)
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
