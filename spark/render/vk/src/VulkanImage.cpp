#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanDevice.h"

#include "spark/base/Exception.h"
#include "spark/log/Logger.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include <vector>

namespace spark::render::vk
{
    struct VulkanImage::Impl
    {
        friend class VulkanImage;

    public:
        explicit Impl(VulkanImage* parent,
                      const VulkanDevice& device,
                      const math::Vector3<unsigned>& extent,
                      const Format format,
                      const ImageDimensions dimensions,
                      const unsigned levels,
                      const unsigned layers,
                      const MultiSamplingLevel samples,
                      const bool writable,
                      const ImageLayout initial_layout,
                      const VmaAllocator allocator,
                      const VmaAllocation allocation)
            : m_device(device), m_extent(extent), m_dimensions(dimensions), m_samples(samples), m_format(format), m_layers(layers), m_levels(levels),
              m_writable(writable), m_allocator(allocator), m_allocationInfo(allocation)
        {
            VkImageViewCreateInfo image_view_info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .image = parent->handle(),
                .viewType = conversions::to_image_view_type(dimensions),
                .format = conversions::to_format(m_format),
                .components = VkComponentMapping {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY
                },
                .subresourceRange = VkImageSubresourceRange {
                    .baseMipLevel = 0,
                    .levelCount = m_levels,
                    .baseArrayLayer = 0,
                    .layerCount = m_layers
                }
            };

            if (!helpers::has_depth(m_format) && !helpers::has_stencil(m_format))
                image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            else if (helpers::has_depth(format))
                image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            else if (helpers::has_stencil(format))
                image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;

            VkImageView view = VK_NULL_HANDLE;
            if (vkCreateImageView(m_device.handle(), &image_view_info, nullptr, &view) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create image view.");
            m_views.push_back(view);

            // TODO: Support multi-planar images
            m_planes = static_cast<unsigned>(m_views.size());
            m_elements = m_layers * m_levels * m_planes;
            m_layouts.resize(m_elements, initial_layout);
        }

    private:
        const VulkanDevice& m_device;
        std::vector<VkImageView> m_views;
        std::vector<ImageLayout> m_layouts;
        math::Vector3<unsigned> m_extent;
        ImageDimensions m_dimensions;
        MultiSamplingLevel m_samples;
        Format m_format;
        unsigned m_elements, m_layers, m_levels, m_planes;
        bool m_writable;

        VmaAllocator m_allocator;
        VmaAllocation m_allocationInfo;
    };

    VulkanImage::VulkanImage(const VulkanDevice& device,
                             VkImage image,
                             const math::Vector3<unsigned>& extent,
                             Format format,
                             ImageDimensions dimensions,
                             unsigned levels,
                             unsigned layers,
                             MultiSamplingLevel samples,
                             bool writable,
                             ImageLayout initial_layout,
                             VmaAllocator allocator,
                             VmaAllocation allocation,
                             const std::string& name)
        : Resource(image),
          m_impl(std::make_unique<Impl>(this, device, extent, format, dimensions, levels, layers, samples, writable, initial_layout, allocator, allocation))
    {
        if (!name.empty())
            this->name() = name;
    }

    VulkanImage::~VulkanImage()
    {
        for (const auto& view : m_impl->m_views)
            vkDestroyImageView(m_impl->m_device.handle(), view, nullptr);

        if (m_impl->m_allocator != nullptr && m_impl->m_allocationInfo != nullptr)
        {
            vmaDestroyImage(m_impl->m_allocator, this->handle(), m_impl->m_allocationInfo);
            log::trace("Destroyed image {0}", reinterpret_cast<void*>(this->handle()));
        }
    }

