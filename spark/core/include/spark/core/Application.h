#pragma once

#include "spark/core/Export.h"
#include "spark/core/Registries.h"
#include "spark/core/Window.h"

#include <filesystem>
#include <string>

namespace spark::core
{
    class SceneManager;
    class Scene;

    /**
     * \brief A class representing a SPARK application.
     */
    class SPARK_CORE_EXPORT Application final
    {
        friend class SceneManager;

        template <typename... Tags>
        friend class ApplicationBuilder;

    public:
        /**
         * \brief A struct containing the settings for the application.
         */
        struct Settings
        {
            std::string name;
            spark::math::Vector2<unsigned int> size;
            bool resizable = false;
        };

        /**
         * \brief A struct containing all the registries for the application.
         */
        struct Registries
        {
            core::GameObjectRegistry gameObject;
            core::ComponentRegistry component;
        };

        /**
         * \brief Gets the instance of the application.
         * \return A pointer to the instance of the application singleton.
         */
        static Application* Instance();

    public:
        ~Application() = default;

        Application(const Application& other) = delete;
        Application(Application&& other) noexcept = default;
        Application& operator=(const Application& other) = delete;
        Application& operator=(Application&& other) noexcept = default;

        /**
         * \brief Runs the application.
         */
        void run();

        /**
         * \brief Closes the application.
         */
        void close();

        /**
         * \brief Gets the settings for the application.
         * \return A \ref spark::core::Application::Settings struct containing the settings for the application.
         */
        [[nodiscard]] Settings settings() const;

        /**
         * \brief Gets the window for the application.
         * \return A reference to the \link spark::core::Window \endlink for the application.
         */
        [[nodiscard]] Window& window();

        /**
         * \brief Sets the scene to render.
         * \param scene A shared pointer to the \link spark::core::Scene \endlink to render.
         */
        void setScene(std::shared_ptr<core::Scene> scene);

        /**
         * \brief Gets the scene to render.
         * \return A \link spark::core::Scene \endlink to render.
         */
        [[nodiscard]] core::Scene& scene();

        /**
         * \brief Gets a reference to the registries for the application.
         * \return A \ref Registries struct containing all the registries for the application.
         */
        [[nodiscard]] Registries& registries();

    private:
        /**
         * \brief Instantiates a new application with the given settings.
         * \param settings The \ref spark::core::Application::Settings for the application.
         */
        explicit Application(const Settings& settings);

        /**
         * \brief Dispatches an event to the application.
         * \param event A reference to the \link spark::events::Event \endlink to dispatch.
         */
        void onEvent(events::Event& event);

    private:
        static Application* s_instance;

    private:
        std::unique_ptr<Window> m_window;
        std::shared_ptr<core::Scene> m_scene;
        Settings m_settings;
        Registries m_registries;
        bool m_isRunning = true;
    };
}
