#pragma once

#include "spark/render/Resource.h"
#include "spark/render/Sampler.h"
#include "spark/render/StateResource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"

#include "spark/base/Macros.h"

#include <memory>

SPARK_FWD_DECLARE_VK_HANDLE(VkSampler)

namespace spark::render::vk
{
    class VulkanDevice;

    /**
     * \brief Interface for Vulkan sampler.
     */
    class SPARK_RENDER_VK_EXPORT IVulkanSampler : public ISampler, public virtual IResource<VkSampler> {};

    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4250) // 'VulkanSampler': inherits 'Resource<VkSampler>::handle' via dominance

    /**
     * \brief Vulkan implementation of \ref ISampler.
     */
    class SPARK_RENDER_VK_EXPORT VulkanSampler final : public IVulkanSampler, public Resource<VkSampler>, public StateResource
    {
    public:
        /**
         * \brief 
         * \param device 
         * \param mag_filter 
         * \param min_filter 
         * \param border_u 
         * \param border_v 
         * \param border_w 
         * \param mip_map_mode 
         * \param mip_map_bias 
         * \param min_lod 
         * \param max_lod 
         * \param anisotropy 
         * \param name 
         */
        explicit VulkanSampler(const VulkanDevice& device,
                               FilterMode mag_filter = FilterMode::Nearest,
                               FilterMode min_filter = FilterMode::Nearest,
                               BorderMode border_u = BorderMode::Repeat,
                               BorderMode border_v = BorderMode::Repeat,
                               BorderMode border_w = BorderMode::Repeat,
                               MipMapMode mip_map_mode = MipMapMode::Nearest,
                               float mip_map_bias = 0.f,
                               float min_lod = 0.f,
                               float max_lod = std::numeric_limits<float>::max(),
                               float anisotropy = 0.f,
                               const std::string& name = "");
        ~VulkanSampler() override;

        VulkanSampler(const VulkanSampler& other) = delete;
        VulkanSampler(VulkanSampler&& other) noexcept = delete;
        VulkanSampler& operator=(const VulkanSampler& other) = delete;
        VulkanSampler& operator=(VulkanSampler&& other) noexcept = delete;

        /// \copydoc ISampler::minifyingFilter()
        [[nodiscard]] FilterMode minifyingFilter() const noexcept override;

        /// \copydoc ISampler::magnifyingFilter()
        [[nodiscard]] FilterMode magnifyingFilter() const noexcept override;

        /// \copydoc ISampler::borderMode()
        [[nodiscard]] std::array<BorderMode, 3> borderMode() const noexcept override;

        /// \copydoc ISampler::anisotropy()
        [[nodiscard]] float anisotropy() const noexcept override;

        /// \copydoc ISampler::mipMapMode()
        [[nodiscard]] MipMapMode mipMapMode() const noexcept override;

        /// \copydoc ISampler::mipMapBias()
        [[nodiscard]] float mipMapBias() const noexcept override;

        /// \copydoc ISampler::minLod()
        [[nodiscard]] float minLod() const noexcept override;

        /// \copydoc ISampler::maxLod()
        [[nodiscard]] float maxLod() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    SPARK_WARNING_POP
}
