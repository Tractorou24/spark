#pragma once

#include "spark/core/Application.h"

namespace spark::core
{
    namespace details::application_tags
    {
        struct set_name_called {};

        struct set_size_called {};
    }

    template <typename... Tags>
    class ApplicationBuilder
    {
        template <typename... Args>
        friend class ApplicationBuilder;

    public:
        /**
         * \brief Constructs a new builder with default settings.
         * \throws spark::base::DuplicatedApplicationException If an application was already created before.
         */
        explicit ApplicationBuilder();

        /**
         * \brief Sets the name of the application.
         * \param name A \ref std::string containing the name of the application.
         * \return A new builder used to continue building the application.
         */
        ApplicationBuilder<details::application_tags::set_name_called, Tags...> setName(std::string name);

        /**
         * \brief Sets the size of the application window.
         * \param width The width of the window.
         * \param height The height of the window.
         * \return A new builder used to continue building the application.
         */
        ApplicationBuilder<details::application_tags::set_size_called, Tags...> setSize(unsigned int width, unsigned int height);

        /**
         * \brief Builds the application with the given settings.
         * \return A \ref std::unique_ptr to the newly created application.
         */
        std::unique_ptr<Application> build();

    private:
        /// \brief Struct holding all the settings for the application.
        Application::Settings m_settings = {};

        /// \brief Constructor used internally to create a new builder with the given settings and tags.
        explicit ApplicationBuilder(Application::Settings settings);
    };
}

#include "spark/core/impl/ApplicationBuilder.h"
