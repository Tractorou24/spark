#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/DescriptorSet.h"
#include "spark/render/Export.h"
#include "spark/render/Image.h"
#include "spark/render/IndexBuffer.h"
#include "spark/render/Pipeline.h"
#include "spark/render/Scissor.h"
#include "spark/render/VertexBuffer.h"
#include "spark/render/Viewport.h"

#include "spark/math/Vector4.h"

#include <memory>
#include <span>

namespace spark::render
{
    /**
     * \brief Interface for a command buffer.
     */
    class SPARK_RENDER_EXPORT ICommandBuffer
    {
    public:
        virtual ~ICommandBuffer() noexcept = default;

        /**
         * \brief Sets the command buffer to recording state, allowing it to receive commands that should be transmitted to the \ref CommandQueue.
         * \note You should wait for the buffer to be executed before recording again.
         *
         * \throws spark::base::CommandBufferAlreadyRecordingException if the command buffer is already recording.
         */
        virtual void begin() const = 0;

        /**
         * \brief Ends the recording of the command buffer.
         * \note You can call this method multiple times but it will do nothing after the first call.
         */
        virtual void end() const = 0;

        /**
         * \brief Checks if the command buffer is recording.
         * \return `true` if the command buffer is recording, `false` otherwise.
         */
        [[nodiscard]] virtual bool isRecording() const noexcept = 0;

        /**
         * \brief Checks if the command buffer is a secondary command buffer.
         * \return `true` if the command buffer is a secondary command buffer, `false` otherwise.
         */
        [[nodiscard]] virtual bool isSecondary() const noexcept = 0;

        /**
         * \brief Performs a buffer to buffer transfer from \p source to \p target.
         * \param source The source buffer to transfer data from.
         * \param target The target buffer to transfer data to.
         * \param source_element The index of the first element to transfer from the source buffer.
         * \param target_element The index of the first element to transfer to the target buffer.
         * \param elements The number of elements to transfer.
         *
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source or target buffers has not enough elements for the specified \p elements parameter.
         */
        void transfer(IBuffer& source, IBuffer& target, unsigned int source_element = 0, unsigned int target_element = 0, unsigned int elements = 1) const
        {
            genericTransfer(source, target, source_element, target_element, elements);
        }

        /**
         * \brief Performs a buffer to buffer transfer from \p source to \p target.
         * \param source The source buffer to transfer data from.
         * \param target The target buffer to transfer data to.
         * \param source_element The index of the first element to transfer from the source buffer.
         * \param target_element The index of the first element to transfer to the target buffer.
         * \param elements The number of elements to transfer.
         *
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source or target buffers has not enough elements for the specified \p elements parameter.
         */
        void transfer(std::shared_ptr<IBuffer> source, IBuffer& target, unsigned int source_element = 0, unsigned int target_element = 0, unsigned int elements = 1) const
        {
            genericTransfer(source, target, source_element, target_element, elements);
        }

        /**
         * \brief Performs a buffer to image transfer from \p source to \p target.
         * \param source The source buffer to transfer data from.
         * \param target The target image to transfer data to.
         * \param source_element The index of the first element to transfer from the source buffer.
         * \param first_subresource The index of the first subresource of the target image to transfer data to.
         * \param elements The number of elements to transfer from the source buffer to the target image subresources.
         *
         * The \p first_subresource parameter describes the index of the first subresource to copy.
         * Each element is copied into the subsequent subresource, where resources are counted in the following order:
         * - Level (mipmaps)
         * - Layer (array slice)
         * - Plane (for multi-planar formats)
         *
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source buffer or the target image has not enough elements for the specified \p elements parameter.
         */
        void transfer(IBuffer& source, IImage& target, unsigned int source_element = 0, unsigned int first_subresource = 0, unsigned int elements = 1) const
        {
            genericTransfer(source, target, source_element, first_subresource, elements);
        }

