#include "spark/render/vk/VulkanDescriptorSet.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanDescriptorLayout.h"
#include "spark/render/vk/VulkanDescriptorSetLayout.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/VulkanSampler.h"

#include "spark/base/Exception.h"

#include <ranges>

namespace spark::render::vk
{
    struct VulkanDescriptorSet::Impl
    {
        friend VulkanDescriptorSet;

    public:
        explicit Impl(const VulkanDescriptorSetLayout& layout)
            : m_layout(layout) {}

    private:
        const VulkanDescriptorSetLayout& m_layout;
        std::unordered_map<unsigned int, VkBufferView> m_bufferViews;
        std::unordered_map<unsigned int, VkImageView> m_imageViews;
    };

    VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDescriptorSetLayout& layout, const VkDescriptorSet descriptor_set)
        : Resource(descriptor_set), m_impl(std::make_unique<Impl>(layout))
    {
        if (!descriptor_set)
            throw base::NullPointerException("The descriptor set handle must be initialized.");
    }

    VulkanDescriptorSet::~VulkanDescriptorSet()
    {
        // Destroy all buffer views
        for (auto* buffer_view : m_impl->m_bufferViews | std::views::values)
            vkDestroyBufferView(m_impl->m_layout.device().handle(), buffer_view, nullptr);

        // Destroy all image views
        for (auto* image_view : m_impl->m_imageViews | std::views::values)
            vkDestroyImageView(m_impl->m_layout.device().handle(), image_view, nullptr);

        // Free the descriptor set
        m_impl->m_layout.free(*this);
    }

    const VulkanDescriptorSetLayout& VulkanDescriptorSet::layout() const noexcept
    {
        return m_impl->m_layout;
    }

    void VulkanDescriptorSet::update(const unsigned binding,
                                     const IVulkanBuffer& buffer,
                                     const unsigned buffer_element,
                                     const unsigned elements,
                                     const unsigned first_descriptor) const
    {
        VkWriteDescriptorSet write_descriptor_set = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = handle(),
            .dstBinding = binding,
            .dstArrayElement = first_descriptor,
            .descriptorCount = 1,
        };

        std::vector<VkDescriptorBufferInfo> buffer_infos;
        const auto* descriptor_layout = m_impl->m_layout.descriptor(binding);
        const unsigned elements_count = elements > 0 ? elements : buffer.elements() - buffer_element;

        switch (descriptor_layout->descriptorType()) // NOLINT(clang-diagnostic-switch-enum): Other cases handled by the default statement.
        {
        case DescriptorType::ConstantBuffer:
            {
                // Configure the descriptor set
                write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                write_descriptor_set.descriptorCount = elements_count;

                // Create the buffer info
                buffer_infos.resize(elements_count);
                std::ranges::generate(buffer_infos,
                                      [&buffer, buffer_element, i = 0]() mutable
                                      {
                                          return VkDescriptorBufferInfo {
                                              .buffer = buffer.handle(),
                                              .offset = buffer.alignedElementSize() * (buffer_element + i++),
                                              .range = buffer.elementSize()
                                          };
                                      });

                write_descriptor_set.pBufferInfo = buffer_infos.data();
                break;
            }
        case DescriptorType::StructuredBuffer:
        case DescriptorType::RWStructuredBuffer:
        case DescriptorType::ByteAddressBuffer:
        case DescriptorType::RWByteAddressBuffer:
            {
                // Configure the descriptor set
                write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                write_descriptor_set.descriptorCount = elements_count;

                // Create the buffer info
                buffer_infos.resize(elements_count);
                std::ranges::generate(buffer_infos,
                                      [&buffer, buffer_element, i = 0]() mutable
                                      {
                                          return VkDescriptorBufferInfo {
                                              .buffer = buffer.handle(),
                                              .offset = buffer.alignedElementSize() * (buffer_element + i++),
                                              .range = buffer.elementSize()
                                          };
                                      });

                write_descriptor_set.pBufferInfo = buffer_infos.data();
                break;
            }
        case DescriptorType::Buffer:
            {
                // Create a buffer view for the buffer
                const VkBufferViewCreateInfo buffer_view_create_info = {
                    .sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .buffer = buffer.handle(),
                    .format = VK_FORMAT_UNDEFINED,
                    .offset = buffer.alignedElementSize() * buffer_element,
                    .range = buffer.alignedElementSize() * elements_count
                };

                VkBufferView buffer_view = {};
                if (vkCreateBufferView(m_impl->m_layout.device().handle(), &buffer_view_create_info, nullptr, &buffer_view) != VK_SUCCESS)
                    throw base::NullPointerException("Failed to create buffer view.");
                m_impl->m_bufferViews.emplace(binding, buffer_view);

                // Configure the descriptor set
                write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                write_descriptor_set.pTexelBufferView = &buffer_view;
                break;
            }
        case DescriptorType::RWBuffer:
            {
                // Create a buffer view for the buffer
                const VkBufferViewCreateInfo buffer_view_create_info = {
                    .sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .buffer = buffer.handle(),
                    .format = VK_FORMAT_UNDEFINED,
                    .offset = buffer.alignedElementSize() * buffer_element,
                    .range = buffer.alignedElementSize() * elements_count
                };

                VkBufferView buffer_view = {};
                if (vkCreateBufferView(m_impl->m_layout.device().handle(), &buffer_view_create_info, nullptr, &buffer_view) != VK_SUCCESS)
                    throw base::NullPointerException("Failed to create buffer view.");
                m_impl->m_bufferViews.emplace(binding, buffer_view);

                // Configure the descriptor set
                write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
                write_descriptor_set.pTexelBufferView = &buffer_view;
                break;
            }
        default:
            throw base::NotImplementedException(std::format("Invalid descriptor type. The binding {0} does not point to a buffer descriptor.", binding));
        }

        // Remove the buffer view, if there is one bound to the current descriptor.
        if (m_impl->m_bufferViews.contains(binding))
        {
            vkDestroyBufferView(m_impl->m_layout.device().handle(), m_impl->m_bufferViews[binding], nullptr);
            m_impl->m_bufferViews.erase(binding);
        }

        // Update the descriptor set.
        vkUpdateDescriptorSets(m_impl->m_layout.device().handle(), 1, &write_descriptor_set, 0, nullptr);
    }

    void VulkanDescriptorSet::update(unsigned binding,
                                     const IVulkanImage& texture,
                                     const unsigned descriptor,
                                     const unsigned first_level,
                                     const unsigned levels,
                                     const unsigned first_layer,
                                     const unsigned layers) const
    {
        VkDescriptorImageInfo image_info = {};
        VkWriteDescriptorSet write_descriptor_set = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = handle(),
            .dstBinding = binding,
            .dstArrayElement = descriptor,
            .descriptorCount = 1,
            .pImageInfo = &image_info
        };

        // Check if the binding points to a texture descriptor and configure the descriptor set
        const auto* layout = m_impl->m_layout.descriptor(binding);
        switch (layout->descriptorType()) // NOLINT(clang-diagnostic-switch-enum): Other cases handled by the default statement.
        {
        case DescriptorType::Texture:
            {
                image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                break;
            }
        case DescriptorType::RWTexture:
            {
                image_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                break;
            }
        default:
            throw base::NotImplementedException(std::format("Invalid descriptor type. The binding {0} does not point to a texture descriptor.", binding));
        }

        // Remove the image view, if there is one bound to the current descriptor.
        if (m_impl->m_imageViews.contains(binding))
        {
            vkDestroyImageView(m_impl->m_layout.device().handle(), m_impl->m_imageViews[binding], nullptr);
            m_impl->m_imageViews.erase(binding);
        }

        // Create a new image view
        const unsigned levels_numbers = levels == 0 ? texture.levels() - first_level : levels;
        const unsigned layers_numbers = layers == 0 ? texture.layers() - first_layer : layers;

        VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .image = texture.handle(),
            .viewType = conversions::to_image_view(texture.dimensions(), texture.layers()),
            .format = conversions::to_format(texture.format()),
            .components = VkComponentMapping {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = VkImageSubresourceRange {
                .baseMipLevel = first_level,
                .levelCount = levels_numbers,
                .baseArrayLayer = first_layer,
                .layerCount = layers_numbers
            }
        };

        if (!helpers::has_depth(texture.format()) && !helpers::has_stencil(texture.format()))
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        else if (helpers::has_depth(texture.format()))
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        else if (helpers::has_stencil(texture.format()))
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;

        VkImageView image_view = {};
        if (vkCreateImageView(m_impl->m_layout.device().handle(), &image_view_create_info, nullptr, &image_view) != VK_SUCCESS)
            throw base::NullPointerException("Failed to create image view.");

        m_impl->m_imageViews[binding] = image_view;
        image_info.imageView = image_view;

        // Update the descriptor set
        vkUpdateDescriptorSets(m_impl->m_layout.device().handle(), 1, &write_descriptor_set, 0, nullptr);
    }

    void VulkanDescriptorSet::update(unsigned binding, const IVulkanSampler& sampler, const unsigned descriptor) const
    {
        // Check if the binding points to a sampler descriptor
        const auto* layout = m_impl->m_layout.descriptor(binding);
        if (layout->descriptorType() != DescriptorType::Sampler)
            throw base::InvalidDescriptorTypeException(std::format("Invalid descriptor type. The binding {0} does not point to a sampler descriptor.", binding));

        // Configure the descriptor set
        const VkDescriptorImageInfo image_info = {
            .sampler = sampler.handle()
        };

        const VkWriteDescriptorSet write_descriptor_set = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = this->handle(),
            .dstBinding = binding,
            .dstArrayElement = descriptor,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .pImageInfo = &image_info
        };

        // Update the descriptor set
        vkUpdateDescriptorSets(m_impl->m_layout.device().handle(), 1, &write_descriptor_set, 0, nullptr);
    }

    void VulkanDescriptorSet::attach(unsigned binding, const IVulkanImage& image) const
    {
        // Check if the binding points to a input attachment descriptor
        const auto* layout = m_impl->m_layout.descriptor(binding);
        if (layout->descriptorType() != DescriptorType::InputAttachment)
            throw base::InvalidDescriptorTypeException(std::format("Invalid descriptor type. The binding {0} does not point to a input attachment descriptor.", binding));

        // Configure the descriptor set
        const VkDescriptorImageInfo image_info = {
            .imageView = image.imageView(),
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        const VkWriteDescriptorSet write_descriptor_set = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = this->handle(),
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
            .pImageInfo = &image_info
        };

        // Update the descriptor set
        vkUpdateDescriptorSets(m_impl->m_layout.device().handle(), 1, &write_descriptor_set, 0, nullptr);
    }
}
