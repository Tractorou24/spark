#include "spark/render/vk/VulkanCommandBuffer.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanFrameBuffer.h"
#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/VulkanIndexBuffer.h"
#include "spark/render/vk/VulkanPipeline.h"
#include "spark/render/vk/VulkanPipelineLayout.h"
#include "spark/render/vk/VulkanPushConstantsLayout.h"
#include "spark/render/vk/VulkanQueue.h"
#include "spark/render/vk/VulkanRenderPass.h"
#include "spark/render/vk/VulkanVertexBuffer.h"

#include "spark/base/Exception.h"

#include <optional>

namespace spark::render::vk
{
    struct VulkanCommandBuffer::Impl
    {
        friend class VulkanCommandBuffer;

    public:
        explicit Impl(const VulkanQueue& queue)
            : m_queue(queue) {}

        VkCommandBuffer initialize(const bool is_primary)
        {
            // Secondary command buffers have their own command pool.
            if (!is_primary)
            {
                const VkCommandPoolCreateInfo pool_info = {
                    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                    .queueFamilyIndex = m_queue.queueId()
                };

                VkCommandPool command_pool = {};
                if (vkCreateCommandPool(m_queue.device().handle(), &pool_info, nullptr, &command_pool) != VK_SUCCESS)
                    throw base::NullPointerException("Failed to create command pool");

                m_commandPool = command_pool;
                m_secondary = true;
            }

            const VkCommandBufferAllocateInfo buffer_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = is_primary ? m_queue.commandPool() : m_commandPool.value(),
                .level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY,
                .commandBufferCount = 1
            };

            VkCommandBuffer command_buffer = {};
            if (vkAllocateCommandBuffers(m_queue.device().handle(), &buffer_info, &command_buffer) != VK_SUCCESS)
                throw base::NullPointerException("Failed to allocate command buffer");

            return command_buffer;
        }

