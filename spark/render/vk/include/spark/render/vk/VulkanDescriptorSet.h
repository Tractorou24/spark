#pragma once

#include "spark/render/DescriptorSet.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/VulkanSampler.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkDescriptorSet)

namespace spark::render::vk
{
    class VulkanDescriptorSetLayout;
    class IVulkanBuffer;
    class IVulkanImage;
    class IVulkanSampler;

    /**
     * \brief Vulkan implementation of \ref IDescriptorSet.
     */
    class SPARK_RENDER_VK_EXPORT VulkanDescriptorSet final : public DescriptorSet<IVulkanBuffer, IVulkanImage, IVulkanSampler>, public Resource<VkDescriptorSet>
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanDescriptorSet.
         * \param layout The parent \ref VulkanDescriptorSetLayout.
         * \param descriptor_set The descriptor set handle.
         *
         * \throws base::NullPointerException if \p descriptor_set is `nullptr`.
         */
        explicit VulkanDescriptorSet(const VulkanDescriptorSetLayout& layout, VkDescriptorSet descriptor_set);
        ~VulkanDescriptorSet() override;

        VulkanDescriptorSet(const VulkanDescriptorSet& other) = delete;
        VulkanDescriptorSet(VulkanDescriptorSet&& other) noexcept = delete;
        VulkanDescriptorSet& operator=(const VulkanDescriptorSet& other) = delete;
        VulkanDescriptorSet& operator=(VulkanDescriptorSet&& other) noexcept = delete;

        /**
         * \brief Gets the parent \ref VulkanDescriptorSetLayout.
         * \return The parent \ref VulkanDescriptorSetLayout.
         */
        [[nodiscard]] const VulkanDescriptorSetLayout& layout() const noexcept;

        /// \copydoc DescriptorSet::update()
        void update(unsigned binding, const IVulkanBuffer& buffer, unsigned buffer_element, unsigned elements, unsigned first_descriptor) const override;

        /// \copydoc DescriptorSet::update()
        void update(unsigned binding,
                    const IVulkanImage& texture,
                    unsigned descriptor,
                    unsigned first_level,
                    unsigned levels,
                    unsigned first_layer,
                    unsigned layers) const override;

        /// \copydoc DescriptorSet::update()
        void update(unsigned binding, const IVulkanSampler& sampler, unsigned descriptor) const override;

        /// \copydoc DescriptorSet::attach()
        void attach(unsigned binding, const IVulkanImage& image) const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
