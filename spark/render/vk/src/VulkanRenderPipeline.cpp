#include "spark/render/vk/VulkanRenderPipeline.h"
#include "spark/render/RenderTarget.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanCommandBuffer.h"
#include "spark/render/vk/VulkanDescriptorSetLayout.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanInputAssembler.h"
#include "spark/render/vk/VulkanPipelineLayout.h"
#include "spark/render/vk/VulkanRasterizer.h"
#include "spark/render/vk/VulkanRenderPass.h"
#include "spark/render/vk/VulkanShaderProgram.h"
#include "spark/render/vk/VulkanVertexBufferLayout.h"

#include "spark/log/Logger.h"

#include "vulkan/vulkan.h"

namespace spark::render::vk
{
    struct VulkanRenderPipeline::Impl
    {
        friend class VulkanRenderPipeline;

    public:
        explicit Impl(const VulkanRenderPass& render_pass,
                      const bool alpha_to_coverage,
                      std::shared_ptr<VulkanPipelineLayout> layout,
                      std::shared_ptr<VulkanShaderProgram> shader_program,
                      std::shared_ptr<VulkanInputAssembler> input_assembler,
                      std::shared_ptr<VulkanRasterizer> rasterizer)
            : m_renderPass(render_pass), m_layout(std::move(layout)), m_program(std::move(shader_program)), m_inputAssembler(std::move(input_assembler)),
              m_rasterizer(std::move(rasterizer)), m_alphaToCoverage(alpha_to_coverage) {}


        VkPipeline initialize() const
        {
            // Set rasterizer state
            const auto& rasterizer = *m_rasterizer;
            const VkPipelineRasterizationStateCreateInfo rasterizer_state_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = conversions::to_polygon_mode(rasterizer.polygonMode()),
                .cullMode = conversions::to_cull_mode(rasterizer.cullMode()),
                .frontFace = rasterizer.cullOrder() == CullOrder::ClockWise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE,
                .depthBiasEnable = rasterizer.depthStencilState().depthBias().enable,
                .depthBiasConstantFactor = rasterizer.depthStencilState().depthBias().constantFactor,
                .depthBiasClamp = rasterizer.depthStencilState().depthBias().clamp,
                .depthBiasSlopeFactor = rasterizer.depthStencilState().depthBias().slopeFactor,
                .lineWidth = rasterizer.lineWidth(),
            };

            log::trace("Rasterizer state: {{ PolygonMode: {0}, CullMode: {1}, CullOrder: {2}, LineWidth: {3} }}",
                       rasterizer.polygonMode(),
                       rasterizer.cullMode(),
                       rasterizer.cullOrder(),
                       rasterizer.lineWidth());

            if (rasterizer.depthStencilState().depthBias().enable)
                log::trace("\tRasterizer depth bias: {{ Clamp: {0}, ConstantFactor: {1}, SlopeFactor: {2} }}",
                           rasterizer.depthStencilState().depthBias().clamp,
                           rasterizer.depthStencilState().depthBias().constantFactor,
                           rasterizer.depthStencilState().depthBias().slopeFactor);
            else
                log::trace("\tRasterizer depth bias disabled.");

            // Set primitive topology
            std::vector<VkVertexInputBindingDescription> vertex_input_bindings;
            std::vector<VkVertexInputAttributeDescription> vertex_input_attributes;

            log::trace("Input assembler state: {{ PrimitiveTopology: {0} }}", m_inputAssembler->topology());

            const VkPipelineInputAssemblyStateCreateInfo input_assembly_state_info =
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology = conversions::to_primitive_topology(m_inputAssembler->topology()),
                .primitiveRestartEnable = VK_FALSE,
            };

            // Parse vertex input descriptors
            for (const auto* layout : m_inputAssembler->vertexBufferLayouts())
            {
                const auto buffer_attributes = layout->attributes();
                const auto binding = layout->binding();

                log::trace("Defining vertex buffer layout {{ Attributes: {0}, Size: {1} bytes, Binding: {2} }}...",
                           buffer_attributes.size(),
                           layout->elementSize(),
                           binding);

                const VkVertexInputBindingDescription binding_description =
                {
                    .binding = binding,
                    .stride = static_cast<unsigned>(layout->elementSize()),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                };

                std::vector<VkVertexInputAttributeDescription> current_attributes;
                std::ranges::transform(buffer_attributes,
                                       std::back_inserter(current_attributes),
                                       [&binding](const BufferAttribute* attribute)
                                       {
                                           log::trace("\tAttribute: {{ Location: {0}, Offset: {1}, Format: {2} }}",
                                                      attribute->location(),
                                                      attribute->offset(),
                                                      attribute->format());

                                           const VkVertexInputAttributeDescription descriptor = {
                                               .location = attribute->location(),
                                               .binding = binding,
                                               .format = conversions::to_format(attribute->format()),
                                               .offset = attribute->offset(),
                                           };
                                           return descriptor;
                                       });

                vertex_input_attributes.insert(vertex_input_attributes.end(), current_attributes.begin(), current_attributes.end());
                vertex_input_bindings.push_back(binding_description);
            }

