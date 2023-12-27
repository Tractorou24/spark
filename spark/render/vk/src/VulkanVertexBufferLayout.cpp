#include "spark/render/vk/VulkanVertexBufferLayout.h"

#include <algorithm>
#include <ranges>
#include <vector>

namespace spark::render::vk
{
    struct VulkanVertexBufferLayout::Impl
    {
        friend class VulkanVertexBufferLayout;

    public:
        explicit Impl(const std::size_t vertex_size, const unsigned int binding)
            : m_vertexSize(vertex_size), m_binding(binding) {}

    private:
        std::vector<std::unique_ptr<BufferAttribute>> m_attributes = {};
        BufferType m_bufferType = BufferType::Vertex;
        std::size_t m_vertexSize;
        unsigned int m_binding;
    };

    VulkanVertexBufferLayout::VulkanVertexBufferLayout(std::size_t vertex_size, unsigned binding)
        : m_impl(std::make_unique<Impl>(vertex_size, binding)) {}

    VulkanVertexBufferLayout::~VulkanVertexBufferLayout() = default;

    std::vector<const BufferAttribute*> VulkanVertexBufferLayout::attributes() const noexcept
    {
        std::vector<const BufferAttribute*> attributes;
        attributes.reserve(m_impl->m_attributes.size());
        std::ranges::transform(m_impl->m_attributes, std::back_inserter(attributes), [](const auto& attribute) { return attribute.get(); });
        return attributes;
    }

    void VulkanVertexBufferLayout::addAttribute(BufferAttribute&& attribute) noexcept
    {
        m_impl->m_attributes.push_back(std::make_unique<BufferAttribute>(std::move(attribute)));
    }

    std::size_t VulkanVertexBufferLayout::elementSize() const noexcept
    {
        return m_impl->m_vertexSize;
    }

    unsigned VulkanVertexBufferLayout::binding() const noexcept
    {
        return m_impl->m_binding;
    }

    BufferType VulkanVertexBufferLayout::type() const noexcept
    {
        return m_impl->m_bufferType;
    }
}
