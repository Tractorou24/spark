#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"

#include "spark/base/Macros.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkBuffer)
SPARK_FWD_DECLARE_VK_HANDLE(VmaAllocator)
SPARK_FWD_DECLARE_VK_HANDLE(VmaAllocation)
struct VkBufferCreateInfo;
struct VmaAllocationCreateInfo;
struct VmaAllocationInfo;

namespace spark::render::vk
{
    /**
     * \brief Vulkan interface for a \ref IBuffer.
     */
    class SPARK_RENDER_VK_EXPORT IVulkanBuffer : public virtual IBuffer, public virtual IResource<VkBuffer> {};

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanBuffer': inherits 'Resource<VkBuffer>::handle' via dominance

    /**
     * \brief Vulkan implementation of \ref IBuffer.
     */
    class SPARK_RENDER_VK_EXPORT VulkanBuffer : public virtual IVulkanBuffer, public Resource<VkBuffer>, public StateResource
    {
    public:
        /**
         * \brief 
         * \param buffer 
         * \param type 
         * \param elements 
         * \param element_size 
         * \param alignment 
         * \param writable 
         * \param allocator 
         * \param allocation 
         * \param name 
         */
        explicit VulkanBuffer(VkBuffer buffer,
                              BufferType type,
                              unsigned elements,
                              std::size_t element_size,
                              std::size_t alignment,
                              bool writable,
                              const VmaAllocator& allocator,
                              const VmaAllocation& allocation,
                              const std::string& name);
        ~VulkanBuffer() override;

        VulkanBuffer(const VulkanBuffer& other) = delete;
        VulkanBuffer(VulkanBuffer&& other) noexcept = delete;
        VulkanBuffer& operator=(const VulkanBuffer& other) = delete;
        VulkanBuffer& operator=(VulkanBuffer&& other) noexcept = delete;

        /**
         * \brief 
         * \param type 
         * \param elements 
         * \param element_size 
         * \param alignment 
         * \param writable 
         * \param allocator 
         * \param create_info 
         * \param allocation_info 
         * \param allocation_result 
         * \return 
         */
        static std::unique_ptr<IVulkanBuffer> Allocate(BufferType type,
                                                       unsigned elements,
                                                       std::size_t element_size,
                                                       std::size_t alignment,
                                                       bool writable,
                                                       const VmaAllocator& allocator,
                                                       const VkBufferCreateInfo& create_info,
                                                       const VmaAllocationCreateInfo& allocation_info,
                                                       VmaAllocationInfo* allocation_result = nullptr);

        /**
         * \brief 
         * \param name 
         * \param type 
         * \param elements 
         * \param element_size 
         * \param alignment 
         * \param writable 
         * \param allocator 
         * \param create_info 
         * \param allocation_info 
         * \param allocation_result 
         * \return 
         */
        static std::unique_ptr<IVulkanBuffer> Allocate(const std::string& name,
                                                       BufferType type,
                                                       unsigned elements,
                                                       std::size_t element_size,
                                                       std::size_t alignment,
                                                       bool writable,
                                                       const VmaAllocator& allocator,
                                                       const VkBufferCreateInfo& create_info,
                                                       const VmaAllocationCreateInfo& allocation_info,
                                                       VmaAllocationInfo* allocation_result = nullptr);

        /// \copydoc IBuffer::type()
        [[nodiscard]] BufferType type() const override;

        /// \copydoc IDeviceMemory::elements()
        [[nodiscard]] unsigned elements() const noexcept override;

        /// \copydoc IDeviceMemory::size()
        [[nodiscard]] std::size_t size() const noexcept override;

        /// \copydoc IDeviceMemory::elementSize()
        [[nodiscard]] std::size_t elementSize() const noexcept override;

        /// \copydoc IDeviceMemory::elementAlignment()
        [[nodiscard]] std::size_t elementAlignment() const override;

        /// \copydoc IDeviceMemory::alignedElementSize()
        [[nodiscard]] std::size_t alignedElementSize() const noexcept override;

        /// \copydoc IDeviceMemory::writable()
        [[nodiscard]] bool writable() const noexcept override;

        /// \copydoc IMappable::map()
        void map(const void* data, std::size_t size, unsigned element) override;

        /// \copydoc IMappable::map()
        void map(std::span<const void*> data, std::size_t element_size, unsigned first_element) override;

        /// \copydoc IMappable::map()
        void map(void* data, size_t size, unsigned element, bool write) override;

        /// \copydoc IMappable::map()
        void map(std::span<void*> data, size_t element_size, unsigned first_element, bool write) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
