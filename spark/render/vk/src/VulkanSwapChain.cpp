#include "spark/render/vk/VulkanSwapChain.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanFrameBuffer.h"

#include "spark/log/Logger.h"
#include "spark/math/Vector2.h"
#include "spark/math/Vector3.h"

namespace spark::render::vk
{
    struct VulkanSwapChain::Impl
    {
        friend class VulkanSwapChain;

    public:
        explicit Impl(const VulkanDevice& device)
            : m_device(device) {}

        void initialize(const Format format, const math::Vector2<unsigned>& render_area, const unsigned buffers)
        {
            if (format == Format::None || format == Format::Other)
                throw base::BadArgumentException("The provided surface format is invalid");

            auto adapter = m_device.graphicsAdapter().handle();
            auto surface = m_device.surface().handle();

            // Query the swap chain surface format
            auto supported_surface_formats = GetSurfaceFormats(adapter, surface);

            const auto match = std::ranges::find(supported_surface_formats, format);
            if (match == supported_surface_formats.end())
                throw base::BadArgumentException("The provided surface format is not supported");

            const Format selected_format = *match;

            // Get the number of images in the swap chain
            VkSurfaceCapabilitiesKHR device_capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(adapter, surface, &device_capabilities);
            if (device_capabilities.maxImageCount == 0) // 0 means unlimited
                device_capabilities.maxImageCount = std::numeric_limits<unsigned>::max();

            unsigned images = std::clamp(buffers, device_capabilities.minImageCount, device_capabilities.maxImageCount);

            // Create a swap chain
            const VkSwapchainCreateInfoKHR swap_chain_info = {
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .surface = surface,
                .minImageCount = images,
                .imageFormat = conversions::to_format(selected_format),
                .imageColorSpace = FindColorSpace(adapter, surface, selected_format),
                .imageExtent = {
                    .width = std::max<unsigned>(1, std::clamp(render_area.x, device_capabilities.minImageExtent.width, device_capabilities.maxImageExtent.width)),
                    .height = std::max<unsigned>(1, std::clamp(render_area.y, device_capabilities.minImageExtent.height, device_capabilities.maxImageExtent.height))
                },
                .imageArrayLayers = 1,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 0,
                .pQueueFamilyIndices = nullptr,
                .preTransform = device_capabilities.currentTransform,
                .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
                .clipped = VK_TRUE,
                .oldSwapchain = nullptr
            };

            log::trace("Creating swap chain for device {0} {{ Images: {1}, Extent: {2}x{3} Px, Format: {4} }}...",
                       reinterpret_cast<const void*>(&m_device),
                       images,
                       swap_chain_info.imageExtent.width,
                       swap_chain_info.imageExtent.height,
                       selected_format);

            // Log if something needed to be changed.
            if (selected_format != format)
                log::info("The format {0} has been changed to the compatible format {1}.", format, selected_format);

            if (swap_chain_info.imageExtent.height != render_area.y || swap_chain_info.imageExtent.width != render_area.x)
                log::info("The render area has been adjusted to {0}x{1} Px (was {2}x{3} Px).",
                          swap_chain_info.imageExtent.width,
                          swap_chain_info.imageExtent.height,
                          render_area.x,
                          render_area.y);

            if (images != buffers)
                log::info("The number of buffers has been adjusted from {0} to {1}.", buffers, images);

            // Create the swap chain
            VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
            if (vkCreateSwapchainKHR(m_device.handle(), &swap_chain_info, nullptr, &swap_chain) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create swap chain");

            // Create a semaphore for swapping images
            constexpr VkSemaphoreCreateInfo semaphore_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            };

            m_swapSemaphores.resize(images);
            std::ranges::generate(m_swapSemaphores,
                                  [&]
                                  {
                                      VkSemaphore semaphore = VK_NULL_HANDLE;
                                      if (vkCreateSemaphore(m_device.handle(), &semaphore_info, nullptr, &semaphore) != VK_SUCCESS)
                                          throw base::NullPointerException("Failed to create swap semaphore");
                                      return semaphore;
                                  });

            // Create the swap chain images
            const auto actual_render_area = math::Vector2<unsigned>(swap_chain_info.imageExtent.width, swap_chain_info.imageExtent.height);
            std::vector<VkImage> swap_chain_images(images);
            vkGetSwapchainImagesKHR(m_device.handle(), swap_chain, &images, swap_chain_images.data());

            m_presentImages.clear();
            std::ranges::transform(swap_chain_images,
                                   std::back_inserter(m_presentImages),
                                   [&](const VkImage& image)
                                   {
                                       return std::make_unique<VulkanImage>(m_device,
                                                                            image,
                                                                            math::Vector3<unsigned> {actual_render_area.x, actual_render_area.y, 1},
                                                                            selected_format,
                                                                            ImageDimensions::DIM_2,
                                                                            1,
                                                                            1,
                                                                            MultiSamplingLevel::X1,
                                                                            false,
                                                                            ImageLayout::Undefined);
                                   });

            std::vector<std::unique_ptr<IVulkanImage>> present_images;
            std::ranges::transform(swap_chain_images,
                                   std::back_inserter(present_images),
                                   [&](const VkImage& image)
                                   {
                                       return std::make_unique<VulkanImage>(m_device,
                                                                            image,
                                                                            math::Vector3<unsigned>(actual_render_area.x, actual_render_area.y, 1),
                                                                            selected_format,
                                                                            ImageDimensions::DIM_2,
                                                                            1,
                                                                            1,
                                                                            MultiSamplingLevel::X1,
                                                                            false,
                                                                            ImageLayout::Present);
                                   });

            // Store state variables
            m_renderArea = actual_render_area;
            m_format = selected_format;
            m_buffers = images;
            m_currentImage = 0;
            m_handle = swap_chain;
        }

