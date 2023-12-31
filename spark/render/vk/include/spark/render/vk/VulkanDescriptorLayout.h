#pragma once

#include "spark/render/DescriptorSet.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanSampler.h"

#include <memory>

namespace spark::render::vk
{
    class IVulkanSampler;

    /**
     * \brief Implements a Vulkan \ref IDescriptorLayout.
     */
    class SPARK_RENDER_VK_EXPORT VulkanDescriptorLayout final : public IDescriptorLayout
    {
    public:
        /**
         * \brief Initializes a new Vulkan descriptor layout.
         * \param type The type of the descriptor.
         * \param binding The binding point for the descriptor.
         * \param element_size The size of the descriptor.
         * \param descriptors The number of descriptors in the array. If set to `-1`, the descriptor is unbounded.
         */
        explicit VulkanDescriptorLayout(DescriptorType type, unsigned int binding, std::size_t element_size, unsigned int descriptors = 1);

        /**
         * \brief Initializes a new Vulkan descriptor layout for a static sampler.
         * \param static_sampler A \ref std::unique_ptr to a \ref VulkanSampler used to initialize the state with.
         * \param binding The binding point for the descriptor.
         */
        explicit VulkanDescriptorLayout(std::unique_ptr<IVulkanSampler>&& static_sampler, unsigned int binding);
        ~VulkanDescriptorLayout() noexcept override;

        VulkanDescriptorLayout(const VulkanDescriptorLayout& other) = delete;
        VulkanDescriptorLayout(VulkanDescriptorLayout&& other) noexcept = delete;
        VulkanDescriptorLayout& operator=(const VulkanDescriptorLayout& other) = delete;
        VulkanDescriptorLayout& operator=(VulkanDescriptorLayout&& other) noexcept = delete;

        /// \copydoc IDescriptorLayout::elementSize()
        [[nodiscard]] std::size_t elementSize() const noexcept override;

        /// \copydoc IDescriptorLayout::binding()
        [[nodiscard]] unsigned binding() const noexcept override;

        /// \copydoc IDescriptorLayout::type()
        [[nodiscard]] BufferType type() const noexcept override;

        /// \copydoc IDescriptorLayout::descriptorType()
        [[nodiscard]] DescriptorType descriptorType() const noexcept override;

        /// \copydoc IDescriptorLayout::descriptors()
        [[nodiscard]] unsigned descriptors() const noexcept override;

        /// \copydoc IDescriptorLayout::staticSampler()
        [[nodiscard]] const IVulkanSampler* staticSampler() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
