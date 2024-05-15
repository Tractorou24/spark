#pragma once

#include "spark/render/DescriptorSet.h"
#include "spark/render/Export.h"
#include "spark/render/PushConstantsLayout.h"
#include "spark/render/StateResource.h"

namespace spark::render
{
    class IShaderProgram;

    /**
     * Defines pipeline stages as points where synchronization may occur.
     */
    enum class PipelineStage
    {
        /// \brief Represents no-blocking behavior.
        /// Translates to `VK_PIPELINE_STAGE_NONE` in Vulkan.
        None = 0x00000000,

        /// \brief Waits for all previous commands to be finished, or blocks all following commands until the barrier is executed.
        /// Translates to `VK_PIPELINE_STAGE_ALL_COMMANDS_BIT` in Vulkan.
        /// This stage flag is special, as it cannot be combined with other stage flags.
        All = 0x00000001,

        /// \brief Waits for previous commands to finish all graphics stages, or blocks following commands until the graphics stages has finished.
        /// Translates to `VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT` in Vulkan.
        /// This stage flag is special, as it cannot be combined with other stage flags.
        Draw = 0x00000002,

        /// \brief Waits for previous commands to finish the input assembly stage, or blocks following commands until the input assembly stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_VERTEX_INPUT_BIT` in Vulkan.
        InputAssembly = 0x00000004,

        /// \brief Waits for previous commands to finish the vertex shader stage, or blocks following commands until the vertex shader stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_VERTEX_SHADER_BIT` in Vulkan.
        Vertex = 0x00000006,

        /// \brief Waits for previous commands to finish the tessellation control/hull shader stage, or blocks following commands until the tessellation control/hull shader stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT` in Vulkan.
        TessellationControl = 0x00000008,

        /// \brief Waits for previous commands to finish the tessellation evaluation/domain shader stage, or blocks following commands until the tessellation evaluation/domain shader stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT` in Vulkan.
        TessellationEvaluation = 0x00000010,

        /// \brief Waits for previous commands to finish the geometry shader stage, or blocks following commands until the geometry shader stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT` in Vulkan.
        Geometry = 0x00000020,

        /// \brief Waits for previous commands to finish the fragment/pixel shader stage, or blocks following commands until the fragment/pixel shader stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT` in Vulkan.
        Fragment = 0x00000040,

        /// \brief Waits for previous commands to finish the depth/stencil stage, or blocks following commands until the depth/stencil stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT` in Vulkan.
        DepthStencil = 0x00000080,

        /// \brief Waits for previous commands to finish the draw indirect stage, or blocks following commands until the draw indirect stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT` in Vulkan.
        Indirect = 0x00000100,

        /// \brief Waits for previous commands to finish the output merger stage, or blocks following commands until the output merger stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT` in Vulkan.
        RenderTarget = 0x00000200,

        /// \brief Waits for previous commands to finish the compute shader stage, or blocks following commands until the compute shader stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT` in Vulkan.
        /// This stage flag is special, as it cannot be combined with other stage flags.
        Compute = 0x00000400,

        /// \brief Waits for previous commands to finish the transfer stage, or blocks following commands until the transfer stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_TRANSFER_BIT` in Vulkan.
        Transfer = 0x00000800,

        /// \brief Waits for previous commands to finish the multi-sampling resolution stage, or blocks following commands until the multi-sampling resolution stage has finished.
        /// Translates to `VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT` in Vulkan.
        Resolve = 0x00001000
    };

    /**
     * \brief Interface for pipeline layouts.
     */
    class SPARK_RENDER_EXPORT IPipelineLayout
    {
    public:
        virtual ~IPipelineLayout() noexcept = default;

        /**
         * \brief Gets the descriptor set layout bound to the given \p space.
         * \param space The space to request the descriptor set layout for.
         * \return A pointer to the \ref IDescriptorSetLayout bound to the given \p space.
         */
        [[nodiscard]] virtual const IDescriptorSetLayout& descriptorSet(unsigned int space) const = 0;

        /**
         * \brief Gets all descriptor set layouts the pipeline has been initialized with.
         * \return A \ref std::vector of \ref IDescriptorSetLayout pointers the pipeline has been initialized with.
         */
        [[nodiscard]] std::vector<const IDescriptorSetLayout*> descriptorSets() const noexcept { return genericDescriptorSets(); }

