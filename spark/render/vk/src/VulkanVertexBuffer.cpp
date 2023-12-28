#include "spark/render/vk/VulkanVertexBuffer.h"

#include "spark/base/Exception.h"

#include "vk_mem_alloc.h"
#include "vulkan/vulkan.h"

namespace spark::render::vk
{
    struct VulkanVertexBuffer::Impl
    {
        friend class VulkanVertexBuffer;

    public:
        explicit Impl(const VulkanVertexBufferLayout& layout)
            : m_layout(layout) {}

    private:
        const VulkanVertexBufferLayout& m_layout;
    };

    VulkanVertexBuffer::VulkanVertexBuffer(const VkBuffer buffer,
                                           const VulkanVertexBufferLayout& layout,
                                           const unsigned elements,
                                           const VmaAllocator& allocator,
                                           const VmaAllocation& allocation,
                                           const std::string& name)
        : VulkanBuffer(buffer, BufferType::Vertex, elements, layout.elementSize(), 0, false, allocator, allocation, name), m_impl(std::make_unique<Impl>(layout)) {}

    VulkanVertexBuffer::~VulkanVertexBuffer() = default;

    std::unique_ptr<IVulkanVertexBuffer> VulkanVertexBuffer::Allocate(const VulkanVertexBufferLayout& layout,
                                                                      const unsigned elements,
                                                                      const VmaAllocator& allocator,
                                                                      const VkBufferCreateInfo& create_info,
                                                                      const VmaAllocationCreateInfo& allocation_info,
                                                                      VmaAllocationInfo* allocation_result)
    {
        return Allocate("", layout, elements, allocator, create_info, allocation_info, allocation_result);
    }

    std::unique_ptr<IVulkanVertexBuffer> VulkanVertexBuffer::Allocate(const std::string& name,
                                                                      const VulkanVertexBufferLayout& layout,
                                                                      unsigned elements,
                                                                      const VmaAllocator& allocator,
                                                                      const VkBufferCreateInfo& create_info,
                                                                      const VmaAllocationCreateInfo& allocation_info,
                                                                      VmaAllocationInfo* allocation_result)
    {
        VkBuffer buffer;
        VmaAllocation allocation;

        if (vmaCreateBuffer(allocator, &create_info, &allocation_info, &buffer, &allocation, allocation_result) != VK_SUCCESS)
            throw base::NullPointerException("Failed to allocate vertex buffer");
        log::trace("Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3} }}",
                   name.empty() ? std::format("{}", reinterpret_cast<void*>(buffer)) : name,
                   BufferType::Vertex,
                   elements,
                   layout.elementSize(),
                   layout.elementSize() * elements);

        return std::make_unique<VulkanVertexBuffer>(buffer, layout, elements, allocator, allocation, name);
    }

    const VulkanVertexBufferLayout& VulkanVertexBuffer::layout() const noexcept
    {
        return m_impl->m_layout;
    }
}