    private:
        const VulkanQueue& m_queue;
        const VulkanPipelineState* m_lastPipeline = nullptr;
        bool m_recording = false, m_secondary = false;
        std::optional<VkCommandPool> m_commandPool;
        std::vector<std::shared_ptr<const IStateResource>> m_sharedResources;
    };

    VulkanCommandBuffer::VulkanCommandBuffer(const VulkanQueue& queue, const bool begin_recording, const bool is_primary)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(queue))
    {
        if (!queue.isBound())
            throw base::BadArgumentException("You must bind the queue before creating a command buffer from it.");

        handle() = m_impl->initialize(is_primary);

        if (begin_recording)
            begin();
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        if (!m_impl->m_commandPool.has_value())
        {
            vkFreeCommandBuffers(m_impl->m_queue.device().handle(), m_impl->m_queue.commandPool(), 1, &this->handle());
            return;
        }

        vkFreeCommandBuffers(m_impl->m_queue.device().handle(), m_impl->m_commandPool.value(), 1, &this->handle());
        vkDestroyCommandPool(m_impl->m_queue.device().handle(), m_impl->m_commandPool.value(), nullptr);
    }

    void VulkanCommandBuffer::begin() const
    {
        constexpr VkCommandBufferBeginInfo begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        };

        if (vkBeginCommandBuffer(handle(), &begin_info) != VK_SUCCESS)
            throw base::NullPointerException("Failed to begin command buffer recording");
        m_impl->m_recording = true;
    }

    void VulkanCommandBuffer::begin(const VulkanRenderPass& render_pass) const
    {
        // Create an inheritance info for the parent buffer.
        const VkCommandBufferInheritanceInfo inheritance_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
            .pNext = nullptr,
            .renderPass = render_pass.handle(),
            .subpass = 0,
            .framebuffer = render_pass.activeFrameBuffer().handle(),
            .occlusionQueryEnable = false
        };

        // Set the buffer into recording state.
        const VkCommandBufferBeginInfo begin_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
            .pInheritanceInfo = &inheritance_info
        };

        if (vkBeginCommandBuffer(handle(), &begin_info) != VK_SUCCESS)
            throw base::NullPointerException("Failed to begin command buffer recording");
        m_impl->m_recording = true;
    }

    void VulkanCommandBuffer::end() const
    {
        if (!m_impl->m_recording)
            return;

        if (vkEndCommandBuffer(handle()) != VK_SUCCESS)
            throw base::NullPointerException("Failed to end command buffer recording");
        m_impl->m_recording = false;
    }

    bool VulkanCommandBuffer::isRecording() const noexcept
    {
        return m_impl->m_recording;
    }

    bool VulkanCommandBuffer::isSecondary() const noexcept
    {
        return m_impl->m_secondary;
    }

    void VulkanCommandBuffer::transfer(IVulkanBuffer& source, IVulkanBuffer& target, const unsigned source_element, const unsigned target_element, const unsigned elements) const
    {
        if (source.elements() < source_element + elements)
            throw
                    base::ArgumentOutOfRangeException(std::format("The source buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.",
                                                                  source.elements(),
                                                                  elements,
                                                                  source_element));

        if (target.elements() < target_element + elements)
            throw
                    base::ArgumentOutOfRangeException(std::format("The target buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.",
                                                                  target.elements(),
                                                                  elements,
                                                                  source_element));

        const VkBufferCopy copy_region = {
            .srcOffset = source_element * source.alignedElementSize(),
            .dstOffset = target_element * target.alignedElementSize(),
            .size = elements * source.alignedElementSize()
        };

        vkCmdCopyBuffer(handle(), std::as_const(source).handle(), std::as_const(target).handle(), 1, &copy_region);
    }

    void VulkanCommandBuffer::transfer(IVulkanImage& source,
                                       IVulkanBuffer& target,
                                       const unsigned first_subresource,
                                       const unsigned target_element,
                                       unsigned subresources) const
    {
        if (source.elements() < first_subresource + subresources)
            throw
                    base::ArgumentOutOfRangeException(std::format("The source image has only {0} sub-resources, but a transfer for {1} sub-resources starting from sub-resource {2} has been requested.",
                                                                  source.elements(),
                                                                  subresources,
                                                                  first_subresource));

        if (target.elements() <= target_element + subresources)
            throw
                    base::ArgumentOutOfRangeException(std::format("The target buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.",
                                                                  target.elements(),
                                                                  subresources,
                                                                  target_element));

        // Create a copy command and add it to the command buffer.
        std::vector<VkBufferImageCopy> copy_infos(subresources);
        std::ranges::generate(copy_infos,
                              [&, i = target_element]() mutable
                              {
                                  const unsigned subresource = i++;
                                  auto [layer, level, plane] = source.resolveSubresource(subresource);

                                  const auto source_extent = source.extent().castTo<unsigned>();
                                  return VkBufferImageCopy {
                                      .bufferOffset = target.alignedElementSize() * subresource,
                                      .bufferRowLength = 0,
                                      .bufferImageHeight = 0,
                                      .imageSubresource = VkImageSubresourceLayers {
                                          .aspectMask = source.aspectMask(plane),
                                          .mipLevel = level,
                                          .baseArrayLayer = layer,
                                          .layerCount = 1
                                      },
                                      .imageOffset = {0, 0, 0},
                                      .imageExtent = {source_extent.x, source_extent.y, source_extent.z}
                                  };
                              });

        vkCmdCopyImageToBuffer(this->handle(),
                               std::as_const(source).handle(),
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               std::as_const(target).handle(),
                               subresources,
                               copy_infos.data());
    }

    void VulkanCommandBuffer::transfer(IVulkanBuffer& source, IVulkanImage& target, unsigned source_element, unsigned first_subresource, unsigned elements) const
    {
        if (source.elements() < source_element + elements)
            throw
                    base::ArgumentOutOfRangeException(std::format("The source buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.",
                                                                  source.elements(),
                                                                  elements,
                                                                  source_element));

        if (target.elements() < first_subresource + elements)
            throw
                    base::ArgumentOutOfRangeException(std::format("The target image has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.",
                                                                  target.elements(),
                                                                  elements,
                                                                  source_element));

        // TODO: Use a barrier to block the memory access during the transfer.

        std::vector<VkBufferImageCopy> copy_regions(elements);
        std::ranges::generate(copy_regions,
                              [&, i = first_subresource]() mutable
                              {
                                  const auto [plane, layer, level] = target.resolveSubresource(i++);

                                  const auto target_extent = target.extent().castTo<unsigned>();
                                  return VkBufferImageCopy {
                                      .bufferOffset = source.alignedElementSize() * source_element,
                                      .bufferRowLength = 0,
                                      .bufferImageHeight = 0,
                                      .imageSubresource = VkImageSubresourceLayers {
                                          .aspectMask = target.aspectMask(plane),
                                          .mipLevel = level,
                                          .baseArrayLayer = layer,
                                          .layerCount = 1
                                      },
                                      .imageOffset = {0, 0, 0},
                                      .imageExtent = {target_extent.x, target_extent.y, target_extent.z}
                                  };
                              });

        vkCmdCopyBufferToImage(handle(), std::as_const(source).handle(), std::as_const(target).handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, elements, copy_regions.data());
    }

    void VulkanCommandBuffer::transfer(IVulkanImage& source,
                                       IVulkanImage& target,
                                       unsigned source_subresource,
                                       unsigned target_subresource,
                                       unsigned subresources) const
    {
        if (source.elements() < source_subresource + subresources)
            throw
                    base::ArgumentOutOfRangeException(std::format("The source image has only {0} sub-resources, but a transfer for {1} sub-resources starting from sub-resource {2} has been requested.",
                                                                  source.elements(),
                                                                  subresources,
                                                                  source_subresource));

        if (target.elements() < target_subresource + subresources)
            throw
                    base::ArgumentOutOfRangeException(std::format("The target image has only {0} sub-resources, but a transfer for {1} sub-resources starting from sub-resources {2} has been requested.",
                                                                  target.elements(),
                                                                  subresources,
                                                                  target_subresource));

        // TODO: Use a barrier to block the memory access during the transfer.

        std::vector<VkImageCopy> copy_regions(subresources);
        std::ranges::generate(copy_regions,
                              [&, i = 0]() mutable
                              {
                                  auto [source_plane, source_layer, source_level] = source.resolveSubresource(source_subresource + i);
                                  auto [target_plane, target_layer, target_level] = target.resolveSubresource(target_subresource + i);
                                  i++;

                                  const auto source_extent = source.extent().castTo<unsigned>();
                                  const auto target_extent = target.extent().castTo<unsigned>();
                                  return VkImageCopy {
                                      .srcSubresource = VkImageSubresourceLayers {
                                          .aspectMask = source.aspectMask(source_plane),
                                          .mipLevel = source_level,
                                          .baseArrayLayer = source_layer,
                                          .layerCount = 1
                                      },
                                      .srcOffset = {0, 0, 0},
                                      .dstSubresource = VkImageSubresourceLayers {
                                          .aspectMask = target.aspectMask(target_plane),
                                          .mipLevel = target_level,
                                          .baseArrayLayer = target_layer,
                                          .layerCount = 1
                                      },
                                      .dstOffset = {0, 0, 0},
                                      .extent = {
                                          std::min(source_extent.x, target_extent.x),
                                          std::min(source_extent.y, target_extent.y),
                                          std::min(source_extent.z, target_extent.z)
                                      }
                                  };
                              });
        vkCmdCopyImage(handle(),
                       std::as_const(source).handle(),
                       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                       std::as_const(target).handle(),
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       subresources,
                       copy_regions.data());
    }

    void VulkanCommandBuffer::transfer(std::shared_ptr<IVulkanBuffer> source,
                                       IVulkanBuffer& target,
                                       const unsigned source_element,
                                       const unsigned target_element,
                                       const unsigned elements) const
    {
        transfer(*source, target, source_element, target_element, elements);
        m_impl->m_sharedResources.push_back(std::move(source));
    }

    void VulkanCommandBuffer::transfer(std::shared_ptr<IVulkanBuffer> source,
                                       IVulkanImage& target,
                                       const unsigned source_element,
                                       const unsigned first_subresource,
                                       const unsigned elements) const
    {
        transfer(*source, target, source_element, first_subresource, elements);
        m_impl->m_sharedResources.push_back(std::move(source));
    }

    void VulkanCommandBuffer::transfer(std::shared_ptr<IVulkanImage> source,
                                       IVulkanImage& target,
                                       const unsigned first_subresource,
                                       const unsigned target_element,
                                       const unsigned subresources) const
    {
        transfer(*source, target, first_subresource, target_element, subresources);
        m_impl->m_sharedResources.push_back(std::move(source));
    }

    void VulkanCommandBuffer::transfer(std::shared_ptr<IVulkanImage> source,
                                       IVulkanBuffer& target,
                                       const unsigned first_subresource,
                                       const unsigned target_element,
                                       const unsigned subresources) const
    {
        transfer(*source, target, first_subresource, target_element, subresources);
        m_impl->m_sharedResources.push_back(std::move(source));
    }

    void VulkanCommandBuffer::use(const VulkanPipelineState& pipeline) const noexcept
    {
        m_impl->m_lastPipeline = &pipeline;
        pipeline.use(*this);
    }

    void VulkanCommandBuffer::bind(const VulkanDescriptorSet& descriptor_set) const
    {
        if (m_impl->m_lastPipeline == nullptr)
            throw base::NullPointerException("You must use a pipeline before binding a descriptor set.");

        m_impl->m_lastPipeline->bind(*this, descriptor_set);
    }

    void VulkanCommandBuffer::bind(const VulkanDescriptorSet& descriptor_set, const VulkanPipelineState& pipeline) const noexcept
    {
        pipeline.bind(*this, descriptor_set);
    }

    void VulkanCommandBuffer::bind(const IVulkanIndexBuffer& index_buffer) const noexcept
    {
        vkCmdBindIndexBuffer(this->handle(), index_buffer.handle(), 0, index_buffer.layout().indexType() == IndexType::UInt16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
    }

    void VulkanCommandBuffer::bind(const IVulkanVertexBuffer& vertex_buffer) const noexcept
    {
        constexpr VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(this->handle(), vertex_buffer.layout().binding(), 1, &vertex_buffer.handle(), offsets);
    }

    void VulkanCommandBuffer::draw(const unsigned vertices, const unsigned instances, const unsigned first_vertex, const unsigned first_instance) const noexcept
    {
        vkCmdDraw(handle(), vertices, instances, first_vertex, first_instance);
    }

    void VulkanCommandBuffer::draw(const IVulkanVertexBuffer& vertex_buffer,
                                   const unsigned instances,
                                   const unsigned first_vertex,
                                   const unsigned first_instance) const noexcept
    {
        bind(vertex_buffer);
        draw(vertex_buffer.elements(), instances, first_vertex, first_instance);
    }

    void VulkanCommandBuffer::drawIndexed(const unsigned indices,
                                          const unsigned instances,
                                          const unsigned first_index,
                                          const int vertex_offset,
                                          const unsigned first_instance) const noexcept
    {
        vkCmdDrawIndexed(handle(), indices, instances, first_index, vertex_offset, first_instance);
    }

    void VulkanCommandBuffer::drawIndexed(const IVulkanIndexBuffer& index_buffer,
                                          const unsigned instances,
                                          const unsigned first_index,
                                          const int vertex_offset,
                                          const unsigned first_instance) const noexcept
    {
        bind(index_buffer);
        drawIndexed(index_buffer.elements(), instances, first_index, vertex_offset, first_instance);
    }

    void VulkanCommandBuffer::drawIndexed(const IVulkanVertexBuffer& vertex_buffer,
                                          const IVulkanIndexBuffer& index_buffer,
                                          const unsigned instances,
                                          const unsigned first_index,
                                          const int vertex_offset,
                                          const unsigned first_instance) const noexcept
    {
        bind(vertex_buffer);
        bind(index_buffer);
        drawIndexed(index_buffer.elements(), instances, first_index, vertex_offset, first_instance);
    }

    void VulkanCommandBuffer::dispatch() const noexcept
    {
        // TODO: Maybe use multiple threads for dispatching.
        vkCmdDispatch(handle(), 1, 1, 1);
    }

    void VulkanCommandBuffer::execute(const std::shared_ptr<const VulkanCommandBuffer> command_buffer) const
    {
        vkCmdExecuteCommands(handle(), 1, &command_buffer->handle());
    }

    void VulkanCommandBuffer::execute(const std::vector<std::shared_ptr<const VulkanCommandBuffer>>& command_buffers) const
    {
        std::vector<VkCommandBuffer> handles(command_buffers.size());
        for (const auto& command_buffer : command_buffers)
            handles.push_back(command_buffer->handle());

        vkCmdExecuteCommands(handle(), static_cast<unsigned>(handles.size()), handles.data());
    }

    void VulkanCommandBuffer::pushConstants(const VulkanPushConstantsLayout& layout, const void* const memory) const noexcept
    {
        for (const auto& range : layout.ranges())
            vkCmdPushConstants(this->handle(), layout.pipelineLayout().handle(), conversions::to_shader_stage(range->stage()), range->offset(), range->size(), memory);
    }

    void VulkanCommandBuffer::setViewport(const IViewport* viewport) const noexcept
    {
        const VkViewport vk_viewport = {
            .x = viewport->rectangle().position.x,
            .y = viewport->rectangle().position.y,
            .width = viewport->rectangle().extent.x,
            .height = viewport->rectangle().extent.y,
            .minDepth = viewport->minDepth(),
            .maxDepth = viewport->maxDepth()
        };
        vkCmdSetViewportWithCount(this->handle(), 1, &vk_viewport);
    }

    void VulkanCommandBuffer::setViewports(std::span<const IViewport*> viewports) const noexcept
    {
        std::vector<VkViewport> vk_viewports;
        std::ranges::transform(viewports,
                               vk_viewports.begin(),
                               [](const IViewport* viewport)
                               {
                                   return VkViewport {
                                       .x = viewport->rectangle().position.x,
                                       .y = viewport->rectangle().position.y,
                                       .width = viewport->rectangle().extent.x,
                                       .height = viewport->rectangle().extent.y,
                                       .minDepth = viewport->minDepth(),
                                       .maxDepth = viewport->maxDepth()
                                   };
                               });

        vkCmdSetViewportWithCount(this->handle(), static_cast<unsigned>(vk_viewports.size()), vk_viewports.data());
    }

    void VulkanCommandBuffer::setScissor(const IScissor* scissor) const noexcept
    {
        const auto position = scissor->rectangle().position.castTo<int>();
        const auto extent = scissor->rectangle().extent.castTo<unsigned>();

        const VkRect2D vk_scissor = {
            .offset = {position.x, position.y},
            .extent = {extent.x, extent.y}
        };
        vkCmdSetScissorWithCount(handle(), 1, &vk_scissor);
    }

    void VulkanCommandBuffer::setScissors(const std::span<const IScissor*> scissors) const noexcept
    {
        std::vector<VkRect2D> vk_scissors(scissors.size());
        for (const auto& scissor : scissors)
        {
            const auto position = scissor->rectangle().position.castTo<int>();
            const auto extent = scissor->rectangle().extent.castTo<unsigned>();

            vk_scissors.push_back(VkRect2D {
                                      .offset = {position.x, position.y},
                                      .extent = {extent.x, extent.y}
                                  });
        }

        vkCmdSetScissorWithCount(handle(), static_cast<unsigned>(vk_scissors.size()), vk_scissors.data());
    }

    void VulkanCommandBuffer::setBlendFactors(const math::Vector4<float>& blend_factors) const noexcept
    {
        const std::array factors = {blend_factors.x, blend_factors.y, blend_factors.z, blend_factors.w};
        vkCmdSetBlendConstants(handle(), factors.data());
    }

    void VulkanCommandBuffer::setStencilRef(const unsigned stencil_ref) const noexcept
    {
        vkCmdSetStencilReference(handle(), VK_STENCIL_FRONT_AND_BACK, stencil_ref);
    }

    void VulkanCommandBuffer::releaseSharedState() const
    {
        m_impl->m_sharedResources.clear();
    }
}