    std::unique_ptr<VulkanImage> VulkanImage::Allocate(const VulkanDevice& device,
                                                       const math::Vector3<unsigned>& extent,
                                                       Format format,
                                                       ImageDimensions dimensions,
                                                       unsigned levels,
                                                       unsigned layers,
                                                       MultiSamplingLevel samples,
                                                       bool writable,
                                                       ImageLayout initial_layout,
                                                       VmaAllocator& allocator,
                                                       const VkImageCreateInfo& create_info,
                                                       const VmaAllocationCreateInfo& allocation_info,
                                                       VmaAllocationInfo* allocation_result)
    {
        return Allocate("", device, extent, format, dimensions, levels, layers, samples, writable, initial_layout, allocator, create_info, allocation_info, allocation_result);
    }

    std::unique_ptr<VulkanImage> VulkanImage::Allocate(const std::string& name,
                                                       const VulkanDevice& device,
                                                       const math::Vector3<unsigned>& extent,
                                                       Format format,
                                                       ImageDimensions dimensions,
                                                       unsigned levels,
                                                       unsigned layers,
                                                       MultiSamplingLevel samples,
                                                       bool writable,
                                                       ImageLayout initial_layout,
                                                       VmaAllocator& allocator,
                                                       const VkImageCreateInfo& create_info,
                                                       const VmaAllocationCreateInfo& allocation_info,
                                                       VmaAllocationInfo* allocation_result)
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = nullptr;
        if (vmaCreateImage(allocator, &create_info, &allocation_info, &image, &allocation, allocation_result) != VK_SUCCESS)
            throw base::NullPointerException("Failed to allocate image.");

