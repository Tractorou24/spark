#include "spark/render/vk/VulkanSampler.h"

#include "spark/base/Exception.h"
#include "spark/render/vk/VulkanDevice.h"

#include "vulkan/vulkan.h"

namespace spark::render::vk
{
    [[nodiscard]] VkFilter to_filter_mode(const FilterMode mode)
    {
        switch (mode)
        {
        case FilterMode::Linear:
            return VK_FILTER_LINEAR;
        case FilterMode::Nearest:
            return VK_FILTER_NEAREST;
        }
        throw base::BadArgumentException("Invalid filter mode.");
    }

    [[nodiscard]] VkSamplerMipmapMode to_mipmap_mode(const MipMapMode mode)
    {
        switch (mode)
        {
        case MipMapMode::Linear:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case MipMapMode::Nearest:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        }
        throw base::BadArgumentException("Invalid mip map mode.");
    }

    [[nodiscard]] VkSamplerAddressMode to_border_mode(const BorderMode mode)
    {
        switch (mode)
        {
        case BorderMode::Repeat:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case BorderMode::ClampToEdge:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case BorderMode::ClampToBorder:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case BorderMode::RepeatMirrored:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case BorderMode::ClampToEdgeMirrored:
            return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
        }
        throw base::BadArgumentException("Invalid border mode.");
    }

    struct VulkanSampler::Impl
    {
        friend class VulkanSampler;

    public:
        explicit Impl(const VulkanDevice& device,
                      const FilterMode mag_filter,
                      const FilterMode min_filter,
                      const BorderMode border_u,
                      const BorderMode border_v,
                      const BorderMode border_w,
                      const MipMapMode mip_map_mod,
                      const float mip_map_bias,
                      const float min_lod,
                      const float max_lod,
                      const float anisotropy)
            : m_device(device), m_magFilter(mag_filter), m_minFilter(min_filter), m_borderU(border_u), m_borderV(border_v), m_borderW(border_w), m_mipMapMode(mip_map_mod),
              m_mipMapBias(mip_map_bias), m_minLod(min_lod), m_maxLod(max_lod), m_anisotropy(anisotropy) {}

        VkSampler initialize() const
        {
            const VkSamplerCreateInfo sampler_info = {
                .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .magFilter = to_filter_mode(m_magFilter),
                .minFilter = to_filter_mode(m_minFilter),
                .mipmapMode = to_mipmap_mode(m_mipMapMode),
                .addressModeU = to_border_mode(m_borderU),
                .addressModeV = to_border_mode(m_borderV),
                .addressModeW = to_border_mode(m_borderW),
                .mipLodBias = m_mipMapBias,
                .anisotropyEnable = m_anisotropy > 1.0f ? VK_TRUE : VK_FALSE,
                .maxAnisotropy = m_anisotropy,
                .compareEnable = VK_FALSE,
                .compareOp = VK_COMPARE_OP_ALWAYS,
                .minLod = m_minLod,
                .maxLod = m_maxLod,
                .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
                .unnormalizedCoordinates = VK_FALSE,
            };

            VkSampler sampler = VK_NULL_HANDLE;
            if (vkCreateSampler(m_device.handle(), &sampler_info, nullptr, &sampler) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create sampler.");
            return sampler;
        }

    private:
        const VulkanDevice& m_device;

        FilterMode m_magFilter, m_minFilter;
        BorderMode m_borderU, m_borderV, m_borderW;
        MipMapMode m_mipMapMode;
        float m_mipMapBias;
        float m_minLod, m_maxLod;
        float m_anisotropy;
    };

    VulkanSampler::VulkanSampler(const VulkanDevice& device,
                                 FilterMode mag_filter,
                                 FilterMode min_filter,
                                 BorderMode border_u,
                                 BorderMode border_v,
                                 BorderMode border_w,
                                 MipMapMode mip_map_mode,
                                 float mip_map_bias,
                                 float min_lod,
                                 float max_lod,
                                 float anisotropy,
                                 const std::string& name)
        : Resource(VK_NULL_HANDLE),
          m_impl(std::make_unique<Impl>(device, mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, min_lod, max_lod, anisotropy))
    {
        handle() = m_impl->initialize();

        if (!name.empty())
            this->name() = name;
    }

    VulkanSampler::~VulkanSampler()
    {
        vkDestroySampler(m_impl->m_device.handle(), handle(), nullptr);
    }

    FilterMode VulkanSampler::minifyingFilter() const noexcept
    {
        return m_impl->m_minFilter;
    }

    FilterMode VulkanSampler::magnifyingFilter() const noexcept
    {
        return m_impl->m_magFilter;
    }

    std::array<BorderMode, 3> VulkanSampler::borderMode() const noexcept
    {
        return {m_impl->m_borderU, m_impl->m_borderV, m_impl->m_borderW};
    }

    float VulkanSampler::anisotropy() const noexcept
    {
        return m_impl->m_anisotropy;
    }

    MipMapMode VulkanSampler::mipMapMode() const noexcept
    {
        return m_impl->m_mipMapMode;
    }

    float VulkanSampler::mipMapBias() const noexcept
    {
        return m_impl->m_mipMapBias;
    }

    float VulkanSampler::minLod() const noexcept
    {
        return m_impl->m_minLod;
    }

    float VulkanSampler::maxLod() const noexcept
    {
        return m_impl->m_maxLod;
    }
}
