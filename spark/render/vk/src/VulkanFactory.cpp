#include "spark/render/vk/VulkanFactory.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanDevice.h"

#include "spark/base/Exception.h"

#include "vk_mem_alloc.h"

namespace spark::render::vk
{
    struct VulkanFactory::Impl
    {
        friend class VulkanFactory;

    public:
        explicit Impl(const VulkanDevice& device)
            : m_device(device)
        {
            const VmaAllocatorCreateInfo allocator_info = {
                .physicalDevice = device.graphicsAdapter().handle(),
                .device = device.handle(),
                .instance = device.surface().instance(),
                .vulkanApiVersion = VK_API_VERSION_1_3,
            };

            if (vmaCreateAllocator(&allocator_info, &m_allocator) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create device factory allocator.");
        }

    private:
        const VulkanDevice& m_device;
        VmaAllocator m_allocator = nullptr;
    };

    VulkanFactory::VulkanFactory(const VulkanDevice& device)
        : m_impl(std::make_unique<Impl>(device)) {}

    VulkanFactory::~VulkanFactory()
    {
        if (m_impl->m_allocator)
            vmaDestroyAllocator(m_impl->m_allocator);
    }

    std::unique_ptr<IVulkanBuffer> VulkanFactory::createBuffer(const BufferType type,
                                                               const BufferUsage usage,
                                                               const std::size_t element_size,
                                                               const unsigned elements,
                                                               const bool allow_write) const
    {
        return createBuffer("", type, usage, element_size, elements, allow_write);
    }

