#include "spark/render/vk/VulkanPushConstantsRange.h"

#include "spark/base/Exception.h"

namespace spark::render::vk
{
    struct VulkanPushConstantsRange::Impl
    {
        friend VulkanPushConstantsRange;

    public:
        explicit Impl(const ShaderStage shader_stage, const unsigned offset, const unsigned size, const unsigned space, const unsigned binding)
            : m_shaderStage(shader_stage), m_offset(offset), m_size(size), m_space(space), m_binding(binding) {}

    private:
        ShaderStage m_shaderStage;
        unsigned m_offset, m_size, m_space, m_binding;
    };

    VulkanPushConstantsRange::VulkanPushConstantsRange(ShaderStage shader_stage, unsigned offset, unsigned size, unsigned space, unsigned binding)
        : m_impl(std::make_unique<Impl>(shader_stage, offset, size, space, binding))
    {
        if (offset % 4 != 0)
            throw base::BadArgumentException("The push constants range offset must be a multiple of 4 bytes.");

        if (size % 4 != 0)
            throw base::BadArgumentException("The push constants range size must be a multiple of 4 bytes.");

        // TODO: (C++23) Use std::to_underlying()
        const auto shader_stage_mask = static_cast<std::underlying_type_t<ShaderStage>>(shader_stage);
        if (!(shader_stage_mask && !(shader_stage_mask & (shader_stage_mask - 1))))
            throw base::BadArgumentException("A push constant range is only allowed to be associated with one shader stage.");
    }

    VulkanPushConstantsRange::~VulkanPushConstantsRange() = default;

    unsigned VulkanPushConstantsRange::space() const noexcept
    {
        return m_impl->m_space;
    }

    unsigned VulkanPushConstantsRange::binding() const noexcept
    {
        return m_impl->m_binding;
    }

    unsigned VulkanPushConstantsRange::offset() const noexcept
    {
        return m_impl->m_offset;
    }

    unsigned VulkanPushConstantsRange::size() const noexcept
    {
        return m_impl->m_size;
    }

    ShaderStage VulkanPushConstantsRange::stage() const noexcept
    {
        return m_impl->m_shaderStage;
    }
}
