#include "spark/render/vk/VulkanBuffer.h"

#include "spark/base/Exception.h"
#include "spark/log/Logger.h"

#include "vk_mem_alloc.h"
#include "vulkan/vulkan.h"

#include <algorithm>

#include <cstring>

namespace spark::render::vk
{
    struct VulkanBuffer::Impl
    {
        friend class VulkanBuffer;

    public:
        explicit Impl(const BufferType type,
                      const unsigned elements,
                      const std::size_t element_size,
                      const std::size_t alignment,
                      const bool writable,
                      const VmaAllocator& allocator,
                      const VmaAllocation& allocation)
            : m_type(type), m_elements(elements), m_writable(writable), m_elementSize(element_size), m_alignment(alignment), m_allocator(allocator), m_allocation(allocation) {}

    private:
        BufferType m_type;
        unsigned m_elements;
        bool m_writable;
        std::size_t m_elementSize, m_alignment;
        VmaAllocator m_allocator;
        VmaAllocation m_allocation;
    };

    VulkanBuffer::VulkanBuffer(const VkBuffer buffer,
                               BufferType type,
                               unsigned elements,
                               std::size_t element_size,
                               std::size_t alignment,
                               bool writable,
                               const VmaAllocator& allocator,
                               const VmaAllocation& allocation,
                               const std::string& name)
        : Resource(buffer), m_impl(std::make_unique<Impl>(type, elements, element_size, alignment, writable, allocator, allocation))
    {
        if (!name.empty())
            this->name() = name;
    }

    VulkanBuffer::~VulkanBuffer()
    {
        vmaDestroyBuffer(m_impl->m_allocator, handle(), m_impl->m_allocation);
        log::trace("Destroyed buffer {0}", reinterpret_cast<void*>(handle()));
    }

    std::unique_ptr<IVulkanBuffer> VulkanBuffer::Allocate(const BufferType type,
                                                          const unsigned elements,
                                                          const std::size_t element_size,
                                                          const std::size_t alignment,
                                                          const bool writable,
                                                          const VmaAllocator& allocator,
                                                          const VkBufferCreateInfo& create_info,
                                                          const VmaAllocationCreateInfo& allocation_info,
                                                          VmaAllocationInfo* allocation_result)
    {
        return Allocate("", type, elements, element_size, alignment, writable, allocator, create_info, allocation_info, allocation_result);
    }

    std::unique_ptr<IVulkanBuffer> VulkanBuffer::Allocate(const std::string& name,
                                                          BufferType type,
                                                          unsigned elements,
                                                          std::size_t element_size,
                                                          std::size_t alignment,
                                                          bool writable,
                                                          const VmaAllocator& allocator,
                                                          const VkBufferCreateInfo& create_info,
                                                          const VmaAllocationCreateInfo& allocation_info,
                                                          VmaAllocationInfo* allocation_result)
    {
        VkBuffer buffer;
        VmaAllocation allocation;

        if (vmaCreateBuffer(allocator, &create_info, &allocation_info, &buffer, &allocation, allocation_result) != VK_SUCCESS)
            throw base::NullPointerException("Failed to create buffer.");

        log::trace("Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3}, Writable: {5} }}",
                   (name.empty() ? std::format("{}", reinterpret_cast<void*>(buffer)) : name),
                   type,
                   elements,
                   element_size,
                   elements * element_size,
                   writable);
        return std::make_unique<VulkanBuffer>(buffer, type, elements, element_size, alignment, writable, allocator, allocation, name);
    }

    BufferType VulkanBuffer::type() const
    {
        return m_impl->m_type;
    }

    unsigned VulkanBuffer::elements() const noexcept
    {
        return m_impl->m_elements;
    }

    std::size_t VulkanBuffer::size() const noexcept
    {
        return m_impl->m_elements * m_impl->m_alignment;
    }

    std::size_t VulkanBuffer::elementSize() const noexcept
    {
        return m_impl->m_elementSize;
    }

    std::size_t VulkanBuffer::elementAlignment() const
    {
        return m_impl->m_alignment;
    }

    std::size_t VulkanBuffer::alignedElementSize() const noexcept
    {
        return m_impl->m_alignment == 0 ? m_impl->m_elementSize : (m_impl->m_elementSize + m_impl->m_alignment - 1) & ~(m_impl->m_alignment - 1);
    }

    bool VulkanBuffer::writable() const noexcept
    {
        return m_impl->m_writable;
    }

    void VulkanBuffer::map(const void* data, const std::size_t size, unsigned element)
    {
        if (element >= m_impl->m_elements)
            throw base::ArgumentOutOfRangeException(std::format("The element {0} is out of range. The buffer only contains {1} elements.", element, m_impl->m_elements));

        // Calculate the aligned size of the buffer
        std::size_t aligned_size = size;
        const std::size_t alignment = this->elementAlignment();

        if (alignment > 0)
            aligned_size = (size + alignment - 1) & ~(alignment - 1);

        // Map memory to pointer `buffer`
        char* buffer = nullptr;
        if (vmaMapMemory(m_impl->m_allocator, m_impl->m_allocation, reinterpret_cast<void**>(&buffer)) != VK_SUCCESS)
            throw base::NullPointerException("Failed to map buffer memory.");

        std::memcpy(buffer + element * aligned_size, data, size);

        vmaUnmapMemory(m_impl->m_allocator, m_impl->m_allocation);
    }

    void VulkanBuffer::map(const std::span<const void*> data, const std::size_t element_size, const unsigned first_element)
    {
        std::ranges::for_each(data,
                              [this, &element_size, i = first_element](const void* const mem) mutable
                              {
                                  this->map(mem, element_size, i++);
                              });
    }

    void VulkanBuffer::map(void* data, const size_t size, unsigned element, const bool write)
    {
        if (element >= m_impl->m_elements)
            throw base::ArgumentOutOfRangeException(std::format("The element {0} is out of range. The buffer only contains {1} elements.", element, m_impl->m_elements));

        // Calculate the aligned size of the buffer
        std::size_t aligned_size = size;
        const std::size_t alignment = this->elementAlignment();

        if (alignment > 0)
            aligned_size = (size + alignment - 1) & ~(alignment - 1);

        // Map memory to pointer `buffer`
        char* buffer = nullptr;
        if (vmaMapMemory(m_impl->m_allocator, m_impl->m_allocation, reinterpret_cast<void**>(&buffer)) != VK_SUCCESS)
            throw base::NullPointerException("Failed to map buffer memory.");

        if (write)
            std::memcpy(buffer + element * aligned_size, data, size);
        else
            std::memcpy(data, buffer + element * aligned_size, aligned_size);

        vmaUnmapMemory(m_impl->m_allocator, m_impl->m_allocation);
    }

    void VulkanBuffer::map(std::span<void*> data, size_t element_size, unsigned first_element, bool write)
    {
        std::ranges::for_each(data,
                              [this, &element_size, &write, i = first_element](void* mem) mutable
                              {
                                  this->map(mem, element_size, i++, write);
                              });
    }
}