    std::unique_ptr<IVulkanBuffer> VulkanFactory::createBuffer(const std::string& name,
                                                               const BufferType type,
                                                               const BufferUsage usage,
                                                               const std::size_t element_size,
                                                               const unsigned elements,
                                                               const bool allow_write) const
    {
        VkBufferCreateInfo buffer_info = {.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        VkBufferUsageFlags usage_flags = {};

        std::size_t aligned_size = element_size;
        std::size_t alignment = 0;
        switch (type)
        {
        case BufferType::Vertex:
            usage_flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        case BufferType::Index:
            usage_flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        case BufferType::Uniform:
            usage_flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            alignment = m_impl->m_device.graphicsAdapter().limits().minUniformBufferOffsetAlignment;
            break;
        case BufferType::Storage:
            usage_flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            alignment = m_impl->m_device.graphicsAdapter().limits().minStorageBufferOffsetAlignment;
            break;
        case BufferType::Texel:
            if (allow_write)
                usage_flags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            else
                usage_flags |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;

            alignment = m_impl->m_device.graphicsAdapter().limits().minTexelBufferOffsetAlignment;
            break;
        default:
            break;
        }

        if (alignment > 0)
            aligned_size = (aligned_size + alignment - 1) & ~(alignment - 1);

        buffer_info.size = aligned_size * elements;

        switch (usage)
        {
        case BufferUsage::Staging:
            usage_flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            break;
        case BufferUsage::Resource:
            usage_flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
        default:
            break;
        }

        buffer_info.usage = usage_flags;

        // Deduct the allocation usage from the buffer usage scenario.
        VmaAllocationCreateInfo alloc_info = {};

        switch (usage)
        {
        case BufferUsage::Staging:
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            break;
        case BufferUsage::Resource:
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
            break;
        case BufferUsage::Dynamic:
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
            break;
        case BufferUsage::Readback:
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
            break;
        }

        std::vector queues = {m_impl->m_device.graphicsQueue().familyId()};

        if (m_impl->m_device.transferQueue().familyId() != m_impl->m_device.graphicsQueue().familyId())
            queues.push_back(m_impl->m_device.transferQueue().familyId());

        // If the buffer is used as a static resource or staging buffer, it needs to be accessible concurrently by the graphics and transfer queues.
        if ((usage != BufferUsage::Staging && usage != BufferUsage::Resource) || queues.size() == 1)
        {
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            buffer_info.queueFamilyIndexCount = 0;

            return VulkanBuffer::Allocate(name, type, elements, element_size, alignment, allow_write, m_impl->m_allocator, buffer_info, alloc_info);
        }

        buffer_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
        buffer_info.queueFamilyIndexCount = static_cast<unsigned>(queues.size());
        buffer_info.pQueueFamilyIndices = queues.data();

        return VulkanBuffer::Allocate(name, type, elements, element_size, alignment, allow_write, m_impl->m_allocator, buffer_info, alloc_info);
    }

    std::unique_ptr<IVulkanVertexBuffer> VulkanFactory::createVertexBuffer(const VulkanVertexBufferLayout& layout, const BufferUsage usage, const unsigned elements) const
    {
        return createVertexBuffer("", layout, usage, elements);
    }

    std::unique_ptr<IVulkanVertexBuffer> VulkanFactory::createVertexBuffer(const std::string& name,
                                                                           const VulkanVertexBufferLayout& layout,
                                                                           const BufferUsage usage,
                                                                           const unsigned elements) const
    {
        VkBufferCreateInfo buffer_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = layout.elementSize() * elements,
        };

        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        switch (usage)
        {
        case BufferUsage::Staging:
            usage_flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            break;
        case BufferUsage::Resource:
            usage_flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
        default:
            break;
        }

        buffer_info.usage = usage_flags;

        // Deduct the allocation usage from the buffer usage scenario.
        VmaAllocationCreateInfo alloc_info = {};

        switch (usage)
        {
        case BufferUsage::Staging:
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            break;
        case BufferUsage::Resource:
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
            break;
        case BufferUsage::Dynamic:
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
            break;
        case BufferUsage::Readback:
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
            break;
        }

        std::vector queues = {m_impl->m_device.graphicsQueue().familyId()};

        if (m_impl->m_device.transferQueue().familyId() != m_impl->m_device.graphicsQueue().familyId())
            queues.push_back(m_impl->m_device.transferQueue().familyId());

        // If the buffer is used as a static resource or staging buffer, it needs to be accessible concurrently by the graphics and transfer queues.
        if ((usage != BufferUsage::Staging && usage != BufferUsage::Resource) || queues.size() == 1)
        {
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            buffer_info.queueFamilyIndexCount = 0;

            return VulkanVertexBuffer::Allocate(name, layout, elements, m_impl->m_allocator, buffer_info, alloc_info);
        }

        buffer_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
        buffer_info.queueFamilyIndexCount = static_cast<unsigned>(queues.size());
        buffer_info.pQueueFamilyIndices = queues.data();

        return VulkanVertexBuffer::Allocate(name, layout, elements, m_impl->m_allocator, buffer_info, alloc_info);
    }

    std::unique_ptr<IVulkanIndexBuffer> VulkanFactory::createIndexBuffer(const VulkanIndexBufferLayout& layout, const BufferUsage usage, const unsigned elements) const
    {
        return createIndexBuffer("", layout, usage, elements);
    }

