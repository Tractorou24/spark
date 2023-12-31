#pragma once

#include "spark/render/DescriptorSet.h"
#include "spark/render/Resource.h"
#include "spark/render/ShaderStages.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanDescriptorLayout.h"
#include "spark/render/vk/VulkanDescriptorSet.h"

#include <memory>

SPARK_FWD_DECLARE_VK_HANDLE(VkDescriptorSetLayout)

namespace spark::render::vk
{
    class VulkanDevice;
    class VulkanDescriptorLayout;
    class VulkanDescriptorSet;

    /**
     * \brief Vulkan implementation of \ref IDescriptorSetLayout.
     */
    class SPARK_RENDER_VK_EXPORT VulkanDescriptorSetLayout final : public DescriptorSetLayout<VulkanDescriptorLayout, VulkanDescriptorSet>, public Resource<VkDescriptorSetLayout>
    {
    public:
        /**
         * \brief Initializes a \ref VulkanDescriptorSetLayout.
         * \param device The parent \ref VulkanDevice the pipeline layout has been created from.
         * \param descriptor_layouts The \link VulkanDescriptorLayout descriptor layouts \endlink that are part of this descriptor set layout.
         * \param space The space (space id) of the descriptor set layout.
         * \param stages The \ref ShaderStage the descriptor set layout is used in.
         * \param pool_size The size of the descriptor pool.
         * \param max_unbounded_array_size The maximum numbers of descriptors in an unbounded array.
         */
        explicit VulkanDescriptorSetLayout(const VulkanDevice& device,
                                           std::vector<std::unique_ptr<VulkanDescriptorLayout>>&& descriptor_layouts,
                                           unsigned int space,
                                           ShaderStage stages,
                                           unsigned int pool_size = 1024,
                                           unsigned int max_unbounded_array_size = 104857);
        ~VulkanDescriptorSetLayout() noexcept override;

        VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout& other) = delete;
        VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&& other) noexcept = delete;
        VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout& other) = delete;
        VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&& other) noexcept = delete;

        /**
         * \brief Gets the size of each descriptor pool.
         * \return The size of one descriptor pool.
         *
         * Descriptors are allocated from descriptor pools in Vulkan. Each descriptor pool has a number of descriptor sets it can hand out. Before allocating a new descriptor set
         * the layout tries to find an unused descriptor set, that it can hand out. If there are no free descriptor sets, the layout tries to allocate a new one. This is only possible
         * if the descriptor pool is not yet full, in which case a new pool needs to be created. All created pools are cached and destroyed, if the layout itself gets destroyed, 
         * causing all descriptor sets allocated from the layout to be invalidated. 
         * 
         * In general, if the number of required descriptor sets can be pre-calculated, it should be used as a pool size. Otherwise there is a trade-off to be made, based on the 
         * frequency of which new descriptor sets are required. A small pool size is more memory efficient, but can have a significant runtime cost, as long as new allocations happen
         * and no descriptor sets can be reused. A large pool size on the other hand is faster, whilst it may leave a large chunk of descriptor sets unallocated. Keep in mind, that the 
         * layout might not be the only active layout, hence a large portion of descriptor sets might end up not being used.
         */
        [[nodiscard]] unsigned poolSize() const noexcept;

        /**
         * \brief Gets the number of active descriptor pools.
         * \return The number of active descriptor pools.
         */
        [[nodiscard]] std::size_t pools() const noexcept;

        /**
         * \brief Gets the parent \ref VulkanDevice the pipeline layout has been created from.
         * \return The parent \ref VulkanDevice the pipeline layout has been created from.
         */
        [[nodiscard]] const VulkanDevice& device() const noexcept;

        /// \copydoc IDescriptorSetLayout::descriptors()
        [[nodiscard]] std::vector<const VulkanDescriptorLayout*> descriptors() const noexcept override;

        /// \copydoc IDescriptorSetLayout::descriptor()
        [[nodiscard]] const VulkanDescriptorLayout* descriptor(unsigned binding) const override;

        /// \copydoc IDescriptorSetLayout::space()
        [[nodiscard]] unsigned space() const noexcept override;

        /// \copydoc IDescriptorSetLayout::shaderStage()
        [[nodiscard]] ShaderStage shaderStage() const noexcept override;

        /// \copydoc IDescriptorSetLayout::uniforms()
        [[nodiscard]] unsigned uniforms() const noexcept override;

        /// \copydoc IDescriptorSetLayout::storages()
        [[nodiscard]] unsigned storages() const noexcept override;

        /// \copydoc IDescriptorSetLayout::images()
        [[nodiscard]] unsigned images() const noexcept override;

        /// \copydoc IDescriptorSetLayout::samplers()
        [[nodiscard]] unsigned samplers() const noexcept override;

        /// \copydoc IDescriptorSetLayout::buffers()
        [[nodiscard]] unsigned buffers() const noexcept override;

        /// \copydoc IDescriptorSetLayout::staticSamplers()
        [[nodiscard]] unsigned staticSamplers() const noexcept override;

        /// \copydoc IDescriptorSetLayout::inputAttachments()
        [[nodiscard]] unsigned inputAttachments() const noexcept override;

        /// \copydoc DescriptorSetLayout::allocate()
        [[nodiscard]] std::unique_ptr<VulkanDescriptorSet> allocate(const std::vector<DescriptorBinding>& bindings = {}) const override;

        /// \copydoc DescriptorSetLayout::allocate()
        [[nodiscard]] std::unique_ptr<VulkanDescriptorSet> allocate(unsigned descriptors, const std::vector<DescriptorBinding>& bindings = {}) const override;

        /// \copydoc DescriptorSetLayout::allocateMultiple()
        [[nodiscard]] std::vector<std::unique_ptr<VulkanDescriptorSet>> allocateMultiple(unsigned descriptor_sets,
                                                                                         const std::vector<std::vector<DescriptorBinding>>& bindings = {}) const override;

        /// \copydoc DescriptorSetLayout::allocateMultiple()
        [[nodiscard]] std::vector<std::unique_ptr<VulkanDescriptorSet>> allocateMultiple(unsigned descriptor_sets,
                                                                                         const std::function<std::vector<DescriptorBinding>(
                                                                                             unsigned)>& binding_factory) const override;

        /// \copydoc DescriptorSetLayout::allocateMultiple()
        [[nodiscard]] std::vector<std::unique_ptr<VulkanDescriptorSet>> allocateMultiple(unsigned descriptor_sets,
                                                                                         unsigned descriptors,
                                                                                         const std::vector<std::vector<DescriptorBinding>>& bindings = {}) const override;

        /// \copydoc DescriptorSetLayout::allocateMultiple()
        [[nodiscard]] std::vector<std::unique_ptr<VulkanDescriptorSet>> allocateMultiple(unsigned descriptor_sets,
                                                                                         unsigned descriptors,
                                                                                         const std::function<std::vector<DescriptorBinding>(
                                                                                             unsigned)>& binding_factory) const override;

        //// \copydoc IDescriptorSetLayout::free()
        void free(const VulkanDescriptorSet& descriptor_set) const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
