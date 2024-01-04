#pragma once

#include "spark/render/Pipeline.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanDescriptorSetLayout.h"
#include "spark/render/vk/VulkanPushConstantsLayout.h"

#include <vector>

SPARK_FWD_DECLARE_VK_HANDLE(VkPipelineLayout)

namespace spark::render::vk
{
    class VulkanDevice;

    /**
     * \brief Vulkan implementation of \ref IPipelineLayout.
     */
    class SPARK_RENDER_VK_EXPORT VulkanPipelineLayout final : public PipelineLayout<VulkanDescriptorSetLayout, VulkanPushConstantsLayout>, public Resource<VkPipelineLayout>
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanPipelineLayout.
         * \param device The parent \ref VulkanDevice, the pipeline layout is created from.
         * \param descriptor_set_layouts The \link VulkanDescriptorSetLayout descriptor set layouts \endlink used by the pipeline.
         * \param push_constants_layout The \link VulkanPushConstantsLayout push constants layout \endlink used by the pipeline.
         */
        explicit VulkanPipelineLayout(const VulkanDevice& device,
                                      std::vector<std::unique_ptr<VulkanDescriptorSetLayout>>&& descriptor_set_layouts,
                                      std::unique_ptr<VulkanPushConstantsLayout>&& push_constants_layout);
        ~VulkanPipelineLayout() override;

        VulkanPipelineLayout(const VulkanPipelineLayout& other) = delete;
        VulkanPipelineLayout(VulkanPipelineLayout&& other) noexcept = delete;
        VulkanPipelineLayout& operator=(const VulkanPipelineLayout& other) = delete;
        VulkanPipelineLayout& operator=(VulkanPipelineLayout&& other) noexcept = delete;

        /**
         * \brief Gets the parent \ref VulkanDevice the pipeline layout layout has been created from.
         * \return The parent \ref VulkanDevice the pipeline layout layout has been created from.
         */
        [[nodiscard]] const VulkanDevice& device() const noexcept;

        /// \copydoc PipelineLayout::descriptorSet()
        [[nodiscard]] const VulkanDescriptorSetLayout& descriptorSet(unsigned space) const override;

        /// \copydoc PipelineLayout::descriptorSets()
        [[nodiscard]] std::vector<const VulkanDescriptorSetLayout*> descriptorSets() const noexcept override;

        /// \copydoc PipelineLayout::pushConstants()
        [[nodiscard]] const VulkanPushConstantsLayout* pushConstants() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
