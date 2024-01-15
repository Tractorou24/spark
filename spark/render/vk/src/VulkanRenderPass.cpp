#include "spark/render/vk/VulkanRenderPass.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanCommandBuffer.h"
#include "spark/render/vk/VulkanDescriptorSet.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanFrameBuffer.h"
#include "spark/render/vk/VulkanInputAttachmentMapping.h"
#include "spark/render/vk/VulkanQueue.h"

#include "spark/base/Exception.h"
#include "spark/log/Logger.h"

#include <algorithm>
#include <optional>

namespace spark::render::vk
{
    struct VulkanRenderPass::Impl
    {
        friend class VulkanRenderPass;

    public:
        explicit Impl(VulkanRenderPass* parent,
                      const VulkanDevice& device,
                      const std::span<RenderTarget> render_targets,
                      const MultiSamplingLevel samples,
                      const std::span<VulkanInputAttachmentMapping> input_attachments)
            : m_parent(parent), m_device(device), m_samples(samples)
        {
            mapRenderTargets(render_targets);
            mapInputAttachments(input_attachments);
        }

        void mapRenderTargets(std::span<RenderTarget> render_targets)
        {
            m_renderTargets.assign(std::begin(render_targets), std::end(render_targets));
            std::ranges::sort(m_renderTargets, [](const RenderTarget& a, const RenderTarget& b) { return a.location() < b.location(); });
        }

        void mapInputAttachments(std::span<VulkanInputAttachmentMapping> input_attachments)
        {
            m_inputAttachments.assign(std::begin(input_attachments), std::end(input_attachments));
            std::ranges::sort(m_inputAttachments, [](const VulkanInputAttachmentMapping& a, const VulkanInputAttachmentMapping& b) { return a.location() < b.location(); });
        }

