#pragma once

#include "spark/base/Macros.h"
#include "spark/render/Format.h"
#include "spark/render/Image.h"
#include "spark/render/Resource.h"
#include "spark/render/StateResource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"

#include "spark/math/Vector3.h"

#include <memory>
#include <string>

SPARK_FWD_DECLARE_VK_HANDLE(VkImage)
SPARK_FWD_DECLARE_VK_HANDLE(VkImageView)
SPARK_FWD_DECLARE_VK_HANDLE(VmaAllocator)
SPARK_FWD_DECLARE_VK_HANDLE(VmaAllocation)
using VkImageAspectFlags = uint32_t;
struct VkImageCreateInfo;
struct VmaAllocationCreateInfo;
struct VmaAllocationInfo;

namespace spark::render::vk
{
    class VulkanDevice;

    /**
     * \brief Represents a Vulkan image.
     */
    class SPARK_RENDER_VK_EXPORT IVulkanImage : public IImage, public virtual IResource<VkImage>
    {
    public:
        ~IVulkanImage() noexcept override = default;

        /**
         * \brief Gets the image aspect mask for all sub-resources.
         * \return A \ref VkImageAspectFlags representing the image aspect mask.
         */
        [[nodiscard]] virtual VkImageAspectFlags aspectMask() const noexcept = 0;

        /**
         * \brief Gets the image aspect mask for the given sub-resource.
         * \param plane The sub-resource index to get the aspect mask for.
         * \return A \ref VkImageAspectFlags representing the aspect mask for the given sub-resource.
         */
        [[nodiscard]] virtual VkImageAspectFlags aspectMask(unsigned int plane = 0) const = 0;

        /**
         * \brief Gets an image view for the given sub-resource.
         * \param plane The sub-resource index to get the image view for.
         * \return A \ref VkImageView representing the image view for the given sub-resource.
         */
        [[nodiscard]] virtual const VkImageView& imageView(unsigned int plane = 0) const = 0;
    };

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanImage': inherits 'StateResource::name' via dominance

    /**
     * \brief Vulkan implementation of \ref IImage.
     */
    class SPARK_RENDER_VK_EXPORT VulkanImage final : public IVulkanImage, public Resource<VkImage>, public StateResource
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanImage.
         * \param device 
         * \param image 
         * \param extent 
         * \param format 
         * \param dimensions 
         * \param levels 
         * \param layers 
         * \param samples 
         * \param writable 
         * \param initial_layout 
         * \param allocator 
         * \param allocation 
         * \param name 
         */
        explicit VulkanImage(const VulkanDevice& device,
                             VkImage image,
                             const math::Vector3<unsigned>& extent,
                             Format format,
                             ImageDimensions dimensions,
                             unsigned levels,
                             unsigned layers,
                             MultiSamplingLevel samples,
                             bool writable,
                             ImageLayout initial_layout,
                             VmaAllocator allocator = nullptr,
                             VmaAllocation allocation = nullptr,
                             const std::string& name = "");
        ~VulkanImage() override;

        VulkanImage(const VulkanImage& other) = delete;
        VulkanImage(VulkanImage&& other) noexcept = delete;
        VulkanImage& operator=(const VulkanImage& other) = delete;
        VulkanImage& operator=(VulkanImage&& other) noexcept = delete;

        /**
         * \brief 
         * \param device 
         * \param extent 
         * \param format 
         * \param dimensions 
         * \param levels 
         * \param layers 
         * \param samples 
         * \param writable 
         * \param initial_layout 
         * \param allocator 
         * \param create_info 
         * \param allocation_info 
         * \param allocation_result 
         * \return 
         */
        static std::unique_ptr<VulkanImage> Allocate(const VulkanDevice& device,
                                                     const math::Vector3<unsigned>& extent,
                                                     Format format,
                                                     ImageDimensions dimensions,
                                                     unsigned levels,
                                                     unsigned layers,
                                                     MultiSamplingLevel samples,
                                                     bool writable,
                                                     ImageLayout initial_layout,
                                                     VmaAllocator& allocator,
                                                     const VkImageCreateInfo& create_info,
                                                     const VmaAllocationCreateInfo& allocation_info,
                                                     VmaAllocationInfo* allocation_result = nullptr);

        /**
         * \brief 
         * \param name 
         * \param device 
         * \param extent 
         * \param format 
         * \param dimensions 
         * \param levels 
         * \param layers 
         * \param samples 
         * \param writable 
         * \param initial_layout 
         * \param allocator 
         * \param create_info 
         * \param allocation_info 
         * \param allocation_result 
         * \return 
         */
        static std::unique_ptr<VulkanImage> Allocate(const std::string& name,
                                                     const VulkanDevice& device,
                                                     const math::Vector3<unsigned>& extent,
                                                     Format format,
                                                     ImageDimensions dimensions,
                                                     unsigned levels,
                                                     unsigned layers,
                                                     MultiSamplingLevel samples,
                                                     bool writable,
                                                     ImageLayout initial_layout,
                                                     VmaAllocator& allocator,
                                                     const VkImageCreateInfo& create_info,
                                                     const VmaAllocationCreateInfo& allocation_info,
                                                     VmaAllocationInfo* allocation_result = nullptr);

        /// \copydoc IVulkanImage::aspectMask()
        [[nodiscard]] VkImageAspectFlags aspectMask() const noexcept override;

        /// \copydoc IVulkanImage::aspectMask()
        [[nodiscard]] VkImageAspectFlags aspectMask(unsigned plane) const override;

        /// \copydoc IVulkanImage::imageView()
        [[nodiscard]] const VkImageView& imageView(unsigned plane = 0) const override;

        /// \copydoc IVulkanImage::elements()
        [[nodiscard]] unsigned elements() const noexcept override;

        /// \copydoc IVulkanImage::size()
        [[nodiscard]] std::size_t size() const noexcept override;

        /// \copydoc IVulkanImage::elementSize()
        [[nodiscard]] std::size_t elementSize() const noexcept override;

        /// \copydoc IVulkanImage::elementAlignment()
        [[nodiscard]] std::size_t elementAlignment() const override;

        /// \copydoc IVulkanImage::alignedElementSize()
        [[nodiscard]] std::size_t alignedElementSize() const noexcept override;

        /// \copydoc IVulkanImage::writable()
        [[nodiscard]] bool writable() const noexcept override;

        /// \copydoc IVulkanImage::size()
        [[nodiscard]] std::size_t size(unsigned level) const override;

        /// \copydoc IVulkanImage::extent()
        [[nodiscard]] math::Vector3<unsigned> extent(unsigned level) const noexcept override;

        /// \copydoc IVulkanImage::format()
        [[nodiscard]] Format format() const noexcept override;

        /// \copydoc IVulkanImage::dimensions()
        [[nodiscard]] ImageDimensions dimensions() const noexcept override;

        /// \copydoc IVulkanImage::levels()
        [[nodiscard]] unsigned levels() const noexcept override;

        /// \copydoc IVulkanImage::layers()
        [[nodiscard]] unsigned layers() const noexcept override;

        /// \copydoc IVulkanImage::planes()
        [[nodiscard]] unsigned planes() const noexcept override;

        /// \copydoc IVulkanImage::samples()
        [[nodiscard]] MultiSamplingLevel samples() const noexcept override;

        /// \copydoc IVulkanImage::layout()
        [[nodiscard]] ImageLayout layout(unsigned sub_resource) const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
