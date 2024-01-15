#pragma once

#include "spark/render/RenderPass.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanFrameBuffer.h"
#include "spark/render/vk/VulkanRenderPipeline.h"

#include <span>

SPARK_FWD_DECLARE_VK_HANDLE(VkRenderPass)

namespace spark::render::vk
{
    class VulkanDescriptorSet;
    class VulkanInputAttachmentMapping;
    class VulkanDevice;

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanRenderPass': inherits 'StateResource::name' via dominance

    /**
     * \brief Vulkan implementation of \ref IRenderPass.
     */
    class SPARK_RENDER_VK_EXPORT VulkanRenderPass final : public RenderPass<VulkanRenderPipeline, VulkanFrameBuffer, VulkanInputAttachmentMapping>, public Resource<VkRenderPass>
    {
    public:
        /**
         * \brief Initializes a \ref VulkanRenderPass.
         * \param device The parent \ref VulkanDevice instance.
         * \param render_targets The \link RenderTarget render targets \endlink that are output by the \ref IRenderPass.
         * \param command_buffers The number of \ref ICommandBuffer in each \ref IFrameBuffer. 
         * \param samples The number of samples to use for multi sampling.
         * \param input_attachments The input attachments that are read in the \ref IRenderPass.
         */
        explicit VulkanRenderPass(const VulkanDevice& device,
                                  std::span<RenderTarget> render_targets,
                                  unsigned command_buffers = 1,
                                  MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                  std::span<VulkanInputAttachmentMapping> input_attachments = {});

        /**
         * \brief Initializes a \ref VulkanRenderPass.
         * \param device The parent \ref VulkanDevice instance.
         * \param name The name of the \ref VulkanRenderPass state resource.
         * \param render_targets The \link RenderTarget render targets \endlink that are output by the \ref IRenderPass.
         * \param command_buffers The number of \ref ICommandBuffer in each \ref IFrameBuffer.
         * \param samples The number of samples to use for multi sampling.
         * \param input_attachments The input attachments that are read in the \ref IRenderPass.
         */
        explicit VulkanRenderPass(const VulkanDevice& device,
                                  const std::string& name,
                                  std::span<RenderTarget> render_targets,
                                  unsigned command_buffers = 1,
                                  MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                  std::span<VulkanInputAttachmentMapping> input_attachments = {});

        ~VulkanRenderPass() override;

        VulkanRenderPass(const VulkanRenderPass& other) = delete;
        VulkanRenderPass(VulkanRenderPass&& other) noexcept = delete;
        VulkanRenderPass& operator=(const VulkanRenderPass& other) = delete;
        VulkanRenderPass& operator=(VulkanRenderPass&& other) noexcept = delete;

        /**
         * \brief Gets the parent \ref VulkanDevice that provides the queue.
         * \return The parent \ref VulkanDevice that provides the queue.
         */
        [[nodiscard]] const VulkanDevice& device() const noexcept;

        /// \copydoc IRenderPass::begin()
        void begin(unsigned buffer) override;

        /// \copydoc IRenderPass::end()
        void end() const override;

        /// \copydoc IRenderPass::activeFrameBuffer()
        [[nodiscard]] const VulkanFrameBuffer& activeFrameBuffer() const override;

        /// \copydoc InputAttachmentMappingSource::frameBuffer()
        [[nodiscard]] const VulkanFrameBuffer& frameBuffer(unsigned buffer) const override;

        /// \copydoc IRenderPass::frameBuffers()
        [[nodiscard]] std::vector<const VulkanFrameBuffer*> frameBuffers() const noexcept override;

        /// \copydoc IRenderPass::resizeFrameBuffers()
        void resizeFrameBuffers(const math::Vector2<unsigned>& new_render_area) override;

        /// \copydoc IRenderPass::pipelines()
        [[nodiscard]] std::vector<const VulkanRenderPipeline*> pipelines() const noexcept override;

        /// \copydoc IRenderPass::renderTarget()
        [[nodiscard]] const RenderTarget& renderTarget(unsigned location) const override;

        /// \copydoc IRenderPass::renderTargets()
        [[nodiscard]] std::span<const RenderTarget> renderTargets() const noexcept override;

        /// \copydoc IRenderPass::hasPresentRenderTarget()
        [[nodiscard]] bool hasPresentRenderTarget() const noexcept override;

        /// \copydoc IRenderPass::multiSamplingLevel()
        [[nodiscard]] MultiSamplingLevel multiSamplingLevel() const noexcept override;

        ///// \copydoc RenderPass::inputAttachments()
        [[nodiscard]] std::span<const VulkanInputAttachmentMapping> inputAttachments() const noexcept override;

        /// \copydoc IRenderPass::updateAttachments
        void updateAttachments(const VulkanDescriptorSet& descriptor_set) const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
