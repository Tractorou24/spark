#pragma once

#include "spark/render/CommandBuffer.h"
#include "spark/render/CommandQueue.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanIndexBuffer.h"
#include "spark/render/vk/VulkanPipeline.h"
#include "spark/render/vk/VulkanPipelineLayout.h"
#include "spark/render/vk/VulkanVertexBuffer.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkCommandBuffer)

namespace spark::render::vk
{
    class VulkanQueue;
    class VulkanRenderPass;
    class IVulkanBuffer;
    class IVulkanImage;
    class VulkanDescriptorSet;
    class VulkanPushConstantsLayout;

    /**
     * \brief Vulkan implementation of \ref ICommandBuffer.
     */
    class SPARK_RENDER_VK_EXPORT VulkanCommandBuffer final : public CommandBuffer<
                                                                 VulkanCommandBuffer, IVulkanBuffer, IVulkanVertexBuffer, IVulkanIndexBuffer, IVulkanImage, VulkanPipelineState>,
                                                             public Resource<VkCommandBuffer>
    {
    public:
        explicit VulkanCommandBuffer(const VulkanQueue& queue, bool begin_recording = false, bool is_primary = true);
        ~VulkanCommandBuffer() override;

        VulkanCommandBuffer(const VulkanCommandBuffer& other) = delete;
        VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept = delete;
        VulkanCommandBuffer& operator=(const VulkanCommandBuffer& other) = delete;
        VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) noexcept = delete;

        /// \copydoc ICommandBuffer::begin()
        void begin() const override;

        /**
         * \brief Begins the command buffer as a secondary command buffer that inherits the state of \p renderPass.
         * \param render_pass The render pass state to inherit.
         */
        void begin(const VulkanRenderPass& render_pass) const;

        /// \copydoc ICommandBuffer::end()
        void end() const override;

        /// \copydoc ICommandBuffer::isRecording()
        [[nodiscard]] bool isRecording() const noexcept override;

        /// \copydoc ICommandBuffer::isSecondary()
        [[nodiscard]] bool isSecondary() const noexcept override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(IVulkanBuffer& source, IVulkanBuffer& target, unsigned source_element, unsigned target_element, unsigned elements) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(IVulkanImage& source, IVulkanBuffer& target, unsigned first_subresource, unsigned target_element, unsigned subresources) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(IVulkanBuffer& source, IVulkanImage& target, unsigned source_element, unsigned first_subresource, unsigned elements) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(IVulkanImage& source, IVulkanImage& target, unsigned source_subresource, unsigned target_subresource, unsigned subresources) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(std::shared_ptr<IVulkanBuffer> source, IVulkanBuffer& target, unsigned source_element, unsigned target_element, unsigned elements) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(std::shared_ptr<IVulkanBuffer> source, IVulkanImage& target, unsigned source_element, unsigned first_subresource, unsigned elements) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(std::shared_ptr<IVulkanImage> source, IVulkanImage& target, unsigned first_subresource, unsigned target_element, unsigned subresources) const override;

        /// \copydoc ICommandBuffer::transfer()
        void transfer(std::shared_ptr<IVulkanImage> source, IVulkanBuffer& target, unsigned first_subresource, unsigned target_element, unsigned subresources) const override;

        /// \copydoc ICommandBuffer::use()
        void use(const VulkanPipelineState& pipeline) const noexcept override;

        /// \copydoc ICommandBuffer::use()
        void bind(const VulkanDescriptorSet& descriptor_set) const override;

        /// \copydoc ICommandBuffer::bind()
        void bind(const VulkanDescriptorSet& descriptor_set, const VulkanPipelineState& pipeline) const noexcept override;

        /// \copydoc ICommandBuffer::bind()
        void bind(const IVulkanIndexBuffer& index_buffer) const noexcept override;

        /// \copydoc ICommandBuffer::bind()
        void bind(const IVulkanVertexBuffer& vertex_buffer) const noexcept override;

        /// \copydoc ICommandBuffer::draw()
        void draw(unsigned vertices, unsigned instances, unsigned first_vertex, unsigned first_instance) const noexcept override;

        /// \copydoc ICommandBuffer::draw()
        void draw(const IVulkanVertexBuffer& vertex_buffer, unsigned instances, unsigned first_vertex, unsigned first_instance) const noexcept override;

        /// \copydoc ICommandBuffer::drawIndexed()
        void drawIndexed(unsigned indices, unsigned instances, unsigned first_index, int vertex_offset, unsigned first_instance) const noexcept override;

        /// \copydoc ICommandBuffer::drawIndexed()
        void drawIndexed(const IVulkanIndexBuffer& index_buffer, unsigned instances, unsigned first_index, int vertex_offset, unsigned first_instance) const noexcept override;

        /// \copydoc ICommandBuffer::drawIndexed()
        void drawIndexed(const IVulkanVertexBuffer& vertex_buffer,
                         const IVulkanIndexBuffer& index_buffer,
                         unsigned instances,
                         unsigned first_index,
                         int vertex_offset,
                         unsigned first_instance) const noexcept override;

        /// \copydoc ICommandBuffer::dispatch()
        void dispatch() const noexcept override;

        /// \copydoc ICommandBuffer::execute()
        void execute(std::shared_ptr<const VulkanCommandBuffer> command_buffer) const override;

        /// \copydoc ICommandBuffer::execute()
        void execute(const std::vector<std::shared_ptr<const VulkanCommandBuffer>>& command_buffers) const override;

        /// \copydoc ICommandBuffer::pushConstants()
        void pushConstants(const VulkanPushConstantsLayout& layout, const void* const memory) const noexcept override;

        /// \copydoc ICommandBuffer::setViewport()
        void setViewport(const IViewport* viewport) const noexcept override;

        /// \copydoc ICommandBuffer::setViewports()
        void setViewports(std::span<const IViewport*> viewports) const noexcept override;

        /// \copydoc ICommandBuffer::setScissor()
        void setScissor(const IScissor* scissor) const noexcept override;

        /// \copydoc ICommandBuffer::setScissors()
        void setScissors(std::span<const IScissor*> scissors) const noexcept override;

        /// \copydoc ICommandBuffer::setBlendFactors()
        void setBlendFactors(const math::Vector4<float>& blend_factors) const noexcept override;

        /// \copydoc ICommandBuffer::setStencilRef()
        void setStencilRef(unsigned stencil_ref) const noexcept override;

        /// \copydoc ICommandBuffer::releaseSharedState()
        void releaseSharedState() const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