        /**
         * \brief Performs a buffer to image transfer from \p source to \p target.
         * \param source The source buffer to transfer data from.
         * \param target The target image to transfer data to.
         * \param source_element The index of the first element to transfer from the source buffer.
         * \param first_subresource The index of the first subresource of the target image to transfer data to.
         * \param elements The number of elements to transfer from the source buffer to the target image subresources.
         *
         * The \p first_subresource parameter describes the index of the first subresource to copy.
         * Each element is copied into the subsequent subresource, where resources are counted in the following order:
         * - Level (mipmaps)
         * - Layer (array slice)
         * - Plane (for multi-planar formats)
         *
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source buffer or the target image has not enough elements for the specified \p elements parameter.
         */
        void transfer(std::shared_ptr<IBuffer> source, IImage& target, unsigned int source_element = 0, unsigned int first_subresource = 0, unsigned int elements = 1) const
        {
            genericTransfer(source, target, source_element, first_subresource, elements);
        }

        /**
         * \brief Performs an image to buffer transfer from \p source to \p target.
         * \param source The source image to transfer data from.
         * \param target The target buffer to transfer data to.
         * \param source_subresource The index of the first subresource of the source image to transfer data from.
         * \param target_subresource The index of the first element to transfer to the target buffer.
         * \param subresources The number of subresources to transfer from the source image to the target buffer.
         *
         * \note You have to manually ensure that \p source and \p target buffers are in the proper state for transfer operations.
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source image or the target buffer has not enough elements for the specified \p subresources parameter.
         */
        void transfer(IImage& source, IImage& target, unsigned int source_subresource = 0, unsigned int target_subresource = 0, unsigned int subresources = 1) const
        {
            genericTransfer(source, target, source_subresource, target_subresource, subresources);
        }

        /**
         * \brief Performs an image to buffer transfer from \p source to \p target.
         * \param source The source image to transfer data from.
         * \param target The target buffer to transfer data to.
         * \param source_subresource The index of the first subresource of the source image to transfer data from.
         * \param target_subresource The index of the first element to transfer to the target buffer.
         * \param subresources The number of subresources to transfer from the source image to the target buffer.
         *
         * \note You have to manually ensure that \p source and \p target buffers are in the proper state for transfer operations.
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source image or the target buffer has not enough elements for the specified \p subresources parameter.
         */
        void transfer(std::shared_ptr<IImage> source, IImage& target, unsigned int source_subresource = 0, unsigned int target_subresource = 0, unsigned int subresources = 1) const
        {
            genericTransfer(source, target, source_subresource, target_subresource, subresources);
        }

        /**
         * \brief Performs an image to buffer transfer from \p source to \p target.
         * \param source The source image to transfer data from.
         * \param target The target buffer to transfer data to.
         * \param first_subresource The index of the first subresource of the source image to transfer data from.
         * \param target_element The index of the first element to transfer to the target buffer.
         * \param subresources The number of subresources to transfer from the source image to the target buffer.
         *
         * The \p first_subresource parameter describes the index of the first subresource to copy.
         * Each element is copied into the subsequent subresource, where resources are counted in the following order:
         * - Level (mipmaps)
         * - Layer (array slice)
         * - Plane (for multi-planar formats)
         *
         * So, if 6 elements should be copied to an image with 3 mip-map levels and 3 layers, the elements 0-2 contain the mip-map levels of the first layer, while elements 3-5 
         * contain the three mip-map levels of the second layer. The third layer would not receive any data in this example. If the image format has multiple planes, this procedure 
         * would be repeated for each plane, however one buffer element only maps to one sub-resource.
         *
         * \note You have to manually ensure that \p source and \p target buffers are in the proper state for transfer operations.
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source image or the target buffer has not enough elements for the specified \p subresources parameter.
         */
        void transfer(IImage& source, IBuffer& target, unsigned int first_subresource = 0, unsigned int target_element = 0, unsigned int subresources = 1) const
        {
            genericTransfer(source, target, first_subresource, target_element, subresources);
        }

