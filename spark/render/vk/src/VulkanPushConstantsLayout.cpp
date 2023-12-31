#include "spark/render/vk/VulkanPushConstantsLayout.h"

#include "spark/base/Exception.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace spark::render::vk
{
    struct VulkanPushConstantsLayout::Impl
    {
        friend VulkanPushConstantsLayout;

    public:
        explicit Impl(const unsigned int size)
            : m_size(size)
        {
            // Align the size to 4 bytes.
            m_size = size % 4 == 0 ? (size + 4 - 1) & ~(size - 1) : size;

            // If the size is greater than 128 bytes, log a warning since this is not guaranteed to be supported on all hardware.
            if (m_size > 128)
                log::warning("The push constant layout backing memory is defined with a size greater than 128 bytes. Blocks larger than 128 bytes are not forbidden, but also not guaranteed to be supported on all hardware.");
        }

        void setRanges(std::vector<std::unique_ptr<VulkanPushConstantsRange>>&& ranges)
        {
            m_rangesPtr.reserve(ranges.size());
            std::ranges::transform(ranges, std::back_inserter(m_rangesPtr), [](auto&& range) { return std::move(range); });

            for (auto&& range : m_rangesPtr)
            {
                if (m_ranges.contains(range->stage()))
                    throw base::BadArgumentException("Only one push constant range can be mapped to a shader stage.");
                m_ranges.emplace(range->stage(), range.get());
            }
        }

    private:
        const VulkanPipelineLayout* m_pipelineLayout = nullptr;
        std::unordered_map<ShaderStage, VulkanPushConstantsRange*> m_ranges;
        std::vector<std::unique_ptr<VulkanPushConstantsRange>> m_rangesPtr;
        unsigned int m_size;
    };

    VulkanPushConstantsLayout::VulkanPushConstantsLayout(std::vector<std::unique_ptr<VulkanPushConstantsRange>>&& ranges, unsigned size)
        : m_impl(std::make_unique<Impl>(size))
    {
        m_impl->setRanges(std::move(ranges));
    }

    VulkanPushConstantsLayout::~VulkanPushConstantsLayout() = default;

    const VulkanPipelineLayout& VulkanPushConstantsLayout::pipelineLayout() const
    {
        if (!m_impl->m_pipelineLayout)
            throw base::NullPointerException("The push constant layout has not yet been added to a pipeline layout.");
        return *m_impl->m_pipelineLayout;
    }

    unsigned VulkanPushConstantsLayout::size() const noexcept
    {
        return m_impl->m_size;
    }

    const VulkanPushConstantsRange& VulkanPushConstantsLayout::range(ShaderStage stage) const
    {
        // TODO: (C++23) Use std::to_underlying() instead.
        const auto shader_stage_mask = static_cast<std::underlying_type_t<ShaderStage>>(stage);
        if (!(shader_stage_mask && !(shader_stage_mask & (shader_stage_mask - 1))))
            throw base::BadArgumentException("The stage mask must only contain one shader stage.");

        if (!m_impl->m_ranges.contains(stage))
            throw base::BadArgumentException("No push constant range has been associated with the provided shader stage.");
        return *m_impl->m_ranges.at(stage);
    }

    std::vector<const VulkanPushConstantsRange*> VulkanPushConstantsLayout::ranges() const noexcept
    {
        std::vector<const VulkanPushConstantsRange*> ranges;
        ranges.reserve(m_impl->m_rangesPtr.size());
        std::ranges::transform(m_impl->m_rangesPtr, std::back_inserter(ranges), [](auto&& range) { return range.get(); });
        return ranges;
    }

    void VulkanPushConstantsLayout::setPipelineLayout(const VulkanPipelineLayout& pipeline_layout)
    {
        if (m_impl->m_pipelineLayout)
            throw base::BadArgumentException("The push constant layout has already been added to a pipeline layout.");
        m_impl->m_pipelineLayout = &pipeline_layout;
    }
}
