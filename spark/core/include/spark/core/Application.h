#pragma once

#include "spark/core/Export.h"
#include "spark/core/Renderer2D.h"
#include "spark/core/Window.h"

#include "spark/engine/Scene.h"
#include "spark/patterns/Singleton.h"

#include <filesystem>
#include <string>
#include <vector>

namespace spark::core
{
    class SceneManager;

    /**
     * \brief A struct containing the settings for the application.
     */
    struct ApplicationSpecification
    {
        std::string name = "Spark Application";
        std::filesystem::path workingDirectory;
        std::vector<std::string> commandLineArgs;
    };

    /**
     * \brief A class representing a SPARK application.
     */
    class SPARK_CORE_EXPORT Application final
    {
        friend class Renderer2D;
        friend class SceneManager;

    public:
        /**
         * \brief Gets the instance of the application.
         * \return A pointer to the instance of the application singleton.
         */
        static Application* Instance();

    public:
        /**
         * \brief Instantiates a new application with the given settings.
         * \param settings The \link spark::core::ApplicationSpecification \endlink for the application.
         */
        explicit Application(ApplicationSpecification settings);
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
         * \return A \link spark::core::ApplicationSpecification \endlink struct containing the settings for the application.
         */
        [[nodiscard]] ApplicationSpecification getSettings() const;

        /**
         * \brief Gets the window for the application.
         * \return A reference to the \link spark::core::Window \endlink for the application.
         */
        [[nodiscard]] Window& getWindow();

        /**
         * \brief Sets the scene to render.
         * \param scene A shared pointer to the \link spark::engine::Scene \endlink to render.
         */
        void setScene(std::shared_ptr<engine::Scene> scene);

        /**
         * \brief Gets the scene to render.
         * \return A \link spark::engine::Scene \endlink to render.
         */
        [[nodiscard]] engine::Scene& getScene();

    private:
        /**
         * \brief Dispatches an event to the application.
         * \param event A reference to the \link spark::events::Event \endlink to dispatch.
         */
        void onEvent(events::Event& event);

    private:
        static Application* s_instance;

    private:
        ApplicationSpecification m_specification;
        std::unique_ptr<Window> m_window;
        std::shared_ptr<engine::Scene> m_scene;
        bool m_isRunning = true;
    };
}
