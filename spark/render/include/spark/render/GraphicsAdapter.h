#pragma once

#include "spark/render/Export.h"

#include "spark/lib/Uuid.h"

#include <format>
#include <string>

namespace spark::render
{
    /**
     * \brief Defines the different graphics adapters that can be recognized by the system.
     */
    enum class GraphicsAdapterType
    {
        /// \brief The adapter is invalid.
        None = 0x00000000,

        /// \brief The adapter is a GPU.
        GPU = 0x00000001,

        /// \brief The adapter is a CPU with integrated graphics.
        CPU = 0x00000002,

        /// \brief The adapter type is not captured. Used to mark adapters we should ignore.
        Other = 0x7FFFFFFF
    };

    /**
     * \brief Represents a physical graphics adapter.
     *
     * A graphics adapter is a physical device that can be used to render graphics. It typically is a GPU, but can also be a CPU with integrated graphics.
     * However, it can also represent an emulated graphics adapter.
     */
    class SPARK_RENDER_EXPORT IGraphicsAdapter
    {
    public:
        virtual ~IGraphicsAdapter() noexcept = default;

        /**
         * \brief Gets the name of the graphics adapter.
         * \return A \ref std::string containing the name of the graphics adapter.
         */
        [[nodiscard]] virtual std::string name() const noexcept = 0;

        /**
         * \brief Gets the unique identifier for the graphics adapter in the system.
         * \return A \ref lib::Uuid containing the unique identifier for the graphics adapter.
         */
        [[nodiscard]] virtual lib::Uuid uuid() const noexcept = 0;

        /**
         * \brief Gets an identifier representing the vendor of the graphics adapter.
         * \return A uint32 representing the vendor of the graphics adapter.
         */
        [[nodiscard]] virtual unsigned int vendorId() const noexcept = 0;

        /**
         * \brief Gets an identifier representing the product.
         * \return A uint32 representing the product.
         */
        [[nodiscard]] virtual unsigned int deviceId() const noexcept = 0;

        /**
         * \brief Gets the type of the graphics adapter.
         * \return A \ref GraphicsAdapter representing the type of the graphics adapter.
         */
        [[nodiscard]] virtual GraphicsAdapterType type() const noexcept = 0;

        /**
         * \brief Gets the version of the driver for the graphics adapter.
         * \return A uint32 representing the version of the driver for the graphics adapter.
         */
        [[nodiscard]] virtual unsigned int driverVersion() const noexcept = 0;

        /**
         * \brief Gets the version of the API supported by the graphics adapter.
         * \return A uint32 representing the version of the API supported by the graphics adapter.
         */
        [[nodiscard]] virtual unsigned int apiVersion() const noexcept = 0;

        /**
         * \brief Gets the amount of dedicated video memory (in bytes) available on the graphics adapter.
         * \return A uint64 representing the amount of dedicated video memory (in bytes) available on the graphics adapter.
         */
        [[nodiscard]] virtual unsigned long long int dedicatedVideoMemory() const noexcept = 0;
    };
};

template <>
struct std::formatter<spark::render::GraphicsAdapterType>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::GraphicsAdapterType adapter, auto& ctx)
    {
        switch (adapter) // NOLINT(clang-diagnostic-switch-enum): Other is handled after the switch statement.
        {
        case spark::render::GraphicsAdapterType::None:
            return std::format_to(ctx.out(), "None");
        case spark::render::GraphicsAdapterType::GPU:
            return std::format_to(ctx.out(), "Gpu");
        case spark::render::GraphicsAdapterType::CPU:
            return std::format_to(ctx.out(), "Cpu");
        default:
            break;
        }
        return std::format_to(ctx.out(), "Other");
    }
};
