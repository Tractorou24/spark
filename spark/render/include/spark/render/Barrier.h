#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/Export.h"
#include "spark/render/Image.h"
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

    /**
     * \brief The interface for a barrier.
     *
     * Barriers are used to synchronize the GPU with itself in a command buffer. They are basically used to control the GPU command flow and ensure that resources are in
     * he right state before using them. Generally speaking, there are two related types of barriers:
     *
     * - **Execution barriers** are enforcing command order by telling the GPU to wait for certain pipeline stages or which stages to block until an operation has finished.
     * - **Memory barriers** are used to transition resources between states and are a subset of execution barriers. Each memory barrier is always also an execution barrier,
     *   but the opposite is not true.
     *
     * An execution barrier is simply a barrier without any *resource transitions* happening. The only properties that are set for an execution barrier are two pipeline stages
     * defined by \ref IBarrier::syncBefore and \ref IBarrier::syncAfter. The first value defines the pipeline stages, all *previous* commands in a command buffer need to
     * finish before execution is allowed to continue. Similarly, the second value defines the stage, all *subsequent* commands need to wait for, before they are allowed to
     * continue execution. For example, setting `syncBefore` to `Compute` and `syncAfter` to `Vertex` logically translates to: *All subsequent  commands that want to pass
     * the vertex stage need to wait there before all previous commands passed the compute stage*. To synchronize reads in previous commands with writes in subsequent commands,
     * this is sufficient. However, in order to do the opposite, this is not enough. Instead, resource memory needs to be *transitioned* by specifying the desired \ref ResourceAccess,
     * alongside the \ref ImageLayout for images (note that buffers always share a *common* layout that can not be changed). This is done using memory barriers.
     * There are two types of memory barriers used for state transitions:
     *
     * - **Global barriers** apply to all resource memory.
     * - **Image and buffer barriers** apply to individual images or buffers or a sub-resource of those.
     *
     * Any `IBarrier` can contain an arbitrary mix of one or more global and/or image/buffer barriers. A global barrier is inserted by calling \ref IBarrier::wait.
     * This method accepts two parameters: a `before` and an `after` access mode. Those parameters specify the \ref ResourceAccess for the previous and subsequent
     * commands. This makes it possible to describe scenarios like *wait for certain writes to finish before continuing with certain reads*. Note that a resource can be
     * accessed in different ways at the same time (for example as copy source and shader resource) and specifying an access state will only wait for the specified subset.
     * As a rule of thumb, you should always specify as little access as possible in order to leave most room for optimization.
     *
     * Image and buffer barriers additionally describe which (sub-)resources to apply the barrier to. For buffers this only applies to individual elements in a buffer array.
     * Currently, buffers are always transitioned as a whole. This is different from image resources, which have addressable sub-resources (mip levels, planes and array elements).
     * For images, it is possible to transition individual sub-resources into different \ref ImageLayouts to indicate when and how a texture is used. An image in a certain
     * layout poses restrictions on how it can be accessed. For example, a `ReadWrite` image written by a compute shader must be transitioned into a proper layout to be read by
     * a graphics shader. To facilitate such a transition, a barrier is required. Image barriers can be inserted by calling one of the overloads of \ref IBarrier::transition that
     * accepts an \ref IImage parameter.
     */
    class SPARK_RENDER_EXPORT IBarrier
    {
    public:
        virtual ~IBarrier() = default;

        /**
         * \brief Gets the stage that all previous commands need to reach before continuing execution.
         * \return The stage that all previous commands need to reach before continuing execution.
         */
        [[nodiscard]] constexpr virtual PipelineStage syncBefore() const noexcept = 0;

        /**
         * \brief Gets the stage all subsequent commands need to wait for before continuing execution.
         * \return The stage all subsequent commands need to wait for before continuing execution.
         */
        [[nodiscard]] constexpr virtual PipelineStage syncAfter() const noexcept = 0;

        /**
         * \brief Inserts a global barrier that waits for previous commands to finish.
         * \param access_before The access types previous commands have to finish.
         * \param access_after The access types that subsequent commands continue with.
         */
        constexpr virtual void wait(ResourceAccess access_before, ResourceAccess access_after) noexcept = 0;

        /**
         * \brief Inserts a buffer barrier that blocks access to \p "buffer.
         * \param buffer The buffer resource to transition.
         * \param access_before The access types previous commands have to finish.
         * \param access_after The access types that subsequent commands continue with.
         */
        constexpr void transition(IBuffer& buffer, ResourceAccess access_before, ResourceAccess access_after)
        {
            genericTransition(buffer, access_before, access_after);
        }

        /**
         * \brief Inserts an image barrier that blocks access to all sub-resources of \p image of the types contained and transitions all sub-resources into \p layout.
         * \param image The image resource to transition.
         * \param access_before The access types previous commands have to finish.
         * \param access_after The access types that subsequent commands continue with.
         * \param layout The image layout to transition into.
         */
        constexpr void transition(IImage& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout layout)
        {
            genericTransition(image, access_before, access_after, layout);
        }

        /**
         * \brief Inserts an image barrier that blocks access to a sub-resource range of \p image and transitions the sub-resource into \p layout.
         * \param image The image resource to transition.
         * \param access_before The access types previous commands have to finish.
         * \param access_after The access types that subsequent commands continue with.
         * \param from_layout The image layout to transition from.
         * \param to_layout The image layout to transition into.
        */
        constexpr void transition(IImage& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout from_layout, ImageLayout to_layout)
        {
            genericTransition(image, access_before, access_after, from_layout, to_layout);
        }

    private:
        constexpr virtual void genericTransition(IBuffer& buffer, ResourceAccess access_before, ResourceAccess access_after) = 0;
        constexpr virtual void genericTransition(IImage& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout layout) = 0;
        constexpr virtual void genericTransition(IImage& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout from_layout, ImageLayout to_layout) = 0;
    };

    /**
     * \brief A barrier used to synchronize the GPU with itself.
     * \tparam BufferType The type of the buffer resource.
     * \tparam ImageType The type of the image resource.
     */
    template <typename BufferType, typename ImageType>
    class SPARK_RENDER_EXPORT Barrier : public IBarrier
    {
    public:
        using buffer_type = BufferType;
        using image_type = ImageType;

    public:
        /// \copydoc IBarrier::transition
        constexpr virtual void transition(buffer_type& buffer, ResourceAccess access_before, ResourceAccess access_after) = 0;

        /// \copydoc IBarrier::transition
        constexpr virtual void transition(image_type& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout layout) = 0;

        /// \copydoc IBarrier::transition
        constexpr virtual auto transition(image_type& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout from_layout, ImageLayout to_layout) -> void = 0;

    private:
        constexpr void genericTransition(IBuffer& buffer, ResourceAccess access_before, ResourceAccess access_after) override
        {
            transition(dynamic_cast<buffer_type&>(buffer), access_before, access_after);
        }

        constexpr void genericTransition(IImage& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout layout) override
        {
            transition(dynamic_cast<image_type&>(image), access_before, access_after, layout);
        }

        constexpr void genericTransition(IImage& image, ResourceAccess access_before, ResourceAccess access_after, ImageLayout from_layout, ImageLayout to_layout) override
        {
            transition(dynamic_cast<image_type&>(image), access_before, access_after, from_layout, to_layout);
        }
    };
}
