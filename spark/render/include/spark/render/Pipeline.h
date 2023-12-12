#pragma once

#include "spark/render/DescriptorSet.h"
#include "spark/render/Export.h"
#include "spark/render/PushConstantsLayout.h"
#include "spark/render/StateResource.h"

namespace spark::render
{
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
}
