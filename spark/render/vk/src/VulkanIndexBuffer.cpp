#include "spark/render/vk/VulkanIndexBuffer.h"

#include "spark/base/Exception.h"

#include "vk_mem_alloc.h"
#include "vulkan/vulkan.h"

namespace spark::render::vk
{
    struct VulkanIndexBuffer::Impl
    {
        friend class VulkanIndexBuffer;

    public:
        explicit Impl(const VulkanIndexBufferLayout& layout)
            : m_layout(layout) {}

    private:
        const VulkanIndexBufferLayout& m_layout;
    };

    VulkanIndexBuffer::VulkanIndexBuffer(const VkBuffer buffer,
                                         const VulkanIndexBufferLayout& layout,
                                         const unsigned elements,
                                         const VmaAllocator& allocator,
                                         const VmaAllocation& allocation,
                                         const std::string& name)
        : VulkanBuffer(buffer, BufferType::Index, elements, layout.elementSize(), 0, false, allocator, allocation, name), m_impl(std::make_unique<Impl>(layout)) {}

    VulkanIndexBuffer::~VulkanIndexBuffer() = default;

    std::unique_ptr<IVulkanIndexBuffer> VulkanIndexBuffer::Allocate(const VulkanIndexBufferLayout& layout,
                                                                    const unsigned elements,
                                                                    const VmaAllocator& allocator,
                                                                    const VkBufferCreateInfo& create_info,
                                                                    const VmaAllocationCreateInfo& allocation_info,
                                                                    VmaAllocationInfo* allocation_result)
    {
        return Allocate("", layout, elements, allocator, create_info, allocation_info, allocation_result);
    }

    std::unique_ptr<IVulkanIndexBuffer> VulkanIndexBuffer::Allocate(const std::string& name,
                                                                    const VulkanIndexBufferLayout& layout,
                                                                    unsigned elements,
                                                                    const VmaAllocator& allocator,
                                                                    const VkBufferCreateInfo& create_info,
                                                                    const VmaAllocationCreateInfo& allocation_info,
                                                                    VmaAllocationInfo* allocation_result)
    {
        VkBuffer buffer;
        VmaAllocation allocation;

        if (vmaCreateBuffer(allocator, &create_info, &allocation_info, &buffer, &allocation, allocation_result) != VK_SUCCESS)
            throw base::NullPointerException("Failed to allocate index buffer.");
        log::trace("Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3} }}",
                   name.empty() ? std::format("{}", reinterpret_cast<void*>(buffer)) : name,
                   BufferType::Vertex,
                   elements,
                   layout.elementSize(),
                   layout.elementSize() * elements);

        return std::make_unique<VulkanIndexBuffer>(buffer, layout, elements, allocator, allocation, name);
    }

    const VulkanIndexBufferLayout& VulkanIndexBuffer::layout() const noexcept
    {
        return m_impl->m_layout;
    }
}