        /**
         * \brief Gets the push constants layout, or nullptr, if the pipeline does not use any push constants.
         * \return A pointer to the \ref IPushConstantsLayout, or nullptr, if the pipeline does not use any push constants.
         */
        [[nodiscard]] virtual const IPushConstantsLayout* pushConstants() const noexcept = 0;

    private:
        /// \brief Private method used to allow replacement of the generic methods by a custom \ref IDescriptorSetLayout type.
        [[nodiscard]] virtual std::vector<const IDescriptorSetLayout*> genericDescriptorSets() const noexcept = 0;
    };

    /**
     * \brief Represents a the layout of a \ref RenderPipeline.
     * \tparam DescriptorSetLayoutType The type of the descriptor set layout. (inherits from \ref IDescriptorSetLayout)
     * \tparam PushConstantsLayoutType The type of the push constants layout. (inherits from \ref IPushConstantsLayout)
     */
    template <typename DescriptorSetLayoutType, typename PushConstantsLayoutType>
    class PipelineLayout : public IPipelineLayout
    {
    public:
        using descriptor_set_layout_type = DescriptorSetLayoutType;
        using push_constants_layout_type = PushConstantsLayoutType;

    public:
        /// \copydoc IPipelineLayout::descriptorSet()
        [[nodiscard]] const descriptor_set_layout_type& descriptorSet(unsigned int space) const override = 0;

        /// \copydoc IPipelineLayout::descriptorSets()
        [[nodiscard]] virtual std::vector<const descriptor_set_layout_type*> descriptorSets() const noexcept = 0;

        /// \copydoc IPipelineLayout::pushConstants()
        [[nodiscard]] const push_constants_layout_type* pushConstants() const noexcept override = 0;

    private:
        [[nodiscard]] std::vector<const IDescriptorSetLayout*> genericDescriptorSets() const noexcept override
        {
            std::vector<const IDescriptorSetLayout*> descriptor_sets_vector;
            descriptor_sets_vector.reserve(descriptorSets().size());
            std::ranges::transform(descriptorSets(),
                                   std::back_inserter(descriptor_sets_vector),
                                   [](const auto& descriptor_set) { return static_cast<const IDescriptorSetLayout*>(descriptor_set); });
            return descriptor_sets_vector;
        }
    };

    /**
     * \brief Interface representing a pipeline.
     */
    class SPARK_RENDER_EXPORT IPipeline : public virtual IStateResource
    {
    public:
        ~IPipeline() noexcept override = default;

        /**
         * \brief Gets the shader program used by the pipeline.
         * \return A \ref std::shared_ptr pointer to the \ref IShaderProgram used by the pipeline.
         */
        [[nodiscard]] std::shared_ptr<const IShaderProgram> program() const noexcept { return genericProgram(); }

        /**
         * \brief Gets the pipeline layout.
         * \return A \ref std::shared_ptr pointer to the \ref IPipelineLayout.
         */
        [[nodiscard]] std::shared_ptr<IPipelineLayout> layout() const noexcept { return genericLayout(); }

    private:
        /// @{
        /// \brief Private methods used to allow replacement of the generic methods by custom types.
        [[nodiscard]] virtual std::shared_ptr<const IShaderProgram> genericProgram() const noexcept = 0;
        [[nodiscard]] virtual std::shared_ptr<IPipelineLayout> genericLayout() const noexcept = 0;
        /// @}
    };

    /**
     * \brief Represents a pipeline state.
     * \tparam PipelineLayoutType The type of the render pipeline layout. (inherits from \ref IPipelineLayout)
     * \tparam ShaderProgramType The type of the shader program. (inherits from \ref IShaderProgram)
     */
    template <typename PipelineLayoutType, typename ShaderProgramType>
    class Pipeline : virtual public IPipeline
    {
    public:
        using shader_program_type = ShaderProgramType;
        using pipeline_layout_type = PipelineLayoutType;

    public:
        /// \copydoc IPipeline::program()
        [[nodiscard]] virtual std::shared_ptr<const shader_program_type> program() const noexcept = 0;

        /// \copydoc IPipeline::layout()
        [[nodiscard]] virtual std::shared_ptr<pipeline_layout_type> layout() const noexcept = 0;

    private:
        [[nodiscard]] std::shared_ptr<IPipelineLayout> genericLayout() const noexcept final { return std::static_pointer_cast<IPipelineLayout>(layout()); }
        [[nodiscard]] std::shared_ptr<const IShaderProgram> genericProgram() const noexcept final { return std::static_pointer_cast<const IShaderProgram>(program()); }
    };
}