        void reset(const Format format, const math::Vector2<unsigned>& render_area, const unsigned buffers)
        {
            cleanup();
            initialize(format, render_area, buffers);
        }

        void cleanup()
        {
            // Destroy the swap chain
            vkDestroySwapchainKHR(m_device.handle(), m_handle, nullptr);

            // Destroy the swap chain semaphores
            for (const auto& semaphore : m_swapSemaphores)
                vkDestroySemaphore(m_device.handle(), semaphore, nullptr);

            // Destroy state
            m_swapSemaphores.clear();
            m_buffers = 0;
            m_renderArea = {0, 0};
            m_format = Format::None;
            m_currentImage = 0;
        }

        [[nodiscard]] unsigned swapBackBuffer()
        {
            unsigned next_image = 0;
            m_currentImage++;
            if (vkAcquireNextImageKHR(m_device.handle(), m_handle, UINT64_MAX, m_swapSemaphores[m_currentImage % m_buffers], VK_NULL_HANDLE, &next_image) != VK_SUCCESS)
                throw base::NullPointerException("Failed to acquire next image");
            return next_image;
        }

        void present(const VulkanFrameBuffer& frame_buffer)
        {
            // Draw the frame, if the result of the render pass it should be presented to the swap chain.
            std::array<VkSwapchainKHR, 1> swap_chains = {m_handle};
            std::array<VkSemaphore, 1> signal_semaphores = {frame_buffer.semaphore()};
            const auto buffer_index = frame_buffer.bufferIndex();

            const VkPresentInfoKHR present_info = {
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .pNext = nullptr,
                .waitSemaphoreCount = static_cast<unsigned>(signal_semaphores.size()),
                .pWaitSemaphores = signal_semaphores.data(),
                .swapchainCount = static_cast<unsigned>(swap_chains.size()),
                .pSwapchains = swap_chains.data(),
                .pImageIndices = &buffer_index,
                .pResults = nullptr
            };

            if (vkQueuePresentKHR(m_device.graphicsQueue().handle(), &present_info) != VK_SUCCESS)
                throw base::NullPointerException("Failed to present frame buffer");
        }