        /**
         * \brief Performs an image to buffer transfer from \p source to \p target.
         * \param source The source image to transfer data from.
         * \param target The target buffer to transfer data to.
         * \param first_subresource The index of the first subresource of the source image to transfer data from.
         * \param target_element The index of the first element to transfer to the target buffer.
         * \param subresources The number of subresources to transfer from the source image to the target buffer.
         *
         * The \p first_subresource parameter describes the index of the first subresource to copy.
         * Each element is copied into the subsequent subresource, where resources are counted in the following order:
         * - Level (mipmaps)
         * - Layer (array slice)
         * - Plane (for multi-planar formats)
         *
         * So, if 6 elements should be copied to an image with 3 mip-map levels and 3 layers, the elements 0-2 contain the mip-map levels of the first layer, while elements 3-5
         * contain the three mip-map levels of the second layer. The third layer would not receive any data in this example. If the image format has multiple planes, this procedure
         * would be repeated for each plane, however one buffer element only maps to one sub-resource.
         *
         * \note You have to manually ensure that \p source and \p target buffers are in the proper state for transfer operations.
         * \throws spark::base::ArgumentOutOfRangeException if the number of either the source image or the target buffer has not enough elements for the specified \p subresources parameter.
         */
        void transfer(std::shared_ptr<IImage> source, IBuffer& target, unsigned int first_subresource = 0, unsigned int target_element = 0, unsigned int subresources = 1) const
        {
            genericTransfer(source, target, first_subresource, target_element, subresources);
        }

        /**
         * \brief Sets the \p pipeline to be used for subsequent draw calls.
         * \param pipeline A \ref IPipeline object to use for subsequent draw calls.
         */
        void use(const IPipeline& pipeline) const noexcept { genericUse(pipeline); }

        /**
         * \brief Binds the \p descriptor_set to the last pipeline used by the command buffer.
         * \param descriptor_set The \ref IDescriptorSet to bind.
         *
         * \throws base::BadArgumentException if the command buffer has not used a pipeline yet.
         */
        void bind(const IDescriptorSet& descriptor_set) { genericBind(descriptor_set); }

        /**
         * \brief Binds the \p descriptor_set to the \p pipeline.
         * \param descriptor_set The \ref IDescriptorSet to bind.
         * \param pipeline The \ref IPipeline to bind the \p descriptor_set to.
         */
        void bind(const IDescriptorSet& descriptor_set, const IPipeline& pipeline) const noexcept { genericBind(descriptor_set, pipeline); }

        /**
         * \brief Binds the \p vertex_buffer to the pipeline.
         * \param vertex_buffer The \ref IVertexBuffer to bind to the pipeline.
         *
         * \note After binding the vertex buffer, the next call to \ref ICommandBuffer::draw() or \ref ICommandBuffer::drawIndexed() will read vertex data from the bound buffer until another vertex buffer is bound.
         */
        void bind(const IVertexBuffer& vertex_buffer) const noexcept { genericBind(vertex_buffer); }

        /**
         * \brief Binds the \p index_buffer to the pipeline.
         * \param index_buffer The \ref IIndexBuffer to bind to the pipeline.
         *
         * \note After binding the index buffer, the next call to \ref ICommandBuffer::drawIndexed() will read data from the bound buffer until another index buffer is bound.
         */
        void bind(const IIndexBuffer& index_buffer) const noexcept { genericBind(index_buffer); }

        /**
         * \brief Draw a number of vertices from the currently bound vertex buffer.
         * \param vertices The number of vertices to draw.
         * \param instances The number of instances to draw.
         * \param first_vertex The index of the first vertex to draw.
         * \param first_instance The index of the first instance to draw.
         */
        void draw(unsigned int vertices, unsigned int instances = 1, unsigned int first_vertex = 0, unsigned int first_instance = 0) const noexcept
        {
            genericDraw(vertices, instances, first_vertex, first_instance);
        }

        /**
         * \brief Draw all vertices from the provided \ref IVertexBuffer in \p vertex_buffer.
         * \param vertex_buffer The \ref IVertexBuffer to draw from.
         * \param instances The number of instances to draw.
         * \param first_vertex The index of the first vertex to start drawing from.
         * \param first_instance The index of the first instance to draw.
         */
        void draw(const IVertexBuffer& vertex_buffer, unsigned int instances = 1, unsigned int first_vertex = 0, unsigned int first_instance = 0) const noexcept
        {
            genericDraw(vertex_buffer, instances, first_vertex, first_instance);
        }

        /**
         * \brief Draws the currently bound vertex buffer with a set of indices from the currently bound index buffer.
         * \param indices The number of indices to draw.
         * \param instances The number of instances to draw.
         * \param first_index The index of the first index to draw.
         * \param vertex_offset The offset to add to each index to find the vertex to draw.
         * \param first_instance The index of the first instance to draw.
         */
        void drawIndexed(unsigned int indices, unsigned int instances = 1, unsigned int first_index = 0, int vertex_offset = 0, unsigned int first_instance = 0) const noexcept
        {
            genericDrawIndexed(indices, instances, first_index, vertex_offset, first_instance);
        }

