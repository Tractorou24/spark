#pragma once

#include "spark/render/IndexBuffer.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanIndexBufferLayout.h"

namespace spark::render::vk
{
    /**
     * \brief Vulkan interface for an \ref IIndexBuffer.
     */
    class SPARK_RENDER_VK_EXPORT IVulkanIndexBuffer : public virtual IVulkanBuffer, public virtual IIndexBuffer
    {
    public:
        using index_buffer_layout_type = VulkanIndexBufferLayout;
    };

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanIndexBuffer' : inherits 'StateResource::name' via dominance

    /**
     * \brief Vulkan implementation of an \ref IIndexBuffer.
     */
    class SPARK_RENDER_VK_EXPORT VulkanIndexBuffer final : public VulkanBuffer, public IVulkanIndexBuffer
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
        explicit VulkanIndexBuffer(VkBuffer buffer,
                                   const VulkanIndexBufferLayout& layout,
                                   unsigned elements,
                                   const VmaAllocator& allocator,
                                   const VmaAllocation& allocation,
                                   const std::string& name = "");
        ~VulkanIndexBuffer() override;

        VulkanIndexBuffer(const VulkanIndexBuffer& other) = delete;
        VulkanIndexBuffer(VulkanIndexBuffer&& other) noexcept = delete;
        VulkanIndexBuffer& operator=(const VulkanIndexBuffer& other) = delete;
        VulkanIndexBuffer& operator=(VulkanIndexBuffer&& other) noexcept = delete;

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
        static std::unique_ptr<IVulkanIndexBuffer> Allocate(const VulkanIndexBufferLayout& layout,
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
        static std::unique_ptr<IVulkanIndexBuffer> Allocate(const std::string& name,
                                                            const VulkanIndexBufferLayout& layout,
                                                            unsigned elements,
                                                            const VmaAllocator& allocator,
                                                            const VkBufferCreateInfo& create_info,
                                                            const VmaAllocationCreateInfo& allocation_info,
                                                            VmaAllocationInfo* allocation_result = nullptr);

        /// \copydoc IVulkanIndexBuffer::layout()
        [[nodiscard]] const VulkanIndexBufferLayout& layout() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