    std::unique_ptr<IVulkanIndexBuffer> VulkanFactory::createIndexBuffer(const std::string& name,
                                                                         const VulkanIndexBufferLayout& layout,
                                                                         const BufferUsage usage,
                                                                         const unsigned elements) const
    {
        VkBufferCreateInfo buffer_info = {.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        buffer_info.size = layout.elementSize() * elements;

        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        switch (usage)
        {
        case BufferUsage::Staging:
            usage_flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            break;
        case BufferUsage::Resource:
            usage_flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
        default:
            break;
        }

        buffer_info.usage = usage_flags;

        // Deduct the allocation usage from the buffer usage scenario.
        VmaAllocationCreateInfo alloc_info = {};

        switch (usage)
        {
        case BufferUsage::Staging:
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            break;
        case BufferUsage::Resource:
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
            break;
        case BufferUsage::Dynamic:
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
            break;
        case BufferUsage::Readback:
            alloc_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
            break;
        }

        std::vector queues = {m_impl->m_device.graphicsQueue().familyId()};

        if (m_impl->m_device.transferQueue().familyId() != m_impl->m_device.graphicsQueue().familyId())
            queues.push_back(m_impl->m_device.transferQueue().familyId());

        // If the buffer is used as a static resource or staging buffer, it needs to be accessible concurrently by the graphics and transfer queues.
        if ((usage != BufferUsage::Staging && usage != BufferUsage::Resource) || queues.size() == 1)
        {
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            buffer_info.queueFamilyIndexCount = 0;

            return VulkanIndexBuffer::Allocate(name, layout, elements, m_impl->m_allocator, buffer_info, alloc_info);
        }

        buffer_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
        buffer_info.queueFamilyIndexCount = static_cast<unsigned>(queues.size());
        buffer_info.pQueueFamilyIndices = queues.data();

        return VulkanIndexBuffer::Allocate(name, layout, elements, m_impl->m_allocator, buffer_info, alloc_info);
    }

    std::unique_ptr<IVulkanImage> VulkanFactory::createAttachment(const Format format, const math::Vector2<unsigned>& size, const MultiSamplingLevel samples) const
    {
        return createAttachment("", format, size, samples);
    }

    std::unique_ptr<IVulkanImage> VulkanFactory::createAttachment(const std::string& name,
                                                                  const Format format,
                                                                  const math::Vector2<unsigned>& size,
                                                                  const MultiSamplingLevel samples) const
    {
        auto width = std::max(1u, size.x);
        auto height = std::max(1u, size.y);

        unsigned queues[] = {m_impl->m_device.graphicsQueue().familyId()};
        const VkImageUsageFlags usage = (helpers::has_depth(format) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) |
                VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

        const VkImageCreateInfo image_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = conversions::to_format(format),
            .extent {
                .width = width,
                .height = height,
                .depth = 1,
            },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = conversions::to_samples(samples),
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = queues,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        constexpr VmaAllocationCreateInfo alloc_info = {
            .usage = VMA_MEMORY_USAGE_GPU_ONLY,
        };

        if (helpers::has_depth(format))
            return VulkanImage::Allocate(name,
                                         m_impl->m_device,
                                         {width, height, 1},
                                         format,
                                         ImageDimensions::DIM_2,
                                         1,
                                         1,
                                         samples,
                                         false,
                                         ImageLayout::DepthRead,
                                         m_impl->m_allocator,
                                         image_info,
                                         alloc_info);

        return VulkanImage::Allocate(name,
                                     m_impl->m_device,
                                     {width, height, 1},
                                     format,
                                     ImageDimensions::DIM_2,
                                     1,
                                     1,
                                     samples,
                                     false,
                                     ImageLayout::Common,
                                     m_impl->m_allocator,
                                     image_info,
                                     alloc_info);
    }

    std::unique_ptr<IVulkanImage> VulkanFactory::createTexture(const Format format,
                                                               const math::Vector3<unsigned>& size,
                                                               const ImageDimensions dimension,
                                                               const unsigned levels,
                                                               const unsigned layers,
                                                               const MultiSamplingLevel samples,
                                                               const bool allow_write) const
    {
        return createTexture("", format, size, dimension, levels, layers, samples, allow_write);
    }

    std::unique_ptr<IVulkanImage> VulkanFactory::createTexture(const std::string& name,
                                                               Format format,
                                                               const math::Vector3<unsigned>& size,
                                                               ImageDimensions dimension,
                                                               const unsigned levels,
                                                               unsigned layers,
                                                               MultiSamplingLevel samples,
                                                               const bool allow_write) const
    {
        if (dimension == ImageDimensions::CUBE && layers != 6)
            throw base::ArgumentOutOfRangeException(std::format("A cube map must be defined with 6 layers, but only {0} are provided.", layers));

        if (dimension == ImageDimensions::DIM_3 && layers != 1)
            throw base::ArgumentOutOfRangeException(std::format("A 3D texture can only have one layer, but {0} are provided.", layers));

        auto width = std::max(1u, size.x);
        auto height = std::max(1u, size.y);
        auto depth = std::max(1u, size.z);

        VkImageCreateInfo image_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = conversions::to_image_type(dimension),
            .format = conversions::to_format(format),
            .extent = {
                .width = width,
                .height = height,
                .depth = depth,
            },
            .mipLevels = levels,
            .arrayLayers = layers,
            .samples = conversions::to_samples(samples),
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        if (allow_write)
            image_info.usage |= VK_IMAGE_USAGE_STORAGE_BIT;

        if (helpers::has_depth(format) || helpers::has_stencil(format))
            image_info.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        std::vector queues = {m_impl->m_device.graphicsQueue().familyId()};

        if (m_impl->m_device.transferQueue().familyId() != m_impl->m_device.graphicsQueue().familyId())
            queues.push_back(m_impl->m_device.transferQueue().familyId());

        image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_info.queueFamilyIndexCount = static_cast<unsigned>(queues.size());
        image_info.pQueueFamilyIndices = queues.data();

        constexpr VmaAllocationCreateInfo alloc_info = {
            .usage = VMA_MEMORY_USAGE_GPU_ONLY
        };

        return VulkanImage::Allocate(name,
                                     m_impl->m_device,
                                     {width, height, depth},
                                     format,
                                     dimension,
                                     levels,
                                     layers,
                                     samples,
                                     allow_write,
                                     ImageLayout::Undefined,
                                     m_impl->m_allocator,
                                     image_info,
                                     alloc_info);
    }

    std::vector<std::unique_ptr<IVulkanImage>> VulkanFactory::createTextures(const std::size_t /*elements*/,
                                                                             const Format /*format*/,
                                                                             const math::Vector3<unsigned>& /*size*/,
                                                                             const ImageDimensions /*dimension*/,
                                                                             const unsigned /*layers*/,
                                                                             const unsigned /*levels*/,
                                                                             const MultiSamplingLevel /*samples*/,
                                                                             const bool /*allow_write*/) const
    {
        // TODO
        return {};
    }

    std::unique_ptr<IVulkanSampler> VulkanFactory::createSampler(const FilterMode mag_filter,
                                                                 const FilterMode min_filter,
                                                                 const BorderMode border_u,
                                                                 const BorderMode border_v,
                                                                 const BorderMode border_w,
                                                                 const MipMapMode mip_map_mode,
                                                                 const float mip_map_bias,
                                                                 const float max_lod,
                                                                 const float min_lod,
                                                                 const float anisotropy) const
    {
        return createSampler("", mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy);
    }

    std::unique_ptr<IVulkanSampler> VulkanFactory::createSampler(const std::string& name,
                                                                 FilterMode mag_filter,
                                                                 FilterMode min_filter,
                                                                 BorderMode border_u,
                                                                 BorderMode border_v,
                                                                 BorderMode border_w,
                                                                 MipMapMode mip_map_mode,
                                                                 float mip_map_bias,
                                                                 float max_lod,
                                                                 float min_lod,
                                                                 float anisotropy) const
    {
        return std::make_unique<VulkanSampler>(m_impl->m_device,
                                               mag_filter,
                                               min_filter,
                                               border_u,
                                               border_v,
                                               border_w,
                                               mip_map_mode,
                                               mip_map_bias,
                                               min_lod,
                                               max_lod,
                                               anisotropy,
                                               name);
    }

    std::vector<std::unique_ptr<IVulkanSampler>> VulkanFactory::createSamplers(const std::size_t /*elements*/,
                                                                               const FilterMode /*mag_filter*/,
                                                                               const FilterMode /*min_filter*/,
                                                                               const BorderMode /*border_u*/,
                                                                               const BorderMode /*border_v*/,
                                                                               const BorderMode /*border_w*/,
                                                                               const MipMapMode /*mip_map_mode*/,
                                                                               const float /*mip_map_bias*/,
                                                                               const float /*max_lod*/,
                                                                               const float /*min_lod*/,
                                                                               const float /*anisotropy*/) const
    {
        // TODO
        return {};
    }
}
