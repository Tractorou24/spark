#pragma once

#include "spark/base/Macros.h"

namespace spark::render
{
    /**
     * \brief Describes the valid shader stages of a graphics pipeline.
     */
    enum class ShaderStage
    {
        /// \brief Represents the vertex shader stage.
        Vertex = 0x00000001,

        /// \brief Represents the tessellation control or hull shader stage.
        TessellationControl = 0x00000002,

        /// \brief Represents the tessellation evaluation or domain shader stage.
        TessellationEvaluation = 0x00000004,

        /// \brief Represents the  geometry shader stage.
        /// \note Geometry shaders come with a performance penalty and might not be supported on all platforms.
        Geometry = 0x00000008,

        /// \brief Represents the fragment or vertex shader stage.
        Fragment = 0x00000010,

        /// \brief Represents the compute shader stage.
        Compute = 0x00000020,

        /// \brief Represents an unknown shader stage.
        Other = 0x7FFFFFFF
    };

    SPARK_DEFINE_ENUM_FLAGS(ShaderStage);
}

template <>
struct std::formatter<spark::render::ShaderStage>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::ShaderStage stage, auto& ctx)
    {
        switch (stage) // NOLINT(clang-diagnostic-switch-enum): Other is handled after the switch statement.
        {
        case spark::render::ShaderStage::Vertex:
            return std::format_to(ctx.out(), "Vertex");
        case spark::render::ShaderStage::TessellationControl:
            return std::format_to(ctx.out(), "TessellationControl");
        case spark::render::ShaderStage::TessellationEvaluation:
            return std::format_to(ctx.out(), "TessellationEvaluation");
        case spark::render::ShaderStage::Geometry:
            return std::format_to(ctx.out(), "Geometry");
        case spark::render::ShaderStage::Fragment:
            return std::format_to(ctx.out(), "Fragment");
        case spark::render::ShaderStage::Compute:
            return std::format_to(ctx.out(), "Compute");
        default:
            break;
        }
        return std::format_to(ctx.out(), "Other");
    }
};
