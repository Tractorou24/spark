#pragma once

#include "spark/render/Export.h"
#include "spark/render/Pipeline.h"

namespace spark::render
{
    /**
     * \brief Defines how a \ref IBuffer or \ref IImage resource is accessed.
     */
    enum class ResourceAccess
    {
        /// \brief Indicates that a resource is not accessed.
        /// This access mode translates to `VK_ACCESS_NONE` in Vulkan.
        /// This access flag is special, as it cannot be combined with other access flags.
        None = 0x7FFFFFFF,

        /// \brief Indicates that a resource is accessed as a vertex buffer.
        /// This access mode translates to `VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT` in Vulkan.
        VertexBuffer = 0x00000001,

        /// \brief Indicates that a resource is accessed as an index buffer.
        /// This access mode translates to `VK_ACCESS_INDEX_READ_BIT` in Vulkan.
        IndexBuffer = 0x00000002,

        /// \brief Indicates that a resource is accessed as an uniform/constant buffer.
        /// This access mode translates to `VK_ACCESS_UNIFORM_READ_BIT` in Vulkan.
        UniformBuffer = 0x00000004,

        /// \brief Indicates that a resource is accessed as a render target.
        /// This access mode translates to `VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT` in Vulkan.
        RenderTarget = 0x00000008,

        /// \brief Indicates that a resource is accessed as to read depth/stencil values.
        /// This access mode translates to `VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT` in the Vulkan.
        DepthStencilRead = 0x00000010,

        /// \brief Indicates that a resource is accessed as to write depth/stencil values.
        /// This access mode translates to `VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT` in Vulkan.
        DepthStencilWrite = 0x00000020,

        /// \brief Indicates that a resource is accessed as a read-only shader resource.
        /// This access mode translates to `VK_ACCESS_SHADER_READ_BIT` in Vulkan.
        ShaderRead = 0x00000040,

        /// \brief Indicates that a resource is accessed as a read-write shader resource.
        /// This access mode translates to `VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT` in Vulkan.
        ShaderReadWrite = 0x00000080,

        /// \brief Indicates that a resource is accessed as to read indirect draw commands.
        /// This access mode translates to `VK_ACCESS_INDIRECT_COMMAND_READ_BIT` in Vulkan.
        Indirect = 0x00000100,

        /// \brief Indicates that a resource is accessed as to read during a transfer operation.
        /// This access mode translates to `VK_ACCESS_TRANSFER_READ_BIT` in Vulkan.
        TransferRead = 0x00000200,

        /// \brief Indicates that a resource is accessed as to write during a transfer operation.
        /// This access mode translates to `VK_ACCESS_TRANSFER_WRITE_BIT` in Vulkan.
        TransferWrite = 0x00000400,

        /// \brief Indicates that a resource is accessed as to read during a resolve operation.
        /// This access mode translates to `VK_ACCESS_MEMORY_READ_BIT` in Vulkan.
        ResolveRead = 0x00000800,

        /// \brief Indicates that a resource is accessed as to write during a resolve operation.
        /// This access mode translates to `VK_ACCESS_MEMORY_WRITE_BIT` in Vulkan.
        ResolveWrite = 0x00001000,

        /// \brief Indicates that a resource can be accessed in any way, compatible to the layout.
        /// Note that you have to ensure that you do not access the resource in an incompatible way manually.
        /// This access mode translates to `VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT` in Vulkan.
        Common = 0x00002000
    };
}
