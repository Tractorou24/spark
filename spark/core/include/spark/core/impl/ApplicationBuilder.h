#pragma once

#include "spark/base/Exception.h"

#include "spark/mpl/typelist.h"

namespace spark::core
{
    template <typename... Tags>
    ApplicationBuilder<Tags...>::ApplicationBuilder()
    {
        if (Application::Instance())
            throw spark::base::DuplicatedApplicationException("There is already an instance of Application");
    }

    template <typename... Tags>
    ApplicationBuilder<Tags...>::ApplicationBuilder(Application::Settings settings)
        : m_settings(std::move(settings)) {}

    template <typename... Tags>
    ApplicationBuilder<details::application_tags::set_name_called, Tags...> ApplicationBuilder<Tags...>::setName(std::string name)
    {
        static_assert(!spark::mpl::typelist<Tags...>::template contains<details::application_tags::set_name_called>, "Cannot set application name twice.");

        m_settings.name = std::move(name);
        return ApplicationBuilder<details::application_tags::set_name_called, Tags...>(std::move(m_settings));
    }

    template <typename... Tags>
    ApplicationBuilder<details::application_tags::set_size_called, Tags...> ApplicationBuilder<Tags...>::setSize(unsigned width, unsigned height)
    {
        static_assert(!spark::mpl::typelist<Tags...>::template contains<details::application_tags::set_size_called>, "Cannot set window size twice.");

        m_settings.size.x = width;
        m_settings.size.y = height;
        return ApplicationBuilder<details::application_tags::set_size_called, Tags...>(std::move(m_settings));
    }

    template <typename... Tags>
    std::unique_ptr<Application> ApplicationBuilder<Tags...>::build()
    {
        static_assert(spark::mpl::typelist<Tags...>::template contains<details::application_tags::set_name_called>, "Cannot build application without setting the name.");
        static_assert(spark::mpl::typelist<Tags...>::template contains<details::application_tags::set_size_called>, "Cannot build application without setting the window size.");

        auto app = std::unique_ptr<Application>(new Application(m_settings));
        app->s_instance = app.get();
        return app;
    }
}
