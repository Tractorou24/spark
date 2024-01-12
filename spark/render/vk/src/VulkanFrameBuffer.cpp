#include "spark/render/vk/VulkanFrameBuffer.h"
#include "spark/render/Image.h"
#include "spark/render/RenderTarget.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/VulkanCommandBuffer.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/VulkanInputAttachmentMapping.h"
#include "spark/render/vk/VulkanQueue.h"
#include "spark/render/vk/VulkanRenderPass.h"

#include "spark/base/Exception.h"
#include "spark/log/Logger.h"
#include "spark/math/Vector2.h"

#include "vulkan/vulkan.h"

#include <algorithm>

namespace spark::render::vk
{
    struct VulkanFrameBuffer::Impl
    {
        friend class VulkanFrameBuffer;

    public:
        explicit Impl(const VulkanRenderPass& render_pass, const unsigned buffer_index, const math::Vector2<unsigned>& render_area, const unsigned command_buffers)
            : m_renderPass(render_pass), m_size(render_area), m_bufferIndex(buffer_index)
        {
            const auto& device = render_pass.device();

            // Create the semaphore
            constexpr VkSemaphoreCreateInfo semaphore_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            };

            if (vkCreateSemaphore(device.handle(), &semaphore_info, nullptr, &m_semaphore) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create semaphore");

            // Retrieve a command buffer from the graphics queue
            m_commandBuffers.resize(command_buffers);
            std::ranges::generate(m_commandBuffers, [&device] { return device.graphicsQueue().createCommandBuffer(false, true); });
        }

        ~Impl()
        {
            vkDestroySemaphore(m_renderPass.device().handle(), m_semaphore, nullptr);
        }

        VkFramebuffer initialize()
        {
            // Clear earlier images.
            m_renderTargetViews.clear();
            m_outputAttachments.clear();

            // Retrieve the image views for the input and output attachments from render targets of the parent render pass
            std::vector<VkImageView> attachments;
            const MultiSamplingLevel samples = m_renderPass.multiSamplingLevel();

            std::ranges::for_each(m_renderPass.inputAttachments(),
                                  [this, &attachments, i = 0u](const auto& attachment) mutable
                                  {
                                      if (attachment.location() != i)
                                          log::warning("Remapped input attachment from location {0} to location {1}. Please make sure that the input attachments are sorted within the render pass and do not have any gaps in their location mappings.",
                                                       attachment.location(),
                                                       i);

                                      if (attachment.renderTarget().type() == RenderTargetType::Present)
                                          throw
                                                  base::BadArgumentException(std::format("The input attachment mapped to location {0} is a present target, which cannot be used as input attachment.",
                                                                                         i));

                                      if (attachment.source() == nullptr)
                                          throw base::BadArgumentException(std::format("The input attachment mapped to location {0} has no initialized source.", i));

                                      // Store the image view from the source frame buffer.
                                      attachments.push_back(attachment.source()->frameBuffer(m_bufferIndex).image(i++).imageView());
                                  });

            std::ranges::for_each(m_renderPass.renderTargets(),
                                  [&, i = 0u](const RenderTarget& render_target) mutable
                                  {
                                      if (render_target.location() != i++)
                                          log::warning("Remapped render target from location {0} to location {1}. Please make sure that the render targets are sorted within the render pass and do not have any gaps in their location mappings.",
                                                       render_target.location(),
                                                       i - 1);

                                      if (render_target.type() == RenderTargetType::Present && samples == MultiSamplingLevel::X1)
                                      {
                                          // If the render target is a present target, acquire an image view from the swap chain
                                          const auto image = m_renderPass.device().swapChain().image(m_bufferIndex);
                                          m_renderTargetViews.push_back(image);
                                          attachments.push_back(image->imageView());
                                      } else
                                      {
                                          // Create an image view for the render target
                                          auto image = m_renderPass.device().factory().createAttachment(render_target.format(), m_size, samples);
                                          attachments.push_back(image->imageView());
                                          m_renderTargetViews.push_back(image.get());
                                          m_outputAttachments.push_back(std::move(image));
                                      }
                                  });

            // If we have a present target and multi sampling is enabled, add a view for the resolve attachment
            const auto has_present_target = std::ranges::any_of(m_renderPass.renderTargets(),
                                                                [](const RenderTarget& render_target) { return render_target.type() == RenderTargetType::Present; });
            if (samples > MultiSamplingLevel::X1 && has_present_target)
            {
                const auto image = m_renderPass.device().swapChain().image(m_bufferIndex);
                m_renderTargetViews.push_back(image);
                attachments.push_back(image->imageView());
            }

            // Allocate the frame buffer
            const auto render_area_size = m_size.castTo<unsigned>();
            const VkFramebufferCreateInfo frame_buffer_info =
            {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = m_renderPass.handle(),
                .attachmentCount = static_cast<unsigned>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = render_area_size.x,
                .height = render_area_size.y,
                .layers = 1,
            };

            VkFramebuffer frame_buffer = VK_NULL_HANDLE;
            if (vkCreateFramebuffer(m_renderPass.device().handle(), &frame_buffer_info, nullptr, &frame_buffer) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create frame buffer");
            return frame_buffer;
        }

    private:
        const VulkanRenderPass& m_renderPass;
        std::vector<std::unique_ptr<IVulkanImage>> m_outputAttachments;
        std::vector<const IVulkanImage*> m_renderTargetViews;
        std::vector<std::shared_ptr<VulkanCommandBuffer>> m_commandBuffers;

        VkSemaphore m_semaphore;
        math::Vector2<unsigned> m_size;
        std::size_t m_lastFence = 0;
        unsigned m_bufferIndex = 0;
    };

