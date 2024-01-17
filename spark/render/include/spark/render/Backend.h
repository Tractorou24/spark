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

    /**
     * \brief Defines a back-end, that provides a device instance for a certain surface and graphics adapter.
     * \tparam GraphicsDeviceType The type of the graphics device. (inherits from \ref IGraphicsDevice)
     */
    template <typename GraphicsDeviceType>
    class RenderBackend : public IRenderBackend
    {
    public:
        using device_type = GraphicsDeviceType;
        using surface_type = typename device_type::surface_type;
        using adapter_type = typename device_type::adapter_type;
        using swap_chain_type = typename device_type::swap_chain_type;
        using command_queue_type = typename device_type::command_queue_type;
        using command_buffer_type = typename device_type::command_buffer_type;
        using frame_buffer_type = typename device_type::frame_buffer_type;
        using render_pass_type = typename device_type::render_pass_type;
        using pipeline_layout_type = typename device_type::pipeline_layout_type;
        using render_pipeline_type = typename device_type::render_pipeline_type;
        using shader_program_type = typename device_type::shader_program_type;
        using input_assembler_type = typename device_type::input_assembler_type;
        using rasterizer_type = typename device_type::rasterizer_type;

    public:
        ~RenderBackend() noexcept override = default;

        [[nodiscard]] virtual std::vector<const adapter_type*> adapters() const noexcept = 0;

        /// \copydoc IRenderBackend::findAdapter()
        [[nodiscard]] const adapter_type* findAdapter(const std::optional<lib::Uuid>& id = std::nullopt) const override = 0;

        /// \copydoc IRenderBackend::device()
        [[nodiscard]] device_type* device(const std::string& name) noexcept override = 0;

        /// \copydoc IRenderBackend::device()
        [[nodiscard]] const device_type* device(const std::string& name) const noexcept override = 0;

        /**
         * \brief Registers a new graphics device.
         * \param name The name of the device.
         * \param device A \ref std::unique_ptr to the device to register.
         * \see RenderBackend::releaseDevice()
         */
        virtual void registerDevice(std::string name, std::unique_ptr<device_type>&& device) = 0;

        /**
         * \brief Unregisters a graphics device.
         * \param name The name of the device to unregister. This name must match the name used when registering the device.
         */
        virtual void releaseDevice(const std::string& name) = 0;

        /**
         * \brief Creates a new graphics device.
         * \tparam Self The real type of the backend. (inheriting from \ref RenderBackend)
         * \tparam Args The types of the arguments to pass to the device constructor.
         * \param name The name of the device to create.
         * \param adapter The adapter to use for the device.
         * \param surface The surface to use for the device.
         * \param args The remaining arguments to pass to the device constructor.
         * \return A pointer to the newly created device.
         */
        template <typename Self, typename... Args>
        device_type* createDevice(std::string name, const adapter_type& adapter, std::unique_ptr<surface_type>&& surface, Args&&... args)
        {
            auto device = std::make_unique<device_type>(adapter, std::move(surface), std::forward<Args>(args)...);
            auto* ptr = device.get();
            static_cast<Self*>(this)->registerDevice(name, std::move(device));
            return ptr;
        }

    private:
        [[nodiscard]] std::vector<const IGraphicsAdapter*> genericAdapters() const noexcept final
        {
            auto tmp = adapters();
            std::vector<const IGraphicsAdapter*> result;
            result.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(result), [](const auto& adapter) { return static_cast<const IGraphicsAdapter*>(adapter); });
            return result;
        }
    };
}
