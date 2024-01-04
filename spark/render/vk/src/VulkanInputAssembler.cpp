#include "spark/render/vk/VulkanInputAssembler.h"
#include "spark/render/vk/VulkanVertexBufferLayout.h"

#include "spark/base/Exception.h"

#include <algorithm>
#include <unordered_map>

namespace spark::render::vk
{
    struct VulkanInputAssembler::Impl
    {
        friend class VulkanInputAssembler;

    public:
        explicit Impl(std::vector<std::unique_ptr<VulkanVertexBufferLayout>>&& vertex_buffer_layouts,
                      std::unique_ptr<VulkanIndexBufferLayout>&& index_buffer_layout,
                      const PrimitiveTopology primitive_topology)
            : m_primitiveTopology(primitive_topology)
        {
            if (!index_buffer_layout)
                throw base::NullPointerException("The index buffer layout must be initialized.");
            m_indexBufferLayout = std::move(index_buffer_layout);

            for (auto& vertex_buffer_layout : vertex_buffer_layouts)
            {
                if (!vertex_buffer_layout)
                    throw base::NullPointerException("One of the provided vertex buffer layouts is not initialized.");

                if (m_vertexBufferLayouts.contains(vertex_buffer_layout->binding()))
                    throw base::BadArgumentException(std::format("Multiple vertex buffer layouts use the binding point {0}, but only one layout per binding point is allowed.",
                                                                 vertex_buffer_layout->binding()));

                m_vertexBufferLayouts.emplace(vertex_buffer_layout->binding(), std::move(vertex_buffer_layout));
            }
        }

    private:
        std::unordered_map<unsigned, std::unique_ptr<VulkanVertexBufferLayout>> m_vertexBufferLayouts;
        std::unique_ptr<VulkanIndexBufferLayout> m_indexBufferLayout;
        PrimitiveTopology m_primitiveTopology;
    };

    VulkanInputAssembler::VulkanInputAssembler(std::vector<std::unique_ptr<VulkanVertexBufferLayout>>&& vertex_buffer_layouts,
                                               std::unique_ptr<VulkanIndexBufferLayout>&& index_buffer_layout,
                                               PrimitiveTopology primitive_topology)
        : m_impl(std::make_unique<Impl>(std::move(vertex_buffer_layouts), std::move(index_buffer_layout), primitive_topology)) {}

    VulkanInputAssembler::~VulkanInputAssembler() = default;

    PrimitiveTopology VulkanInputAssembler::topology() const noexcept
    {
        return m_impl->m_primitiveTopology;
    }

    std::vector<const VulkanVertexBufferLayout*> VulkanInputAssembler::vertexBufferLayouts() const noexcept
    {
        std::vector<const VulkanVertexBufferLayout*> layouts;
        layouts.reserve(m_impl->m_vertexBufferLayouts.size());
        std::ranges::transform(m_impl->m_vertexBufferLayouts, std::back_inserter(layouts), [](const auto& pair) { return pair.second.get(); });
        return layouts;
    }

    const VulkanVertexBufferLayout& VulkanInputAssembler::vertexBufferLayout(unsigned binding) const
    {
        if (!m_impl->m_vertexBufferLayouts.contains(binding))
            throw base::BadArgumentException(std::format("No vertex buffer layout is bound to binding point {0}.", binding));
        return *m_impl->m_vertexBufferLayouts.at(binding);
    }

    const VulkanIndexBufferLayout& VulkanInputAssembler::indexBufferLayout() const noexcept
    {
        return *m_impl->m_indexBufferLayout;
    }
}
