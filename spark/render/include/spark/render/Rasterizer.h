#pragma once

#include "spark/render/DepthStencilState.h"
#include "spark/render/Export.h"

#include <format>
#include <memory>

namespace spark::render
{
    /**
     * \brief Describes the draw mode for polygons.
     * \see IInputAssembler
     */
    enum class PolygonMode
    {
        /// \brief Polygons are drawn as solid surfaces.
        Solid = 0x00000001,

        /// \brief Polygons are only drawn as wireframes.
        WireFrame = 0x00000002,

        /// \brief Polygons are drawn as points at the vertex positions.
        Point = 0x00000004
    };

    /**
     * \brief Describes which faces are culled by the \ref Rasterizer stage.
     */
    enum class CullMode
    {
        /// \brief The rasterizer will discard front-facing polygons.
        FrontFaces = 0x00000001,

        /// \brief The rasterizer wll discard back-facing polygons.
        BackFaces = 0x00000002,

        /// \brief The rasterizer will discard front and back-facing polygons.
        Both = 0x00000004,

        /// \brief The rasterizer will not discard any polygons.
        Disabled = 0x0000000F
    };

    /**
     * \brief Describes the order or vertex winding, that is used to determine, whether a polygon is facing towards or away from the camera.
     * \see CullMode
     * \see Rasterizer
     */
    enum class CullOrder
    {
        /// \brief Vertices are evaluated in a clock-wise manner.
        ClockWise = 0x00000001,

        /// \brief Vertices are evaluated in a counter clock-wise manner.
        CounterClockWise = 0x00000002
    };

    /**
     * \brief Represents the rasterizer stage of the graphics pipeline.
     */
    class SPARK_RENDER_EXPORT IRasterizer
    {
    public:
        virtual ~IRasterizer() noexcept = default;

        /**
         * \brief Gets the polygon mode of the rasterizer state.
         * \return The \ref PolygonMode of the rasterizer state.
         */
        [[nodiscard]] virtual PolygonMode polygonMode() const noexcept = 0;

        /**
         * \brief Gets the cull mode of the rasterizer state.
         * \return The \ref CullMode of the rasterizer state.
         */
        [[nodiscard]] virtual CullMode cullMode() const noexcept = 0;

        /**
         * \brief Gets the cull order of the rasterizer state.
         * \return The \ref CullOrder of the rasterizer state.
         */
        [[nodiscard]] virtual CullOrder cullOrder() const noexcept = 0;

        /**
         * \brief Gets the line width of the rasterizer state.
         * \return A float value representing the line width of the rasterizer state.
         * \note This value is only simulated in Vulkan. Instead of this, use a custom shader.
         */
        [[nodiscard]] virtual float lineWidth() const noexcept = 0;

        /**
         * \brief Gets the depth/stencil state of the rasterizer.
         * \return The \ref DepthStencilState of the rasterizer.
         */
        [[nodiscard]] virtual const DepthStencilState& depthStencilState() const noexcept = 0;
    };

    /**
     * \brief Implements the \ref IRasterizer interface.
     */
    class SPARK_RENDER_EXPORT Rasterizer : public IRasterizer
    {
    public:
        /**
         * \brief Creates a new \ref Rasterizer instance.
         * \param polygon_mode The \ref PolygonMode of the rasterizer.
         * \param cull_mode The \ref CullMode of the rasterizer.
         * \param cull_order The \ref CullOrder of the rasterizer.
         * \param line_width The line width of the rasterizer.
         * \param depth_stencil_state The \ref DepthStencilState of the rasterizer.
         */
        explicit Rasterizer(PolygonMode polygon_mode, CullMode cull_mode, CullOrder cull_order, float line_width, const DepthStencilState& depth_stencil_state) noexcept;
        ~Rasterizer() noexcept override;

        Rasterizer(const Rasterizer& other);
        Rasterizer(Rasterizer&& other) noexcept;
        Rasterizer& operator=(const Rasterizer& other);
        Rasterizer& operator=(Rasterizer&& other) noexcept;

        /// \copydoc IRasterizer::polygonMode()
        [[nodiscard]] PolygonMode polygonMode() const noexcept override;

        /// \copydoc IRasterizer::cullMode()
        [[nodiscard]] CullMode cullMode() const noexcept override;

        /// \copydoc IRasterizer::cullOrder()
        [[nodiscard]] CullOrder cullOrder() const noexcept override;

        /// \copydoc IRasterizer::lineWidth()
        [[nodiscard]] float lineWidth() const noexcept override;

        /// \copydoc IRasterizer::depthStencilState()
        [[nodiscard]] const DepthStencilState& depthStencilState() const noexcept override;

    protected:
        /// \brief Gets a reference to the line width of the rasterizer.
        [[nodiscard]] float& lineWidth() noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}

template <>
struct std::formatter<spark::render::PolygonMode> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::PolygonMode polygon_mode, auto& ctx)
    {
        switch (polygon_mode)
        {
        case spark::render::PolygonMode::Solid:
            return std::format_to(ctx.out(), "Solid");
        case spark::render::PolygonMode::WireFrame:
            return std::format_to(ctx.out(), "WireFrame");
        case spark::render::PolygonMode::Point:
            return std::format_to(ctx.out(), "Point");
        }

        return std::format_to(ctx.out(), "Unknown");
    }
};

template <>
struct std::formatter<spark::render::CullMode> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::CullMode cull_mode, auto& ctx)
    {
        switch (cull_mode)
        {
        case spark::render::CullMode::FrontFaces:
            return std::format_to(ctx.out(), "FrontFaces");
        case spark::render::CullMode::BackFaces:
            return std::format_to(ctx.out(), "BackFaces");
        case spark::render::CullMode::Both:
            return std::format_to(ctx.out(), "Both");
        case spark::render::CullMode::Disabled:
            return std::format_to(ctx.out(), "Disabled");
        }

        return std::format_to(ctx.out(), "Unknown");
    }
};

template <>
struct std::formatter<spark::render::CullOrder> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::CullOrder cull_order, auto& ctx)
    {
        switch (cull_order)
        {
        case spark::render::CullOrder::ClockWise:
            return std::format_to(ctx.out(), "ClockWise");
        case spark::render::CullOrder::CounterClockWise:
            return std::format_to(ctx.out(), "CounterClockWise");
        }

        return std::format_to(ctx.out(), "Unknown");
    }
};