        VkRenderPass initialize()
        {
            // Set up the attachments
            std::vector<VkAttachmentDescription> attachments;
            std::vector<VkAttachmentReference> input_attachments;
            std::vector<VkAttachmentReference> output_attachments;
            std::optional<VkAttachmentReference> depth_target, present_target;
            std::optional<VkAttachmentDescription> present_attachment;

            // Map input attachments
            std::ranges::for_each(m_inputAttachments,
                                  [&, this, i = 0](const VulkanInputAttachmentMapping& input_attachment) mutable
                                  {
                                      const std::size_t index = i++;

                                      if (input_attachment.location() != index)
                                          throw
                                                  base::ArgumentOutOfRangeException(std::format("No input attachment is mapped to location {0}. The locations must be within a contiguous domain.",
                                                                                                index));

                                      VkAttachmentDescription attachment_description = {
                                          .format = conversions::to_format(input_attachment.renderTarget().format()),
                                          .samples = conversions::to_samples(input_attachment.source()->multiSamplingLevel()),
                                          .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                          .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                          .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                          .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      };

                                      // Add a clear value, so the index matches the location
                                      m_clearValues.push_back({});

                                      switch (input_attachment.renderTarget().type())
                                      {
                                      case RenderTargetType::Present:
                                          {
                                              throw
                                                      base::BadArgumentException(std::format("The render pass input attachment at location {0} maps to a present render target, which can not be used as input attachment.",
                                                                                             index));
                                          }
                                      case RenderTargetType::Color:
                                          {
                                              attachment_description.initialLayout = attachment_description.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                                              input_attachments.push_back(VkAttachmentReference {
                                                                              .attachment = static_cast<unsigned>(index),
                                                                              .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                                          });
                                              attachments.push_back(attachment_description);
                                              break;
                                          }
                                      case RenderTargetType::DepthStencil:
                                          {
                                              if (helpers::has_depth(input_attachment.renderTarget().format()) &&
                                                  helpers::has_stencil(input_attachment.renderTarget().format()))
                                                  attachment_description.initialLayout = attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                                              else if (helpers::has_depth(input_attachment.renderTarget().format()))
                                                  attachment_description.initialLayout = attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
                                              else if (helpers::has_stencil(input_attachment.renderTarget().format()))
                                                  attachment_description.initialLayout = attachment_description.finalLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
                                              else
                                              {
                                                  log::warning("The depth/stencil input attachment at location {0} does not have a valid depth/stencil format ({1}). Falling back to VK_IMAGE_LAYOUT_GENERAL.",
                                                               index,
                                                               input_attachment.renderTarget().format());
                                                  attachment_description.initialLayout = attachment_description.finalLayout = VK_IMAGE_LAYOUT_GENERAL;
                                              }

                                              input_attachments.push_back(VkAttachmentReference {
                                                                              .attachment = static_cast<unsigned>(index),
                                                                              .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                                          });
                                              attachments.push_back(attachment_description);
                                              break;
                                          }
                                      };
                                  });

            // Map render targets
            std::ranges::for_each(m_renderTargets,
                                  [&, this, i = 0](const RenderTarget& render_target) mutable
                                  {
                                      const std::size_t index = i++;

                                      if (render_target.location() != index)
                                          throw
                                                  base::BadArgumentException(std::format("No render target is mapped to location {0}. The locations must be within a contiguous domain.",
                                                                                         index));

                                      if (render_target.type() == RenderTargetType::DepthStencil && depth_target.has_value())
                                          throw
                                                  base::BadArgumentException(std::format("The depth/stencil target at location {0} cannot be mapped. Another depth/stencil target is already bound to location {1} and only one is allowed.",
                                                                                         render_target.location(),
                                                                                         depth_target->attachment));
                                      if (render_target.type() == RenderTargetType::Present && present_attachment.has_value())
                                          throw
                                                  base::BadArgumentException(std::format("The present target at location {0} cannot be mapped. Another present target is already bound to location {1} and only one is allowed.",
                                                                                         render_target.location(),
                                                                                         present_target->attachment));
                                      VkAttachmentDescription attachment {
                                          .format = conversions::to_format(render_target.format()),
                                          .samples = conversions::to_samples(m_samples),
                                          .loadOp = render_target.clearBuffer() ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                          .storeOp = render_target.isVolatile() ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE,
                                          .stencilLoadOp = render_target.clearStencil() ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                          .stencilStoreOp = render_target.isVolatile() ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE,
                                      };

                                      // Add a clear values (even if it's unused).
                                      if (render_target.clearBuffer() || render_target.clearStencil())
                                          m_clearValues.push_back(VkClearValue {
                                                                      {
                                                                          {
                                                                              render_target.clearValues().x, render_target.clearValues().y,
                                                                              render_target.clearValues().z, render_target.clearValues().w
                                                                          }
                                                                      }
                                                                  });
                                      else
                                          m_clearValues.push_back({});

                                      switch (render_target.type())
                                      {
                                      case RenderTargetType::Color:
                                          {
                                              attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                                              attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                                              output_attachments.push_back({static_cast<unsigned>(index + input_attachments.size()), attachment.finalLayout});
                                              break;
                                          }
                                      case RenderTargetType::DepthStencil:
                                          {
                                              if (helpers::has_depth(render_target.format()) || helpers::has_stencil(render_target.format()))
                                                  attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                                              else if (helpers::has_depth(render_target.format()))
                                                  attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
                                              else if (helpers::has_stencil(render_target.format()))
                                                  attachment.finalLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
                                              else
                                              {
                                                  log::warning("The depth/stencil render target at location {0} does not have a valid depth/stencil format ({1}). Falling back to VK_IMAGE_LAYOUT_GENERAL.",
                                                               index,
                                                               render_target.format());
                                                  attachment.finalLayout = VK_IMAGE_LAYOUT_GENERAL;
                                              }

                                              attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                                              depth_target = VkAttachmentReference {
                                                  static_cast<unsigned>(index + input_attachments.size()), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                                              };
                                              break;
                                          }
                                      case RenderTargetType::Present:
                                          {
                                              attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

                                              // If we have a multi-sampled present attachment, we also need to attach a resolve attachment for it.
                                              if (m_samples == MultiSamplingLevel::X1)
                                                  attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                                              else
                                              {
                                                  attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                                                  present_attachment = VkAttachmentDescription {
                                                      .format = attachment.format,
                                                      .samples = VK_SAMPLE_COUNT_1_BIT,
                                                      .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                                      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                  };
                                              }

                                              present_target = VkAttachmentReference {
                                                  static_cast<unsigned>(index + input_attachments.size()), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                              };
                                              output_attachments.push_back(present_target.value());
                                              break;
                                          }
                                      }

                                      attachments.push_back(attachment);
                                  });

            // Set up the sub-pass
            VkSubpassDescription sub_pass =
            {
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .inputAttachmentCount = static_cast<unsigned>(input_attachments.size()),
                .pInputAttachments = input_attachments.data(),
                .colorAttachmentCount = static_cast<unsigned>(output_attachments.size()),
                .pColorAttachments = output_attachments.data(),
                .pResolveAttachments = nullptr,
                .pDepthStencilAttachment = depth_target.has_value() ? &depth_target.value() : nullptr,
            };

            // Add the resolve attachment
            const VkAttachmentReference resolve_attachment =
            {
                .attachment = static_cast<unsigned>(attachments.size()),
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };

            if (present_attachment.has_value())
            {
                sub_pass.pResolveAttachments = &resolve_attachment;
                attachments.push_back(present_attachment.value());
            }

            // Define an external sub-pass dependency, if there are input attachments to synchronize with
            std::vector<VkSubpassDependency> dependencies;
            if (!m_inputAttachments.empty())
            {
                constexpr VkSubpassDependency dependency = {
                    .srcSubpass = VK_SUBPASS_EXTERNAL,
                    .dstSubpass = 0,
                    .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                    .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                    VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT |
                    VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
                };
                dependencies.push_back(dependency);
            }

            // Setup render pass state
            const VkRenderPassCreateInfo render_pass_info = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .attachmentCount = static_cast<unsigned>(attachments.size()),
                .pAttachments = attachments.data(),
                .subpassCount = 1,
                .pSubpasses = &sub_pass,
                .dependencyCount = static_cast<unsigned>(dependencies.size()),
                .pDependencies = dependencies.data(),
            };

