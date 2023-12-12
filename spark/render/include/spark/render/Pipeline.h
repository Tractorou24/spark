#pragma once

#include "spark/render/DescriptorSet.h"
#include "spark/render/Export.h"
#include "spark/render/PushConstantsLayout.h"
#include "spark/render/StateResource.h"

namespace spark::render
{
    class IShaderProgram;

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
}
