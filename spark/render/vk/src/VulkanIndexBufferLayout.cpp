#include "spark/render/vk/VulkanIndexBufferLayout.h"

namespace spark::render::vk
{
    struct VulkanIndexBufferLayout::Impl
    {
        friend VulkanIndexBufferLayout;

    public:
        explicit Impl(const IndexType type)
            : m_indexType(type) {}

    private:
        BufferType m_type = BufferType::Index;
        unsigned m_binding = 0;
        IndexType m_indexType;
    };

    VulkanIndexBufferLayout::VulkanIndexBufferLayout(IndexType type)
        : m_impl(std::make_unique<Impl>(type)) {}

    VulkanIndexBufferLayout::~VulkanIndexBufferLayout() = default;

    std::size_t VulkanIndexBufferLayout::elementSize() const noexcept
    {
        // UInt16 = 2 bytes, UInt32 = 4 bytes
        // - IndexType::UInt16 = 0x00000010, so 0b00010000 >> 3 = 0b00000010 = 2
        // - IndexType::UInt32 = 0x00000020, so 0b00100000 >> 3 = 0b00000100 = 4
        return static_cast<unsigned>(m_impl->m_indexType) >> 3;
    }

    unsigned VulkanIndexBufferLayout::binding() const noexcept
    {
        return m_impl->m_binding;
    }

    BufferType VulkanIndexBufferLayout::type() const noexcept
    {
        return m_impl->m_type;
    }

    IndexType VulkanIndexBufferLayout::indexType() const noexcept
    {
        return m_impl->m_indexType;
    }
}
