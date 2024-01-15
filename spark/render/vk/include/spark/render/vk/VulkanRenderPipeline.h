#pragma once

#include "spark/base/Macros.h"
#include "spark/render/RenderPipeilne.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanInputAssembler.h"
#include "spark/render/vk/VulkanPipeline.h"
#include "spark/render/vk/VulkanRasterizer.h"

namespace spark::render::vk
{
    class VulkanShaderProgram;
    class VulkanPipelineLayout;
    class VulkanRenderPass;
    class VulkanDescriptorSet;

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanRenderPipeline': inherits 'StateResource::name' via dominance

    /**
     * \brief Vulkan implementation of \ref IRenderPipeline.
     */
    class SPARK_RENDER_VK_EXPORT VulkanRenderPipeline final : public RenderPipeline<VulkanPipelineLayout, VulkanShaderProgram, VulkanInputAssembler, VulkanRasterizer>,
                                                              public VulkanPipelineState
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanRenderPipeline.
         * \param render_pass The parent \ref VulkanRenderPass.
         * \param shader_program The \ref VulkanShaderProgram used by this pipeline.
         * \param layout The \ref VulkanPipelineLayout of the pipeline.
         * \param input_assembler The \ref VulkanInputAssembler of the pipeline.
         * \param rasterizer The \ref VulkanRasterizer of the pipeline.
         * \param name The optional name of the render pipeline.
         * \param enable_alpha_to_coverage `true` if alpha to coverage multisampling should be enabled, `false` otherwise.
         */
        explicit VulkanRenderPipeline(const VulkanRenderPass& render_pass,
                                      std::shared_ptr<VulkanShaderProgram> shader_program,
                                      std::shared_ptr<VulkanPipelineLayout> layout,
                                      std::shared_ptr<VulkanInputAssembler> input_assembler,
                                      std::shared_ptr<VulkanRasterizer> rasterizer,
                                      bool enable_alpha_to_coverage = false,
                                      const std::string& name = "");
        ~VulkanRenderPipeline() override;

        VulkanRenderPipeline(const VulkanRenderPipeline& other) = delete;
        VulkanRenderPipeline(VulkanRenderPipeline&& other) noexcept = delete;
        VulkanRenderPipeline& operator=(const VulkanRenderPipeline& other) = delete;
        VulkanRenderPipeline& operator=(VulkanRenderPipeline&& other) noexcept = delete;

        /// \copydoc IRenderPipeline::program()
        [[nodiscard]] std::shared_ptr<const VulkanShaderProgram> program() const noexcept override;

        /// \copydoc IRenderPipeline::layout()
        [[nodiscard]] std::shared_ptr<VulkanPipelineLayout> layout() const noexcept override;

        /// \copydoc IRenderPipeline::alphaToCoverage()
        [[nodiscard]] bool alphaToCoverage() const noexcept override;

        /// \copydoc IRenderPipeline::inputAssembler()
        [[nodiscard]] std::shared_ptr<VulkanInputAssembler> inputAssembler() const noexcept override;

        /// \copydoc IRenderPipeline::rasterizer()
        [[nodiscard]] std::shared_ptr<VulkanRasterizer> rasterizer() const noexcept override;

        /// \copydoc VulkanPipelineState::use()
        void use(const VulkanCommandBuffer& command_buffer) const noexcept override;

        /// \copydoc VulkanPipelineState::bind()
        void bind(const VulkanCommandBuffer& command_buffer, const VulkanDescriptorSet& descriptor_set) const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
