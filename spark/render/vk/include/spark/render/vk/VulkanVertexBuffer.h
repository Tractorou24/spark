#pragma once

#include "spark/render/VertexBuffer.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanVertexBufferLayout.h"

namespace spark::render::vk
{
    /**
     * \brief Vulkan interface for a \ref IVertexBuffer.
     */
    class SPARK_RENDER_VK_EXPORT IVulkanVertexBuffer : public virtual IVulkanBuffer, public virtual IVertexBuffer
    {
    public:
        using vertex_buffer_layout_type = VulkanVertexBufferLayout;
    };

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanVertexBuffer' : inherits 'StateResource::name' via dominance

    /**
     * \brief Vulkan implementation of a \ref IVertexBuffer.
     */
    class SPARK_RENDER_VK_EXPORT VulkanVertexBuffer final : public VulkanBuffer, public IVulkanVertexBuffer
    {
    public:
        /**
         * \brief 
         * \param buffer 
         * \param layout 
         * \param elements 
         * \param allocator 
         * \param allocation 
         * \param name 
         */
        explicit VulkanVertexBuffer(VkBuffer buffer,
                                    const VulkanVertexBufferLayout& layout,
                                    unsigned elements,
                                    const VmaAllocator& allocator,
                                    const VmaAllocation& allocation,
                                    const std::string& name = "");
        ~VulkanVertexBuffer() override;

        VulkanVertexBuffer(const VulkanVertexBuffer& other) = delete;
        VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept = delete;
        VulkanVertexBuffer& operator=(const VulkanVertexBuffer& other) = delete;
        VulkanVertexBuffer& operator=(VulkanVertexBuffer&& other) noexcept = delete;

        /**
         * \brief 
         * \param layout 
         * \param elements 
         * \param allocator 
         * \param create_info 
         * \param allocation_info 
         * \param allocation_result 
         * \return 
         */
        static std::unique_ptr<IVulkanVertexBuffer> Allocate(const VulkanVertexBufferLayout& layout,
                                                             unsigned elements,
                                                             const VmaAllocator& allocator,
                                                             const VkBufferCreateInfo& create_info,
                                                             const VmaAllocationCreateInfo& allocation_info,
                                                             VmaAllocationInfo* allocation_result = nullptr);

        /**
         * \brief
         * \param name 
         * \param layout 
         * \param elements 
         * \param allocator 
         * \param create_info 
         * \param allocation_info 
         * \param allocation_result 
         * \return 
         */
        static std::unique_ptr<IVulkanVertexBuffer> Allocate(const std::string& name,
                                                             const VulkanVertexBufferLayout& layout,
                                                             unsigned elements,
                                                             const VmaAllocator& allocator,
                                                             const VkBufferCreateInfo& create_info,
                                                             const VmaAllocationCreateInfo& allocation_info,
                                                             VmaAllocationInfo* allocation_result = nullptr);

        /// \copydoc IVulkanVertexBuffer::layout()
        [[nodiscard]] const VulkanVertexBufferLayout& layout() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
