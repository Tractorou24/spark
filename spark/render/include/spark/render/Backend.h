#pragma once

#include "spark/render/Export.h"

namespace spark::core
{
    class Application;
}

namespace spark::render
{
    /**
     * \brief Describes the type of a backend.
     */
    enum class BackendType
    {
        Rendering = 0x01,
        Other     = 0xFF
    };

    /**
     * \brief The base class for a backend.
     */
    class SPARK_RENDER_EXPORT IBackend
    {
        friend class spark::core::Application;

    public:
        virtual ~IBackend() noexcept = default;

        /**
         * \brief Gets the type of the backend.
         * \return A \ref BackendType value describing the type of the backend.
         */
        [[nodiscard]] virtual BackendType type() const noexcept = 0;
    };
}