        log::trace("Allocated image {0} with {1} bytes {{ Extent: {2}x{3} Px, Format: {4}, Levels: {5}, Layers: {6}, Samples: {8}, Writable: {7} }}",
                   name.empty() ? std::format("{}", reinterpret_cast<void*>(image)) : name,
                   helpers::format_size(format) * extent.x * extent.y,
                   extent.x,
                   extent.y,
                   format,
                   levels,
                   layers,
                   writable,
                   samples);
        return std::make_unique<VulkanImage>(device, image, extent, format, dimensions, levels, layers, samples, writable, initial_layout, allocator, allocation, name);
    }

    VkImageAspectFlags VulkanImage::aspectMask() const noexcept
    {
        // Get the aspect mask for all sub-resources
        if (helpers::has_depth(m_impl->m_format) && helpers::has_stencil(m_impl->m_format))
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

        if (helpers::has_depth(m_impl->m_format))
            return VK_IMAGE_ASPECT_DEPTH_BIT;

        if (helpers::has_stencil(m_impl->m_format))
            return VK_IMAGE_ASPECT_STENCIL_BIT;

        if (m_impl->m_planes > 1)
        {
            VkImageAspectFlags aspect_mask = VK_IMAGE_ASPECT_PLANE_0_BIT;

            if (m_impl->m_planes > 1)
                aspect_mask |= VK_IMAGE_ASPECT_PLANE_1_BIT;
            if (m_impl->m_planes > 2)
                aspect_mask |= VK_IMAGE_ASPECT_PLANE_2_BIT;
            if (m_impl->m_planes > 3)
                log::error("An image resource with a multi-planar format has {0} planes, which is not supported (maximum is {1}).", m_impl->m_planes, 3);
            return aspect_mask;
        }

        return VK_IMAGE_ASPECT_COLOR_BIT;
    }

    VkImageAspectFlags VulkanImage::aspectMask(const unsigned plane) const
    {
        if (helpers::has_depth(m_impl->m_format) && helpers::has_stencil(m_impl->m_format))
        {
            if (plane > 2)
                throw base::BadArgumentException("An image resource with a depth/stencil format has more than two planes, which is not supported.");
            return plane == 1 ? VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if (helpers::has_depth(m_impl->m_format))
        {
            if (plane > 1)
                throw base::BadArgumentException("An image resource with a depth-only format has more than one planes, which is not supported.");
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if (helpers::has_stencil(m_impl->m_format))
        {
            if (plane > 1)
                throw base::BadArgumentException("An image resource with a stencil-only format has more than one planes, which is not supported.");
            return VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        if (m_impl->m_planes > 1)
        {
            if (plane == 0)
                return VK_IMAGE_ASPECT_PLANE_0_BIT;
            if (plane == 1)
                return VK_IMAGE_ASPECT_PLANE_1_BIT;
            if (plane == 2)
                return VK_IMAGE_ASPECT_PLANE_2_BIT;
            throw base::BadArgumentException("An image resource with a multi-planar format has more than three planes, which is not supported.");
        }

        return VK_IMAGE_ASPECT_COLOR_BIT;
    }

    const VkImageView& VulkanImage::imageView(unsigned plane) const
    {
        if (plane >= m_impl->m_views.size())
            throw base::ArgumentOutOfRangeException(std::format("The image does not have a plane {0}.", plane));
        return m_impl->m_views[plane];
    }

    unsigned VulkanImage::elements() const noexcept
    {
        return m_impl->m_elements;
    }

    std::size_t VulkanImage::size() const noexcept
    {
        if (m_impl->m_allocationInfo)
            return m_impl->m_allocationInfo->GetSize();

        auto element_size = helpers::format_size(m_impl->m_format) * m_impl->m_extent.x * m_impl->m_extent.y * m_impl->m_extent.z * m_impl->m_layers;
        auto total_size = element_size;

        for (unsigned l = 1; l < m_impl->m_levels; ++l)
        {
            element_size /= 2;
            total_size += element_size;
        }

        return total_size * m_impl->m_planes;
    }

    std::size_t VulkanImage::elementSize() const noexcept
    {
        return size();
    }

    std::size_t VulkanImage::elementAlignment() const
    {
        if (m_impl->m_allocationInfo)
            return m_impl->m_allocationInfo->GetAlignment();
        throw base::NotImplementedException("Getting element alignment on a non vma allocated image is not implemented.");
    }

    std::size_t VulkanImage::alignedElementSize() const noexcept
    {
        const std::size_t alignment = elementAlignment();
        const std::size_t size = elementSize();
        return alignment == 0 ? size : (size + alignment - 1) & ~(alignment - 1);
    }

    bool VulkanImage::writable() const noexcept
    {
        return m_impl->m_writable;
    }

    std::size_t VulkanImage::size(const unsigned level) const
    {
        if (level >= m_impl->m_levels)
            return 0;

        const auto size = extent(level);
        switch (this->dimensions())
        {
        case ImageDimensions::DIM_1:
            return helpers::format_size(this->format()) * size.x;
        case ImageDimensions::CUBE:
        case ImageDimensions::DIM_2:
            return helpers::format_size(this->format()) * size.x * size.y;
        case ImageDimensions::DIM_3:
            return helpers::format_size(this->format()) * size.x * size.y * size.z;
        }
        throw base::UnknownException("Unknown image dimension.");
    }

    math::Vector3<unsigned> VulkanImage::extent(const unsigned level) const noexcept
    {
        if (level >= m_impl->m_levels)
            return {0, 0, 0};

        auto size = m_impl->m_extent;
        for (std::size_t l = 0; l < level; ++l)
            size /= 2;

        size.x = std::max<unsigned>(size.x, 1);
        size.y = std::max<unsigned>(size.y, 1);
        size.z = std::max<unsigned>(size.z, 1);

        return size;
    }

    Format VulkanImage::format() const noexcept
    {
        return m_impl->m_format;
    }

    ImageDimensions VulkanImage::dimensions() const noexcept
    {
        return m_impl->m_dimensions;
    }

    unsigned VulkanImage::levels() const noexcept
    {
        return m_impl->m_levels;
    }

    unsigned VulkanImage::layers() const noexcept
    {
        return m_impl->m_layers;
    }

    unsigned VulkanImage::planes() const noexcept
    {
        return m_impl->m_planes;
    }

    MultiSamplingLevel VulkanImage::samples() const noexcept
    {
        return m_impl->m_samples;
    }

    ImageLayout VulkanImage::layout(const unsigned sub_resource) const
    {
        if (sub_resource >= m_impl->m_layouts.size())
            throw base::ArgumentOutOfRangeException(std::format("The image does not have a subresource {0}.", sub_resource));
        return m_impl->m_layouts[sub_resource];
    }
}