        [[nodiscard]] static std::vector<Format> GetSurfaceFormats(const VkPhysicalDevice adapter, const VkSurfaceKHR surface) noexcept
        {
            unsigned formats = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &formats, nullptr);

            std::vector<VkSurfaceFormatKHR> vk_available_formats(formats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &formats, vk_available_formats.data());

            std::vector<Format> available_formats;
            std::ranges::transform(vk_available_formats,
                                   std::back_inserter(available_formats),
                                   [](const VkSurfaceFormatKHR& surface_format)
                                   {
                                       return conversions::from_format(surface_format.format);
                                   });
            return available_formats;
        }

        [[nodiscard]] static VkColorSpaceKHR FindColorSpace(const VkPhysicalDevice adapter, const VkSurfaceKHR surface, Format format) noexcept
        {
            uint32_t formats;
            vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &formats, nullptr);

            std::vector<VkSurfaceFormatKHR> available_formats(formats);
            vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &formats, available_formats.data());

            const auto match = std::ranges::find_if(available_formats,
                                                    [&format](const VkSurfaceFormatKHR& surface_format)
                                                    {
                                                        return surface_format.format == conversions::to_format(format);
                                                    });
            if (match != available_formats.end())
                return match->colorSpace;
            return VK_COLOR_SPACE_MAX_ENUM_KHR;
        }

    private:
        const VulkanDevice& m_device;
        std::vector<std::unique_ptr<IVulkanImage>> m_presentImages;
        math::Vector2<unsigned> m_renderArea;
        Format m_format = Format::None;
        unsigned m_buffers = 0;
        unsigned m_currentImage = 0;

        std::vector<VkSemaphore> m_swapSemaphores {};
        VkSwapchainKHR m_handle = VK_NULL_HANDLE;
    };

    VulkanSwapChain::VulkanSwapChain(const VulkanDevice& device, const Format surface_format, const math::Vector2<unsigned>& render_area, const unsigned buffers)
        : m_impl(std::make_unique<Impl>(device))
    {
        m_impl->initialize(surface_format, render_area, buffers);
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        m_impl->cleanup();
    }

    const VkSemaphore& VulkanSwapChain::semaphore() const noexcept
    {
        return m_impl->m_swapSemaphores[m_impl->m_currentImage % m_impl->m_buffers];
    }

    unsigned VulkanSwapChain::buffers() const noexcept
    {
        return m_impl->m_buffers;
    }

    math::Vector2<unsigned> VulkanSwapChain::renderArea() const noexcept
    {
        return m_impl->m_renderArea;
    }

    Format VulkanSwapChain::surfaceFormat() const noexcept
    {
        return m_impl->m_format;
    }

    std::vector<Format> VulkanSwapChain::surfaceFormats() const noexcept
    {
        return Impl::GetSurfaceFormats(m_impl->m_device.graphicsAdapter().handle(), m_impl->m_device.surface().handle());
    }

    void VulkanSwapChain::reset(const Format surface_format, const math::Vector2<unsigned> render_area, const unsigned buffers) noexcept
    {
        m_impl->reset(surface_format, render_area, buffers);
    }

    unsigned VulkanSwapChain::swapBackBuffer() const noexcept
    {
        return m_impl->swapBackBuffer();
    }

    std::vector<const IVulkanImage*> VulkanSwapChain::images() const noexcept
    {
        std::vector<const IVulkanImage*> images;
        images.reserve(m_impl->m_buffers);
        std::ranges::transform(m_impl->m_presentImages, std::back_inserter(images), [](const auto& image) { return image.get(); });
        return images;
    }

    const IVulkanImage* VulkanSwapChain::image(unsigned back_buffer) const
    {
        if (back_buffer >= m_impl->m_buffers)
            throw base::ArgumentOutOfRangeException(std::format("Back buffer index {} is out of range", back_buffer));
        return m_impl->m_presentImages[back_buffer].get();
    }

    void VulkanSwapChain::present(const VulkanFrameBuffer& frame_buffer) const noexcept
    {
        m_impl->present(frame_buffer);
    }
}