        /**
         * \brief Draws the currently bound \ref IVertexBuffer using the provided \ref IIndexBuffer in \p index_buffer.
         * \param index_buffer The \ref IIndexBuffer to draw with.
         * \param instances The number of instances to draw.
         * \param first_index The index of the first element of the \p index_buffer to start drawing from.
         * \param vertex_offset The offset to add to each index to find the vertex to draw.
         * \param first_instance The index of the first instance to draw.
         *
         * \note This helper method binds the index buffer and issues a draw command for all indices.
         */
        void drawIndexed(const IIndexBuffer& index_buffer,
                         unsigned int instances = 1,
                         unsigned int first_index = 0,
                         int vertex_offset = 0,
                         unsigned int first_instance = 0) const noexcept { genericDrawIndexed(index_buffer, instances, first_index, vertex_offset, first_instance); }

        /**
         * \brief Draws the currently bound \ref IVertexBuffer using the provided \ref IIndexBuffer in \p index_buffer.
         * \param vertex_buffer The \ref IVertexBuffer to draw from.
         * \param index_buffer The \ref IIndexBuffer to draw with.
         * \param instances The number of instances to draw.
         * \param first_index The index of the first element of the \p index_buffer to start drawing from.
         * \param vertex_offset The offset to add to each index to find the vertex to draw.
         * \param first_instance The index of the first instance to draw.
         */
        void drawIndexed(const IVertexBuffer& vertex_buffer,
                         const IIndexBuffer& index_buffer,
                         unsigned int instances = 1,
                         unsigned int first_index = 0,
                         int vertex_offset = 0,
                         unsigned int first_instance = 0) const noexcept { genericDrawIndexed(vertex_buffer, index_buffer, instances, first_index, vertex_offset, first_instance); }

        /**
         * \brief Executes a compute shader.
         */
        virtual void dispatch() const noexcept = 0;

        /**
         * \brief Executes a secondary command buffer.
         * \param command_buffer The secondary command buffer to execute.
         */
        void execute(std::shared_ptr<const ICommandBuffer> command_buffer) const { genericExecute(command_buffer); }

        /**
         * \brief Executes a series of secondary command buffers/bundles.
         * \param command_buffers The \ref std::vector of secondary command buffers to execute.
         */
        void execute(const std::vector<std::shared_ptr<const ICommandBuffer>>& command_buffers) const { genericExecute(command_buffers); }

        /**
         * \brief Pushes a block of memory into the push constants backing memory.
         * \param layout The \ref IPushConstantsLayout to update.
         * \param memory A pointer to the source memory.
         */
        void pushConstants(const IPushConstantsLayout& layout, const void* const memory) const noexcept { genericPushConstants(layout, memory); }

        /**
         * \brief Sets the viewport to use for subsequent draw calls.
         * \param viewport The \ref IViewport to use for subsequent draw calls.
         */
        virtual void setViewport(const IViewport* viewport) const noexcept = 0;

        /**
         * \brief Sets the viewports to use for subsequent draw calls.
         * \param viewports The \ref std::span of \ref IViewport to use for subsequent draw calls.
         */
        virtual void setViewports(std::span<const IViewport*> viewports) const noexcept = 0;

        /**
         * \brief Sets the scissor to use for subsequent draw calls.
         * \param scissor The \ref IScissor to use for subsequent draw calls.
         */
        virtual void setScissor(const IScissor* scissor) const noexcept = 0;

        /**
         * \brief Sets the scissors to use for subsequent draw calls.
         * \param scissors A \ref std::span of \ref IScissor to use for subsequent draw calls.
         */
        virtual void setScissors(std::span<const IScissor*> scissors) const noexcept = 0;

        /**
         * \brief Sets the blend factors to use for subsequent draw calls.
         * \param blend_factors The \ref math::Vector4<float> containing the blend factors to use for subsequent draw calls.
         *
         * \note Blend factors are set for all render targets that use the blend modes `ConstantColor`, `OneMinusConstantColor`, `ConstantAlpha` or `OneMinusConstantAlpha`.
         */
        virtual void setBlendFactors(const math::Vector4<float>& blend_factors) const noexcept = 0;

