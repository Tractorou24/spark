#pragma once

#include "spark/render/IndexBuffer.h"
#include "spark/render/vk/Export.h"

namespace spark::render::vk
{
    /**
     * \brief Implements \ref IIndexBufferLayout for Vulkan.
     */
    class SPARK_RENDER_VK_EXPORT VulkanIndexBufferLayout final : public IIndexBufferLayout
    {
    public:
        /**
         * \brief Instantiate a new \ref VulkanIndexBufferLayout for use with Vulkan. 
         * \param type The types of the indices in the index buffer.
         */
        explicit VulkanIndexBufferLayout(IndexType type);
        ~VulkanIndexBufferLayout() override;

        VulkanIndexBufferLayout(const VulkanIndexBufferLayout& other) = delete;
        VulkanIndexBufferLayout(VulkanIndexBufferLayout&& other) noexcept = delete;
        VulkanIndexBufferLayout& operator=(const VulkanIndexBufferLayout& other) = delete;
        VulkanIndexBufferLayout& operator=(VulkanIndexBufferLayout&& other) noexcept = delete;

        /// \copydoc IIndexBufferLayout::elementSize()
        [[nodiscard]] std::size_t elementSize() const noexcept override;

        /// \copydoc IIndexBufferLayout::binding()
        [[nodiscard]] unsigned binding() const noexcept override;

        /// \copydoc IIndexBufferLayout::type()
        [[nodiscard]] BufferType type() const noexcept override;

        /// \copydoc IIndexBufferLayout::indexType()
        [[nodiscard]] IndexType indexType() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
