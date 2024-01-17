#pragma once

#include "spark/render/Export.h"
#include "spark/render/GraphicsAdapter.h"
#include "spark/render/GraphicsDevice.h"

#include <optional>

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

    /**
     * \brief Describes a rendering backend. (backend with type \ref BackendType::Rendering)
     */
    class SPARK_RENDER_EXPORT IRenderBackend : public IBackend
    {
    public:
        ~IRenderBackend() noexcept override = default;

        /// \copydoc IBackend::type()
        [[nodiscard]] BackendType type() const noexcept override { return BackendType::Rendering; }

        /**
         * \brief Lists all available graphics adapters
         * \return A \ref std::vector containing all available graphics adapters.
         */
        [[nodiscard]] std::vector<const IGraphicsAdapter*> adapters() const noexcept { return genericAdapters(); }

        /**
         * \brief Finds a graphics adapter by unique id.
         * \param id The unique id of the graphics adapter to find. If `std::nullopt`, the returned adapter is implementation-defined.
         * \return A pointer to the graphics adapter if found, `nullptr` otherwise.
         */
        SPARK_SUPPRESS_MSVC_WARNING(4324) // 'std::optional': structure was padded due to alignment specifier
        [[nodiscard]] virtual const IGraphicsAdapter* findAdapter(const std::optional<lib::Uuid>& id = std::nullopt) const = 0;

        /**
         * \brief Searches for a graphics device by name.
         * \param name The name of the graphics device to find.
         * \return A pointer to the graphics device if found, `nullptr` otherwise.
         */
        [[nodiscard]] virtual IGraphicsDevice* device(const std::string& name) noexcept = 0;

        /// \copydoc IRenderBackend::device()
        [[nodiscard]] virtual const IGraphicsDevice* device(const std::string& name) const noexcept = 0;

    private:
        /// \brief Private method used to allow replacement of the generic method by a custom type.
        [[nodiscard]] virtual std::vector<const IGraphicsAdapter*> genericAdapters() const noexcept = 0;
    };
}