    VulkanFrameBuffer::VulkanFrameBuffer(const VulkanRenderPass& render_pass, unsigned buffer_index, const math::Vector2<unsigned>& render_area, unsigned command_buffers)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(render_pass, buffer_index, render_area, command_buffers))
    {
        handle() = m_impl->initialize();
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        vkDestroyFramebuffer(m_impl->m_renderPass.device().handle(), handle(), nullptr);
    }

    const VkSemaphore& VulkanFrameBuffer::semaphore() const noexcept
    {
        return m_impl->m_semaphore;
    }

    std::size_t& VulkanFrameBuffer::lastFence() const noexcept
    {
        return m_impl->m_lastFence;
    }

    unsigned VulkanFrameBuffer::bufferIndex() const noexcept
    {
        return m_impl->m_bufferIndex;
    }

    spark::math::Vector2<unsigned> VulkanFrameBuffer::size() const noexcept
    {
        return m_impl->m_size;
    }

    void VulkanFrameBuffer::resize(const spark::math::Vector2<unsigned>& render_area)
    {
        // Destroy the old frame buffer
        vkDestroyFramebuffer(m_impl->m_renderPass.device().handle(), handle(), nullptr);

        // Recreate the frame buffer
        m_impl->m_size = render_area;
        handle() = m_impl->initialize();
    }

    std::vector<std::shared_ptr<const VulkanCommandBuffer>> VulkanFrameBuffer::commandBuffers() const noexcept
    {
        std::vector<std::shared_ptr<const VulkanCommandBuffer>> buffers;
        buffers.reserve(m_impl->m_commandBuffers.size());
        std::ranges::transform(m_impl->m_commandBuffers, std::back_inserter(buffers), [](const auto& buffer) { return buffer; });
        return buffers;
    }

    std::shared_ptr<const VulkanCommandBuffer> VulkanFrameBuffer::commandBuffer(const unsigned index) const
    {
        if (index >= m_impl->m_commandBuffers.size())
            throw base::ArgumentOutOfRangeException("Index out of bounds");

        return m_impl->m_commandBuffers[index];
    }

    std::vector<const IVulkanImage*> VulkanFrameBuffer::images() const noexcept
    {
        return m_impl->m_renderTargetViews;
    }

    const IVulkanImage& VulkanFrameBuffer::image(const unsigned location) const
    {
        if (location >= m_impl->m_renderTargetViews.size())
            throw base::ArgumentOutOfRangeException("Index out of bounds");

        return *m_impl->m_renderTargetViews[location];
    }
}
