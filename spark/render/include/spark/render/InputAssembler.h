#pragma once

#include "spark/render/Export.h"
#include "spark/render/IndexBuffer.h"
#include "spark/render/VertexBuffer.h"

#include <algorithm>
#include <format>
#include <vector>

namespace spark::render
{
    /**
     * \brief The primitive topology used to interpret vertices.
     */
    enum class PrimitiveTopology
    {
        /// \brief List of points.
        PointList = 0x00010001,

        /// \brief List of lines. Vertices 0-1 are considered a line, then 2-3...
        LineList = 0x00020001,

        /// \brief List of triangles. Vertices 0-1-2 are considered a triangle, then 3-4-5...
        TriangleList = 0x00040001,

        /// \brief List of lines. Vertices 0-1 are considered a line, then 1-2, 2-3...
        LineStrip = 0x00020002,

        /// \brief List of triangles. Vertices 012345 create 4 triangles (012, 213, 234, 435).
        TriangleStrip = 0x00040002
    };

    class SPARK_RENDER_EXPORT IInputAssembler
    {
    public:
        virtual ~IInputAssembler() noexcept = default;

        /**
         * \brief Gets all vertex buffers layouts in the input assembly.
         * \return A \ref std::vector of \link IVertexBufferLayout vertex buffers layouts \endlink in the input assembly.
         */
        [[nodiscard]] std::vector<const IVertexBufferLayout*> vertexBufferLayouts() const noexcept { return genericVertexBufferLayouts(); }

        /**
         * \brief Gets the vertex buffer layout for the given \p binding.
         * \param binding The binding point of the vertex buffer layout.
         * \return The \ref IVertexBufferLayout for the given \p binding.
         */
        [[nodiscard]] virtual const IVertexBufferLayout& vertexBufferLayout(unsigned int binding) const = 0;

        /**
         * \brief Gets the index buffer layout.
         * \return The \ref IIndexBufferLayout for the input assembly.
         */
        [[nodiscard]] virtual const IIndexBufferLayout& indexBufferLayout() const = 0;

        /**
         * \brief Gets the topology of the input assembly.
         * \return A \ref PrimitiveTopology value representing the topology of the input assembly.
         */
        [[nodiscard]] virtual PrimitiveTopology topology() const noexcept = 0;

    private:
        /// \brief Private method used to allow replacement of the generic methods by a custom \ref IVertexBufferLayout type.
        [[nodiscard]] virtual std::vector<const IVertexBufferLayout*> genericVertexBufferLayouts() const noexcept = 0;
    };

    /**
     * \brief Represents a the input assembler state of a \ref IRenderPipeline.
     * \tparam VertexBufferLayoutType The type of the vertex buffer layout. (inherits from \ref IVertexBufferLayout)
     * \tparam IndexBufferLayoutType The type of the index buffer layout. (inherits from \ref IIndexBufferLayout)
     */
    template <typename VertexBufferLayoutType, typename IndexBufferLayoutType>
    class InputAssembler : public IInputAssembler
    {
    public:
        using VertexBufferLayout = VertexBufferLayoutType;
        using IndexBufferLayout = IndexBufferLayoutType;

    public:
        /// \copydoc IInputAssembler::vertexBufferLayouts()
        [[nodiscard]] virtual std::vector<const VertexBufferLayoutType*> vertexBufferLayouts() const noexcept = 0;

        /// \copydoc IInputAssembler::vertexBufferLayout(unsigned)
        [[nodiscard]] const VertexBufferLayoutType& vertexBufferLayout(unsigned binding) const override = 0;

        /// \copydoc IInputAssembler::indexBufferLayout()
        [[nodiscard]] const IndexBufferLayoutType& indexBufferLayout() const override = 0;

    private:
        [[nodiscard]] std::vector<const IVertexBufferLayout*> genericVertexBufferLayouts() const noexcept override
        {
            auto tmp = vertexBufferLayouts();
            std::vector<const IVertexBufferLayout*> vertex_buffer_layouts_vector;
            vertex_buffer_layouts_vector.reserve(tmp.size());
            std::ranges::transform(tmp,
                                   std::back_inserter(vertex_buffer_layouts_vector),
                                   [](const auto& vertex_buffer_layout) { return static_cast<const IVertexBufferLayout*>(vertex_buffer_layout); });
            return vertex_buffer_layouts_vector;
        }
    };
}

template <>
struct std::formatter<spark::render::PrimitiveTopology> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::PrimitiveTopology topology, auto& ctx)
    {
        switch (topology)
        {
        case spark::render::PrimitiveTopology::PointList:
            return std::format_to(ctx.out(), "PointList");
        case spark::render::PrimitiveTopology::LineList:
            return std::format_to(ctx.out(), "LineList");
        case spark::render::PrimitiveTopology::TriangleList:
            return std::format_to(ctx.out(), "TriangleList");
        case spark::render::PrimitiveTopology::LineStrip:
            return std::format_to(ctx.out(), "LineStrip");
        case spark::render::PrimitiveTopology::TriangleStrip:
            return std::format_to(ctx.out(), "TriangleStrip");
        }
        return std::format_to(ctx.out(), "Unknown");
    }
};