            // Define vertex input state
            const VkPipelineVertexInputStateCreateInfo input_state_info =
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .vertexBindingDescriptionCount = static_cast<unsigned>(vertex_input_bindings.size()),
                .pVertexBindingDescriptions = vertex_input_bindings.data(),
                .vertexAttributeDescriptionCount = static_cast<unsigned>(vertex_input_attributes.size()),
                .pVertexAttributeDescriptions = vertex_input_attributes.data(),
            };

            // Setup viewport state
            constexpr VkPipelineViewportStateCreateInfo viewport_state_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
            };

            // Setup dynamic state
            constexpr VkPipelineDynamicStateCreateInfo dynamic_state_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount = static_cast<unsigned>(s_dynamicStates.size()),
                .pDynamicStates = s_dynamicStates.data()
            };

            // Setup multisampling state
            const VkPipelineMultisampleStateCreateInfo multisampling_state_info =
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples = conversions::to_samples(m_renderPass.multiSamplingLevel()),
                .sampleShadingEnable = VK_FALSE,
                .minSampleShading = 1.0f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = m_alphaToCoverage,
                .alphaToOneEnable = VK_FALSE,
            };

            // Setup color blend state
            std::vector<VkPipelineColorBlendAttachmentState> color_blend_attachments;
            for (const auto& render_target : m_renderPass.renderTargets())
            {
                if (render_target.type() == RenderTargetType::DepthStencil)
                    break;

                const VkPipelineColorBlendAttachmentState blend_attachment_state = {
                    .blendEnable = render_target.blendState().enable,
                    .srcColorBlendFactor = conversions::to_blend_factor(render_target.blendState().sourceColor),
                    .dstColorBlendFactor = conversions::to_blend_factor(render_target.blendState().destinationColor),
                    .colorBlendOp = conversions::to_blend_operation(render_target.blendState().colorOperation),
                    .srcAlphaBlendFactor = conversions::to_blend_factor(render_target.blendState().sourceAlpha),
                    .dstAlphaBlendFactor = conversions::to_blend_factor(render_target.blendState().destinationAlpha),
                    .alphaBlendOp = conversions::to_blend_operation(render_target.blendState().alphaOperation),
                    .colorWriteMask = static_cast<VkColorComponentFlags>(render_target.blendState().writeMask)
                };
                color_blend_attachments.push_back(blend_attachment_state);
            }

            const VkPipelineColorBlendStateCreateInfo color_blending_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = static_cast<unsigned>(color_blend_attachments.size()),
                .pAttachments = color_blend_attachments.data(),
            };

            // Setup depth/stencil state
            const VkPipelineDepthStencilStateCreateInfo depth_stencil_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .depthTestEnable = rasterizer.depthStencilState().depthState().enable,
                .depthWriteEnable = rasterizer.depthStencilState().depthState().write,
                .depthCompareOp = conversions::to_compare_operation(rasterizer.depthStencilState().depthState().operation),
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable = rasterizer.depthStencilState().stencilState().enable,
                .front =
                {
                    .failOp = conversions::to_stencil_operation(rasterizer.depthStencilState().stencilState().frontFace.stencilFailOp),
                    .passOp = conversions::to_stencil_operation(rasterizer.depthStencilState().stencilState().frontFace.stencilPassOp),
                    .depthFailOp = conversions::to_stencil_operation(rasterizer.depthStencilState().stencilState().frontFace.depthFailOp),
                    .compareOp = conversions::to_compare_operation(rasterizer.depthStencilState().stencilState().frontFace.operation),
                    .compareMask = rasterizer.depthStencilState().stencilState().readMask,
                    .writeMask = rasterizer.depthStencilState().stencilState().writeMask,
                },
                .back = {
                    .failOp = conversions::to_stencil_operation(rasterizer.depthStencilState().stencilState().backFace.stencilFailOp),
                    .passOp = conversions::to_stencil_operation(rasterizer.depthStencilState().stencilState().backFace.stencilPassOp),
                    .depthFailOp = conversions::to_stencil_operation(rasterizer.depthStencilState().stencilState().backFace.depthFailOp),
                    .compareOp = conversions::to_compare_operation(rasterizer.depthStencilState().stencilState().backFace.operation),
                    .compareMask = rasterizer.depthStencilState().stencilState().readMask,
                    .writeMask = rasterizer.depthStencilState().stencilState().writeMask,
                },
            };

            // Setup shader stages
            auto shader_modules = m_program->shaders();
            log::trace("Using shader program {0} with {1} modules...", reinterpret_cast<const void*>(m_program.get()), shader_modules.size());

            std::vector<VkPipelineShaderStageCreateInfo> shader_stages_info;
            std::ranges::transform(shader_modules,
                                   std::back_inserter(shader_stages_info),
                                   [](const auto& shader_module)
                                   {
                                       const VkPipelineShaderStageCreateInfo shader_info =
                                       {
                                           .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                           .stage = conversions::to_shader_stage(shader_module->stage()),
                                           .module = shader_module->handle(),
                                           .pName = shader_module->entryPoint().c_str(),
                                       };
                                       return shader_info;
                                   });

            // Create pipeline
            const VkGraphicsPipelineCreateInfo pipeline_info = {
                .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .stageCount = static_cast<unsigned>(shader_modules.size()),
                .pStages = shader_stages_info.data(),
                .pVertexInputState = &input_state_info,
                .pInputAssemblyState = &input_assembly_state_info,
                .pViewportState = &viewport_state_info,
                .pRasterizationState = &rasterizer_state_info,
                .pMultisampleState = &multisampling_state_info,
                .pDepthStencilState = &depth_stencil_info,
                .pColorBlendState = &color_blending_info,
                .pDynamicState = &dynamic_state_info,
                .layout = std::as_const(*m_layout).handle(),
                .renderPass = m_renderPass.handle(),
                .subpass = 0,
                .basePipelineHandle = VK_NULL_HANDLE,
            };

            VkPipeline pipeline = VK_NULL_HANDLE;
            if (vkCreateGraphicsPipelines(m_renderPass.device().handle(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create graphics pipeline.");
            return pipeline;
        }

    private:
        const VulkanRenderPass& m_renderPass;

        std::shared_ptr<VulkanPipelineLayout> m_layout;
        std::shared_ptr<VulkanShaderProgram> m_program;
        std::shared_ptr<VulkanInputAssembler> m_inputAssembler;
        std::shared_ptr<VulkanRasterizer> m_rasterizer;
        bool m_alphaToCoverage = false;

        inline static constexpr std::array<VkDynamicState, 5> s_dynamicStates =
        {
            VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT,
            VkDynamicState::VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT,
            VkDynamicState::VK_DYNAMIC_STATE_LINE_WIDTH,
            VkDynamicState::VK_DYNAMIC_STATE_BLEND_CONSTANTS,
            VkDynamicState::VK_DYNAMIC_STATE_STENCIL_REFERENCE
        };
    };

    VulkanRenderPipeline::VulkanRenderPipeline(const VulkanRenderPass& render_pass,
                                               std::shared_ptr<VulkanShaderProgram> shader_program,
                                               std::shared_ptr<VulkanPipelineLayout> layout,
                                               std::shared_ptr<VulkanInputAssembler> input_assembler,
                                               std::shared_ptr<VulkanRasterizer> rasterizer,
                                               bool enable_alpha_to_coverage,
                                               const std::string& name)
        : VulkanPipelineState(VK_NULL_HANDLE),
          m_impl(std::make_unique<Impl>(render_pass, enable_alpha_to_coverage, std::move(layout), std::move(shader_program), std::move(input_assembler), std::move(rasterizer)))
    {
        log::info("Creating render pipeline \"{1}\" for layout {0}...", reinterpret_cast<void*>(layout.get()), name);
        handle() = m_impl->initialize();

        if (!name.empty())
            this->name() = name;
    }

    VulkanRenderPipeline::~VulkanRenderPipeline()
    {
        vkDestroyPipeline(m_impl->m_renderPass.device().handle(), handle(), nullptr);
    }

    std::shared_ptr<const VulkanShaderProgram> VulkanRenderPipeline::program() const noexcept
    {
        return m_impl->m_program;
    }

    std::shared_ptr<VulkanPipelineLayout> VulkanRenderPipeline::layout() const noexcept
    {
        return m_impl->m_layout;
    }

    bool VulkanRenderPipeline::alphaToCoverage() const noexcept
    {
        return m_impl->m_alphaToCoverage;
    }

    std::shared_ptr<VulkanInputAssembler> VulkanRenderPipeline::inputAssembler() const noexcept
    {
        return m_impl->m_inputAssembler;
    }

    std::shared_ptr<VulkanRasterizer> VulkanRenderPipeline::rasterizer() const noexcept
    {
        return m_impl->m_rasterizer;
    }

    void VulkanRenderPipeline::use(const VulkanCommandBuffer& command_buffer) const noexcept
    {
        // Bind the pipeline to the command buffer
        vkCmdBindPipeline(command_buffer.handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, handle());

        // Set the line width (in th case it was changed)
        vkCmdSetLineWidth(command_buffer.handle(), std::as_const(*m_impl->m_rasterizer).lineWidth());
    }

    void VulkanRenderPipeline::bind(const VulkanCommandBuffer& command_buffer, const VulkanDescriptorSet& descriptor_set) const noexcept
    {
        vkCmdBindDescriptorSets(command_buffer.handle(),
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                std::as_const(*m_impl->m_layout).handle(),
                                descriptor_set.layout().space(),
                                1,
                                &descriptor_set.handle(),
                                0,
                                nullptr);
    }
}