            VkRenderPass render_pass = VK_NULL_HANDLE;
            if (vkCreateRenderPass(m_device.handle(), &render_pass_info, nullptr, &render_pass) != VK_SUCCESS)
                throw base::NullPointerException("Unable to create render pass.");
            return render_pass;
        }

        void initializeFrameBuffers(unsigned command_buffers)
        {
            // Initialize the frame buffers
            m_frameBuffers.resize(m_device.swapChain().buffers());
            std::ranges::generate(m_frameBuffers,
                                  [this, &command_buffers, i = 0]() mutable
                                  {
                                      return std::make_unique<VulkanFrameBuffer>(*m_parent, i++, m_device.swapChain().renderArea(), command_buffers);
                                  });

            // Initialize the primary command buffers
            m_primaryCommandBuffers.resize(m_device.swapChain().buffers());
            std::ranges::generate(m_primaryCommandBuffers,
                                  [this]() mutable
                                  {
                                      return m_device.graphicsQueue().createCommandBuffer(false, false);
                                  });
        }

    private:
        VulkanRenderPass* m_parent;
        const VulkanDevice& m_device;

        unsigned m_backBuffer = 0;
        MultiSamplingLevel m_samples;

        const VulkanFrameBuffer* m_activeFrameBuffer = nullptr;
        std::vector<std::unique_ptr<VulkanFrameBuffer>> m_frameBuffers;
        std::vector<std::unique_ptr<VulkanRenderPipeline>> m_pipelines;
        std::vector<std::shared_ptr<VulkanCommandBuffer>> m_primaryCommandBuffers;
        std::shared_ptr<const VulkanCommandBuffer> m_activeCommandBuffer;
        std::vector<RenderTarget> m_renderTargets;
        std::vector<VulkanInputAttachmentMapping> m_inputAttachments;
        std::vector<VkClearValue> m_clearValues;
    };

    VulkanRenderPass::VulkanRenderPass(const VulkanDevice& device,
                                       std::span<RenderTarget> render_targets,
                                       const unsigned command_buffers,
                                       MultiSamplingLevel samples,
                                       std::span<VulkanInputAttachmentMapping> input_attachments)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(this, device, render_targets, samples, input_attachments))
    {
        handle() = m_impl->initialize();
        m_impl->initializeFrameBuffers(command_buffers);
    }

    VulkanRenderPass::VulkanRenderPass(const VulkanDevice& device,
                                       const std::string& name,
                                       const std::span<RenderTarget> render_targets,
                                       const unsigned command_buffers,
                                       const MultiSamplingLevel samples,
                                       const std::span<VulkanInputAttachmentMapping> input_attachments)
        : VulkanRenderPass(device, render_targets, command_buffers, samples, input_attachments)
    {
        if (!name.empty())
            StateResource::name() = name;
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        vkDestroyRenderPass(m_impl->m_device.handle(), handle(), nullptr);
    }

    const VulkanDevice& VulkanRenderPass::device() const noexcept
    {
        return m_impl->m_device;
    }

    void VulkanRenderPass::begin(unsigned buffer)
    {
        // Only begin, if we are currently not running.
        if (m_impl->m_activeFrameBuffer != nullptr)
            throw base::RenderPassAlreadyStartedException("Unable to begin a render pass, that is already running. End the current pass first.");

        // Set the active frame buffer
        if (buffer >= m_impl->m_frameBuffers.size())
            throw base::ArgumentOutOfRangeException(std::format("The buffer {0} does not exist in this render pass. The render pass only contains {1} frame buffers.",
                                                                buffer,
                                                                m_impl->m_frameBuffers.size()));

        auto* frame_buffer = m_impl->m_activeFrameBuffer = m_impl->m_frameBuffers[buffer].get();
        const auto command_buffer = m_impl->m_activeCommandBuffer = m_impl->m_primaryCommandBuffers[buffer];
        m_impl->m_backBuffer = buffer;

        // Begin the command recording on the frame buffers command buffer. Make sure it has not being executed anymore
        m_impl->m_device.graphicsQueue().waitFor(frame_buffer->lastFence());
        command_buffer->begin();

        // Begin the render pass
        const VkRenderPassBeginInfo render_pass_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = handle(),
            .framebuffer = frame_buffer->handle(),
            .renderArea = VkRect2D {
                .offset = {0, 0},
                .extent = VkExtent2D {
                    frame_buffer->size().x,
                    frame_buffer->size().y,
                },
            },
            .clearValueCount = static_cast<unsigned>(m_impl->m_clearValues.size()),
            .pClearValues = m_impl->m_clearValues.data(),
        };

        vkCmdBeginRenderPass(std::as_const(*command_buffer).handle(), &render_pass_info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

        // Begin the command buffers in the frame buffers
        for (auto cmd_buffer : frame_buffer->commandBuffers())
            cmd_buffer->begin(*this);
    }

    void VulkanRenderPass::end() const
    {
        // Check if we are currently running
        if (m_impl->m_activeFrameBuffer == nullptr)
            throw base::RenderPassNotStartedException("Unable to end a render pass, that is not running. Begin the render pass first.");

        const auto& frame_buffer = m_impl->m_activeFrameBuffer;
        const auto& command_buffer = m_impl->m_activeCommandBuffer;

        // End the render pass and the command buffer recording
        std::vector<VkCommandBuffer> secondary_command_buffers;
        std::ranges::transform(frame_buffer->commandBuffers(),
                               std::back_inserter(secondary_command_buffers),
                               [](auto cmd_buffer)
                               {
                                   cmd_buffer->end();
                                   return cmd_buffer->handle();
                               });

        vkCmdExecuteCommands(std::as_const(*command_buffer).handle(), static_cast<unsigned>(secondary_command_buffers.size()), secondary_command_buffers.data());
        vkCmdEndRenderPass(std::as_const(*command_buffer).handle());

        // Submit the command buffer
        if (!this->hasPresentRenderTarget())
            frame_buffer->lastFence() = m_impl->m_device.graphicsQueue().submit(command_buffer);
        else
        {
            // Draw the frame, if the result of the render pass it should be presented to the swap chain
            std::array<VkPipelineStageFlags, 1> wait_for_stages = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
            std::array<VkSemaphore, 1> wait_for_semaphores = {m_impl->m_device.swapChain().semaphore()};
            std::array<VkSemaphore, 1> signal_semaphores = {frame_buffer->semaphore()};
            frame_buffer->lastFence() = m_impl->m_device.graphicsQueue().submit(command_buffer, wait_for_semaphores, wait_for_stages, signal_semaphores);

            // Present the swap chain
            m_impl->m_device.swapChain().present(*frame_buffer);
        }

        // Reset the frame buffer
        m_impl->m_activeFrameBuffer = nullptr;
        m_impl->m_activeCommandBuffer = nullptr;
    }

    const VulkanFrameBuffer& VulkanRenderPass::activeFrameBuffer() const
    {
        if (!m_impl->m_activeFrameBuffer)
            throw base::NullPointerException("No frame buffer is active, since the render pass has not begun.");
        return *m_impl->m_activeFrameBuffer;
    }

    const VulkanFrameBuffer& VulkanRenderPass::frameBuffer(unsigned buffer) const
    {
        if (buffer >= m_impl->m_frameBuffers.size())
            throw base::ArgumentOutOfRangeException(std::format("The buffer {0} does not exist in this render pass. The render pass only contains {1} frame buffers.",
                                                                buffer,
                                                                m_impl->m_frameBuffers.size()));
        return *m_impl->m_frameBuffers[buffer];
    }

    std::vector<const VulkanFrameBuffer*> VulkanRenderPass::frameBuffers() const noexcept
    {
        std::vector<const VulkanFrameBuffer*> frame_buffers;
        frame_buffers.reserve(m_impl->m_frameBuffers.size());
        std::ranges::transform(m_impl->m_frameBuffers, std::back_inserter(frame_buffers), [](const auto& frame_buffer) { return frame_buffer.get(); });
        return frame_buffers;
    }

    void VulkanRenderPass::resizeFrameBuffers(const math::Vector2<unsigned>& new_render_area)
    {
        if (m_impl->m_activeFrameBuffer)
            throw base::NullPointerException("Unable to reset the frame buffers while the render pass is running. End the render pass first.");

        for (const auto& frame_buffer : m_impl->m_frameBuffers)
            frame_buffer->resize(new_render_area);
    }

    std::vector<const VulkanRenderPipeline*> VulkanRenderPass::pipelines() const noexcept
    {
        std::vector<const VulkanRenderPipeline*> pipelines;
        pipelines.reserve(m_impl->m_pipelines.size());
        std::ranges::transform(m_impl->m_pipelines, std::back_inserter(pipelines), [](const auto& pipeline) { return pipeline.get(); });
        return pipelines;
    }

    const RenderTarget& VulkanRenderPass::renderTarget(unsigned location) const
    {
        const auto match = std::ranges::find_if(m_impl->m_renderTargets, [location](const RenderTarget& render_target) { return render_target.location() == location; });
        if (match == m_impl->m_renderTargets.end())
            throw base::ArgumentOutOfRangeException(std::format("No render target is mapped to location {0} in this render pass.", location));
        return *match;
    }

    std::span<const RenderTarget> VulkanRenderPass::renderTargets() const noexcept
    {
        return m_impl->m_renderTargets;
    }

    bool VulkanRenderPass::hasPresentRenderTarget() const noexcept
    {
        return std::ranges::any_of(m_impl->m_renderTargets, [](const RenderTarget& render_target) { return render_target.type() == RenderTargetType::Present; });
    }

    MultiSamplingLevel VulkanRenderPass::multiSamplingLevel() const noexcept
    {
        return m_impl->m_samples;
    }

    std::span<const VulkanInputAttachmentMapping> VulkanRenderPass::inputAttachments() const noexcept
    {
        return m_impl->m_inputAttachments;
    }

    void VulkanRenderPass::updateAttachments(const VulkanDescriptorSet& descriptor_set) const
    {
        const unsigned back_buffer = m_impl->m_backBuffer;
        for (auto& input_attachment : m_impl->m_inputAttachments)
            descriptor_set.attach(input_attachment.location(), input_attachment.source()->frameBuffer(back_buffer).image(input_attachment.renderTarget().location()));
    }
}
