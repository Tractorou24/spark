#pragma once

#include "spark/render/VertexBuffer.h"
#include "spark/render/vk/Export.h"

#include <memory>

namespace spark::render::vk
{
    /**
     * \brief Implements \ref IVertexBufferLayout for Vulkan.
     */
    class SPARK_RENDER_VK_EXPORT VulkanVertexBufferLayout final : public IVertexBufferLayout
    {
    public:
        /**
         * \brief Initializes a new \ref IVertexBufferLayout for use with Vulkan. 
         * \param vertex_size The size of a single vertex in.
         * \param binding The binding index of the vertex buffers using this layout.
         */
        explicit VulkanVertexBufferLayout(std::size_t vertex_size, unsigned int binding = 0);
        ~VulkanVertexBufferLayout() override;

        VulkanVertexBufferLayout(const VulkanVertexBufferLayout& other) = delete;
        VulkanVertexBufferLayout(VulkanVertexBufferLayout&& other) noexcept = delete;
        VulkanVertexBufferLayout& operator=(const VulkanVertexBufferLayout& other) = delete;
        VulkanVertexBufferLayout& operator=(VulkanVertexBufferLayout&& other) noexcept = delete;

        /// \copydoc IVertexBufferLayout::attributes()
        [[nodiscard]] std::vector<const BufferAttribute*> attributes() const noexcept override;

        /// \copydoc IVertexBufferLayout::addAttribute()
        void addAttribute(BufferAttribute&& attribute) noexcept override;

        /// \copydoc IVertexBufferLayout::elementSize()
        [[nodiscard]] std::size_t elementSize() const noexcept override;

        /// \copydoc IVertexBufferLayout::binding()
        [[nodiscard]] unsigned int binding() const noexcept override;

        /// \copydoc IVertexBufferLayout::type()
        [[nodiscard]] BufferType type() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