        /**
         * \brief Sets the stencil reference value to use for subsequent draw calls.
         * \param stencil_ref The stencil reference value to use for subsequent draw calls.
         */
        virtual void setStencilRef(unsigned int stencil_ref) const noexcept = 0;

        /**
         * \brief Called by the parent command queue to signal that the command buffer should release it's shared state.
         */
        virtual void releaseSharedState() const = 0;

    private:
        /// @{
        /// \brief Private method used to allow replacement of the generic methods by custom types.
        virtual void genericTransfer(IBuffer& source,
                                     IBuffer& target,
                                     unsigned int source_element = 0,
                                     unsigned int target_element = 0,
                                     unsigned int elements = 1) const noexcept = 0;
        virtual void genericTransfer(std::shared_ptr<IBuffer> source,
                                     IBuffer& target,
                                     unsigned int source_element = 0,
                                     unsigned int target_element = 0,
                                     unsigned int elements = 1) const = 0;
        virtual void genericTransfer(IBuffer& source,
                                     IImage& target,
                                     unsigned int source_element = 0,
                                     unsigned int first_subresource = 0,
                                     unsigned int elements = 1) const noexcept = 0;
        virtual void genericTransfer(std::shared_ptr<IBuffer> source,
                                     IImage& target,
                                     unsigned int source_element = 0,
                                     unsigned int first_subresource = 0,
                                     unsigned int elements = 1) const noexcept = 0;
        virtual void genericTransfer(IImage& source,
                                     IImage& target,
                                     unsigned int source_subresource = 0,
                                     unsigned int target_subresource = 0,
                                     unsigned int subresources = 1) const noexcept = 0;
        virtual void genericTransfer(std::shared_ptr<IImage> source,
                                     IImage& target,
                                     unsigned int source_subresource = 0,
                                     unsigned int target_subresource = 0,
                                     unsigned int subresources = 1) const noexcept = 0;
        virtual void genericTransfer(IImage& source, IBuffer& target, unsigned int first_subresource = 0, unsigned int target_element = 0, unsigned int subresources = 1) const = 0;
        virtual void genericTransfer(std::shared_ptr<IImage> source,
                                     IBuffer& target,
                                     unsigned int first_subresource = 0,
                                     unsigned int target_element = 0,
                                     unsigned int subresources = 1) const noexcept = 0;
        virtual void genericUse(const IPipeline& pipeline) const noexcept = 0;
        virtual void genericBind(const IDescriptorSet& descriptor_set) const = 0;
        virtual void genericBind(const IDescriptorSet& descriptor_set, const IPipeline& pipeline) const noexcept = 0;
        virtual void genericBind(const IIndexBuffer& index_buffer) const noexcept = 0;
        virtual void genericBind(const IVertexBuffer& vertex_buffer) const noexcept = 0;
        virtual void genericDraw(unsigned int vertices, unsigned int instances = 1, unsigned int first_vertex = 0, unsigned int first_instance = 0) const noexcept = 0;
        virtual void genericDraw(const IVertexBuffer& vertex_buffer, unsigned int instances = 1, unsigned int first_vertex = 0, unsigned int first_instance = 0) const noexcept = 0;
        virtual void genericDrawIndexed(unsigned int indices,
                                        unsigned int instances = 1,
                                        unsigned int first_index = 0,
                                        int vertex_offset = 0,
                                        unsigned int first_instance = 0) const noexcept = 0;
        virtual void genericDrawIndexed(const IIndexBuffer& index_buffer,
                                        unsigned int instances = 1,
                                        unsigned int first_index = 0,
                                        int vertex_offset = 0,
                                        unsigned int first_instance = 0) const noexcept = 0;
        virtual void genericDrawIndexed(const IVertexBuffer& vertex_buffer,
                                        const IIndexBuffer& index_buffer,
                                        unsigned int instances = 1,
                                        unsigned int first_index = 0,
                                        int vertex_offset = 0,
                                        unsigned int first_instance = 0) const noexcept = 0;
        virtual void genericExecute(std::shared_ptr<const ICommandBuffer> command_buffer) const = 0;
        virtual void genericExecute(const std::vector<std::shared_ptr<const ICommandBuffer>>& command_buffers) const = 0;
        virtual void genericPushConstants(const IPushConstantsLayout& layout, const void* const memory) const noexcept = 0;
        /// @}
    };
}
