#pragma once

#include "spark/render/InputAssembler.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanIndexBufferLayout.h"
#include "spark/render/vk/VulkanVertexBufferLayout.h"

#include <memory>
#include <vector>

namespace spark::render::vk
{
    class VulkanVertexBufferLayout;
    class VulkanIndexBufferLayout;

    /**
     * \brief Vulkan implementation of \ref IInputAssembler.
     */
    class SPARK_RENDER_VK_EXPORT VulkanInputAssembler final : public InputAssembler<VulkanVertexBufferLayout, VulkanIndexBufferLayout>
    {
    public:
        /**
         * \brief Initializes a new Vulkan input assembler state. 
         * \param vertex_buffer_layouts The vertex buffer layouts supported by this input assembler state. Each layout must have a unique binding.
         * \param index_buffer_layout The index buffer layout of the input assembler state.
         * \param primitive_topology The primitive topology of the input assembler state.
         *
         * \throws base::NullPointerException if one of the \p vertex_buffer_layouts or \p index_buffer_layout is null.
         * \throws base::BadArgumentException if the \p vertex_buffer_layouts contain multiple layouts with the same binding.
         */
        explicit VulkanInputAssembler(std::vector<std::unique_ptr<VulkanVertexBufferLayout>>&& vertex_buffer_layouts,
                                      std::unique_ptr<VulkanIndexBufferLayout>&& index_buffer_layout,
                                      PrimitiveTopology primitive_topology = PrimitiveTopology::TriangleList);
        ~VulkanInputAssembler() override;

        VulkanInputAssembler(const VulkanInputAssembler& other) = delete;
        VulkanInputAssembler(VulkanInputAssembler&& other) noexcept = delete;
        VulkanInputAssembler& operator=(const VulkanInputAssembler& other) = delete;
        VulkanInputAssembler& operator=(VulkanInputAssembler&& other) noexcept = delete;

        /// \copydoc InputAssembler::topology()
        [[nodiscard]] PrimitiveTopology topology() const noexcept override;

        /// \copydoc InputAssembler::vertexBufferLayouts()
        [[nodiscard]] std::vector<const VulkanVertexBufferLayout*> vertexBufferLayouts() const noexcept override;

        /// \copydoc InputAssembler::vertexBufferLayout()
        [[nodiscard]] const VulkanVertexBufferLayout& vertexBufferLayout(unsigned binding) const override;

        /// \copydoc InputAssembler::indexBufferLayout()
        [[nodiscard]] const VulkanIndexBufferLayout& indexBufferLayout() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
