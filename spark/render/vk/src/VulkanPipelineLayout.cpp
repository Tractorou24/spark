#include "spark/render/vk/VulkanPipelineLayout.h"
#include "spark/render/vk/Conversions.h"
#include "spark/render/vk/VulkanDescriptorSetLayout.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanPushConstantsLayout.h"
#include "spark/render/vk/VulkanPushConstantsRange.h"

#include "spark/base/Exception.h"
#include "spark/log/Logger.h"

#include <ranges>

namespace spark::render::vk
{
    struct VulkanPipelineLayout::Impl
    {
        friend class VulkanPipelineLayout;

    public:
        explicit Impl(VulkanPipelineLayout* parent,
                      const VulkanDevice& device,
                      std::vector<std::unique_ptr<VulkanDescriptorSetLayout>>&& descriptor_set_layouts,
                      std::unique_ptr<VulkanPushConstantsLayout>&& push_constants_layout)
            : m_parent(parent), m_device(device), m_descriptorSetLayouts(std::move(descriptor_set_layouts)), m_pushConstantsLayout(std::move(push_constants_layout)) {}

        VkPipelineLayout initialize()
        {
            // In vulkan, descriptor sets are mapped to their indices based on the order they are defined. So, sort by space to define them in the correct order.
            std::ranges::sort(m_descriptorSetLayouts,
                              [](const std::unique_ptr<VulkanDescriptorSetLayout>& a, const std::unique_ptr<VulkanDescriptorSetLayout>& b)
                              {
                                  return a->space() < b->space();
                              });

            // Find unused or duplicated descriptor set
            std::vector<std::tuple<unsigned, unsigned>> tuples;
            auto tmp_tuples = m_descriptorSetLayouts | std::views::transform([](const std::unique_ptr<VulkanDescriptorSetLayout>& layout) { return layout->space(); });
            for (auto it = tmp_tuples.begin(); it != tmp_tuples.end(); ++it)
                if (it + 1 != tmp_tuples.end())
                    tuples.emplace_back(*it, *(it + 1));

            std::vector<unsigned> empty_sets;
            for (unsigned i = 0; const std::tuple<unsigned, unsigned>& tuple : tuples)
            {
                auto [a, b] = tuple;

                if (a == b)
                    throw base::BadArgumentException(std::format("Two layouts defined for the same descriptor set {}. Each descriptor set must use it's own space.", a));

                while (i != a)
                    empty_sets.push_back(i++);
            }

            if (!empty_sets.empty())
            {
                for (const auto set : empty_sets)
                    m_descriptorSetLayouts.push_back(std::unique_ptr<VulkanDescriptorSetLayout> {
                                                         new VulkanDescriptorSetLayout(m_device, {}, set, ShaderStage::Vertex | ShaderStage::Fragment, 0, 0)
                                                     }); // No descriptor can ever be allocated from an empty descriptor set.
                // Re-order them.
                std::ranges::sort(m_descriptorSetLayouts,
                                  [](const std::unique_ptr<VulkanDescriptorSetLayout>& a, const std::unique_ptr<VulkanDescriptorSetLayout>& b)
                                  {
                                      return a->space() < b->space();
                                  });
            }

            // Store the pipeline layout in the push constants layout
            if (m_pushConstantsLayout)
                m_pushConstantsLayout->setPipelineLayout(*m_parent);

            // Get descriptor set layouts
            std::vector<VkDescriptorSetLayout> layouts;
            std::ranges::transform(m_descriptorSetLayouts,
                                   std::back_inserter(layouts),
                                   [](const auto& descriptor_set_layout) { return std::as_const(*descriptor_set_layout.get()).handle(); });

            // Get push constant ranges
            std::vector<VkPushConstantRange> ranges_handles;
            auto ranges = m_pushConstantsLayout ? m_pushConstantsLayout->ranges() : std::vector<const VulkanPushConstantsRange*> {};
            std::ranges::transform(ranges,
                                   std::back_inserter(ranges_handles),
                                   [](const VulkanPushConstantsRange* range)
                                   {
                                       return VkPushConstantRange {
                                           .stageFlags = static_cast<VkShaderStageFlags>(conversions::to_shader_stage(range->stage())),
                                           .offset = range->offset(),
                                           .size = range->size()
                                       };
                                   });

            log::trace("Creating pipeline layout {0} {{ Descriptor Sets: {1}, Push Constant Ranges: {2} }}...",
                       static_cast<void*>(m_parent),
                       layouts.size(),
                       ranges_handles.size());

            // Create pipeline layout
            const VkPipelineLayoutCreateInfo pipeline_layout_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .setLayoutCount = static_cast<unsigned>(layouts.size()),
                .pSetLayouts = layouts.data(),
                .pushConstantRangeCount = static_cast<unsigned>(ranges_handles.size()),
                .pPushConstantRanges = ranges_handles.data()
            };

            VkPipelineLayout layout = {};
            if (vkCreatePipelineLayout(m_device.handle(), &pipeline_layout_info, nullptr, &layout) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create pipeline layout");

            return layout;
        }

    private:
        VulkanPipelineLayout* m_parent;
        const VulkanDevice& m_device;
        std::vector<std::unique_ptr<VulkanDescriptorSetLayout>> m_descriptorSetLayouts;
        std::unique_ptr<VulkanPushConstantsLayout> m_pushConstantsLayout;
    };

    VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice& device,
                                               std::vector<std::unique_ptr<VulkanDescriptorSetLayout>>&& descriptor_set_layouts,
                                               std::unique_ptr<VulkanPushConstantsLayout>&& push_constants_layout)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(this, device, std::move(descriptor_set_layouts), std::move(push_constants_layout)))
    {
        handle() = m_impl->initialize();
    }

    VulkanPipelineLayout::~VulkanPipelineLayout()
    {
        ::vkDestroyPipelineLayout(m_impl->m_device.handle(), handle(), nullptr);
    }

    const VulkanDevice& VulkanPipelineLayout::device() const noexcept
    {
        return m_impl->m_device;
    }

    const VulkanDescriptorSetLayout& VulkanPipelineLayout::descriptorSet(unsigned space) const
    {
        const auto match = std::ranges::find_if(m_impl->m_descriptorSetLayouts, [space](const auto& descriptor_set_layout) { return descriptor_set_layout->space() == space; });
        if (match != m_impl->m_descriptorSetLayouts.end())
            return *match->get();
        throw base::BadArgumentException(std::format("No descriptor set layout uses provided space {}", space));
    }

    std::vector<const VulkanDescriptorSetLayout*> VulkanPipelineLayout::descriptorSets() const noexcept
    {
        std::vector<const VulkanDescriptorSetLayout*> layouts;
        layouts.reserve(m_impl->m_descriptorSetLayouts.size());
        std::ranges::transform(m_impl->m_descriptorSetLayouts, std::back_inserter(layouts), [](const auto& descriptor_set_layout) { return descriptor_set_layout.get(); });
        return layouts;
    }

    const VulkanPushConstantsLayout* VulkanPipelineLayout::pushConstants() const noexcept
    {
        return m_impl->m_pushConstantsLayout.get();
    }
}
