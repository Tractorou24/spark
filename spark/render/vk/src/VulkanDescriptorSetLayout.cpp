#include "spark/render/vk/VulkanDescriptorSetLayout.h"
#include "spark/render/vk/Formatters.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanDescriptorLayout.h"
#include "spark/render/vk/VulkanDescriptorSet.h"
#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/VulkanSampler.h"

#include "spark/base/Exception.h"
#include "spark/lib/Overloaded.h"

#include <mutex>
#include <queue>

#include "spark/log/Logger.h"

namespace spark::render::vk
{
    struct VulkanDescriptorSetLayout::Impl
    {
        friend class VulkanDescriptorSetLayout;

    public:
        explicit Impl(VulkanDescriptorSetLayout* parent,
                      const VulkanDevice& device,
                      std::vector<std::unique_ptr<VulkanDescriptorLayout>>&& descriptor_layouts,
                      const unsigned space,
                      const ShaderStage stage)
            : m_parent(parent), m_device(device), m_stages(stage), m_space(space), m_descriptorLayouts(std::move(descriptor_layouts)) {}

        VkDescriptorSetLayout initialize(unsigned int pool_size, const unsigned int max_unbounded_array_size)
        {
            log::trace("Defining layout for descriptor set {0} {{ Stages: {1}, Pool Size: {2} }}...", m_space, m_stages, m_poolSize);

            // Figure out the proper pool size.
            if (m_descriptorLayouts.empty() && pool_size > 0)
            {
                log::warning("The descriptor set layout does not contain any descriptors but pool size is set to `{}` and will be reset to `0`. Allocating empty descriptor sets is not valid will raise an error.",
                             pool_size);
                pool_size = 0;
            }

            m_poolSize = pool_size;

            // Parse the shader stage descriptor
            VkShaderStageFlags stage_flags = 0;
            if ((m_stages & ShaderStage::Vertex) == ShaderStage::Vertex)
                stage_flags |= VK_SHADER_STAGE_VERTEX_BIT;
            if ((m_stages & ShaderStage::Geometry) == ShaderStage::Geometry)
                stage_flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
            if ((m_stages & ShaderStage::Fragment) == ShaderStage::Fragment)
                stage_flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
            if ((m_stages & ShaderStage::TessellationEvaluation) == ShaderStage::TessellationEvaluation)
                stage_flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            if ((m_stages & ShaderStage::TessellationControl) == ShaderStage::TessellationControl)
                stage_flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            if ((m_stages & ShaderStage::Compute) == ShaderStage::Compute)
                stage_flags |= VK_SHADER_STAGE_COMPUTE_BIT;

            // Parse descriptor set layout
            std::vector<VkDescriptorSetLayoutBinding> bindings;
            std::vector<VkDescriptorBindingFlags> binding_flags;
            std::vector<VkDescriptorSetLayoutBindingFlagsCreateInfo> binding_flag_info;

            for (const auto& descriptor_layout : m_descriptorLayouts)
            {
                auto binding_point = descriptor_layout->binding();
                auto type = descriptor_layout->descriptorType();

                log::trace("\tWith descriptor {{ Type: {0}, Element size: {1} bytes, Array size: {4}, Offset: {2}, Binding point: {3} }}...",
                           type,
                           descriptor_layout->elementSize(),
                           0,
                           binding_point,
                           descriptor_layout->descriptors());

                VkDescriptorSetLayoutBinding binding = {
                    .binding = binding_point,
                    .descriptorCount = descriptor_layout->descriptors(),
                    .stageFlags = stage_flags,
                    .pImmutableSamplers = nullptr,
                };

                if (type == DescriptorType::InputAttachment && m_stages != ShaderStage::Fragment)
                    throw
                            base::BadArgumentException(std::format("Unable to bind input attachment at {0} to a descriptor set that is accessible from other stages, than the fragment shader.",
                                                                   binding_point));

                switch (type)
                {
                case DescriptorType::ConstantBuffer:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    break;
                case DescriptorType::ByteAddressBuffer:
                case DescriptorType::RWByteAddressBuffer:
                case DescriptorType::StructuredBuffer:
                case DescriptorType::RWStructuredBuffer:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                    break;
                case DescriptorType::RWTexture:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                    break;
                case DescriptorType::Texture:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                    break;
                case DescriptorType::RWBuffer:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
                    break;
                case DescriptorType::Buffer:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                    break;
                case DescriptorType::InputAttachment:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
                    break;
                case DescriptorType::Sampler:
                    binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                    break;
                }

                if (type != DescriptorType::Sampler || (type == DescriptorType::Sampler && descriptor_layout->staticSampler() == nullptr))
                    m_poolSizes.at(Impl::s_poolSizeMapping.at(binding.descriptorType)).descriptorCount++;
                else
                    binding.pImmutableSamplers = &descriptor_layout->staticSampler()->handle();

                // If the descriptor is an unbounded runtime array, disable validation warnings about partially bound elements.
                if (binding.descriptorCount == std::numeric_limits<unsigned>::max())
                {
                    // Unbounded arrays must be the only descriptor within a descriptor set.
                    if (m_descriptorLayouts.size() != 1)
                        throw
                                base::BadArgumentException(std::format("If an unbounded runtime array descriptor is used, it must be the only descriptor in the descriptor set, however the current descriptor set specifies {0} descriptors",
                                                                       m_descriptorLayouts.size()));

                    binding_flags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT |
                                            VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT | VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT_EXT);
                    m_usesDescriptorIndexing = true;
                    binding.descriptorCount = max_unbounded_array_size;
                } else
                {
                    binding_flags.push_back({});
                }

                bindings.push_back(binding);
            }

            log::trace("Creating descriptor set {0} layout with {1} bindings {{ Uniform: {2}, Storage: {3}, Images: {4}, Sampler: {5}, Input Attachments: {6}, Writable Images: {7}, Texel Buffers: {8} }}...",
                       m_space,
                       m_descriptorLayouts.size(),
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_SAMPLER)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)).descriptorCount);

            // Create the descriptor set layout
            const VkDescriptorSetLayoutBindingFlagsCreateInfo binding_flags_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
                .bindingCount = static_cast<unsigned>(binding_flags.size()),
                .pBindingFlags = binding_flags.data(),
            };

            VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext = &binding_flags_info,
                .bindingCount = static_cast<unsigned>(bindings.size()),
                .pBindings = bindings.data()
            };

            // Allow for descriptors to update after they have been bound. This also means, we have to manually take care of not to update a descriptor before it got used.
            if (m_usesDescriptorIndexing)
                descriptor_set_layout_info.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

            VkDescriptorSetLayout descriptor_set_layout = {};
            if (vkCreateDescriptorSetLayout(m_device.handle(), &descriptor_set_layout_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create descriptor set layout");

            // Create the initial descriptor pool
            addDescriptorPool();
            return descriptor_set_layout;
        }

        void addDescriptorPool()
        {
            // Don't add a pool for empty descriptor sets.
            if (m_poolSize == 0)
                return;

            log::trace("Allocating descriptor pool with {5} sets {{ Uniforms: {0}, Storages: {1}, Images: {2}, Samplers: {3}, Input attachments: {4} }}...",
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_SAMPLER)).descriptorCount,
                       m_poolSizes.at(s_poolSizeMapping.at(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)).descriptorCount,
                       m_poolSize);

            // Filter pool sizes, since descriptorCount must be greater than 0, according to the specs.
            std::vector<VkDescriptorPoolSize> pool_sizes;
            std::ranges::copy_if(m_poolSizes, std::back_inserter(pool_sizes), [](const VkDescriptorPoolSize& pool_size) { return pool_size.descriptorCount > 0; });

            VkDescriptorPoolCreateInfo pool_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .maxSets = m_poolSize,
                .poolSizeCount = static_cast<unsigned>(pool_sizes.size()),
                .pPoolSizes = pool_sizes.data(),
            };

            if (m_usesDescriptorIndexing)
                pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

            VkDescriptorPool descriptor_pool = {};
            if (vkCreateDescriptorPool(m_device.handle(), &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create descriptor pool");
            m_descriptorPools.push_back(descriptor_pool);
        }

        VkDescriptorSet tryAllocate(unsigned descriptors)
        {
            // If the descriptor set is empty, no descriptor can be allocated
            if (m_poolSize == 0)
                throw base::ArgumentOutOfRangeException("Failed to allocate descriptor set");

            // Configure the descriptor set allocation
            VkDescriptorSetVariableDescriptorCountAllocateInfo variable_count_info = {};
            VkDescriptorSetAllocateInfo descriptor_set_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .descriptorPool = m_descriptorPools.back(),
                .descriptorSetCount = 1,
                .pSetLayouts = &m_parent->handle()
            };

            if (m_usesDescriptorIndexing)
            {
                variable_count_info = {
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
                    .descriptorSetCount = 1,
                    .pDescriptorCounts = &descriptors
                };
                descriptor_set_info.pNext = &variable_count_info;
            }

            // Create the descriptor set
            VkDescriptorSet descriptor_set = {};
            const auto result = vkAllocateDescriptorSets(m_device.handle(), &descriptor_set_info, &descriptor_set);
            if (result == VK_SUCCESS)
            {
                m_descriptorSetSources.insert(std::make_pair(&descriptor_set, &m_descriptorPools.back()));
                return descriptor_set;
            }
            if (result != VK_ERROR_OUT_OF_POOL_MEMORY)
                throw base::NullPointerException("Failed to allocate descriptor set");

            addDescriptorPool();
            return tryAllocate(descriptors);
        }

    private:
        VulkanDescriptorSetLayout* m_parent;
        const VulkanDevice& m_device;
        ShaderStage m_stages;
        unsigned int m_space = 0, m_poolSize = 0;
        bool m_usesDescriptorIndexing = false;

        std::unordered_map<const VkDescriptorSet*, const VkDescriptorPool*> m_descriptorSetSources;
        std::vector<std::unique_ptr<VulkanDescriptorLayout>> m_descriptorLayouts;
        std::vector<VkDescriptorPool> m_descriptorPools;
        std::queue<VkDescriptorSet> m_freeDescriptorSets;
        std::mutex m_mutex;

        std::vector<VkDescriptorPoolSize> m_poolSizes {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 0},
            {VK_DESCRIPTOR_TYPE_SAMPLER, 0},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 0},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 0},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 0}
        };

        inline static const std::unordered_map<VkDescriptorType, unsigned int> s_poolSizeMapping {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2},
            {VK_DESCRIPTOR_TYPE_SAMPLER, 3},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 4},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 5},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 6},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 7}
        };
    };

    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VulkanDevice& device,
                                                         std::vector<std::unique_ptr<VulkanDescriptorLayout>>&& descriptor_layouts,
                                                         unsigned int space,
                                                         ShaderStage stages,
                                                         unsigned int pool_size,
                                                         unsigned int max_unbounded_array_size)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(this, device, std::move(descriptor_layouts), space, stages))
    {
        handle() = m_impl->initialize(pool_size, max_unbounded_array_size);
    }

    VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() noexcept
    {
        for (const auto& pool : m_impl->m_descriptorPools)
            vkDestroyDescriptorPool(m_impl->m_device.handle(), pool, nullptr);
        vkDestroyDescriptorSetLayout(m_impl->m_device.handle(), handle(), nullptr);
    }

    unsigned VulkanDescriptorSetLayout::poolSize() const noexcept
    {
        return m_impl->m_poolSize;
    }

    std::size_t VulkanDescriptorSetLayout::pools() const noexcept
    {
        return m_impl->m_descriptorPools.size();
    }

    const VulkanDevice& VulkanDescriptorSetLayout::device() const noexcept
    {
        return m_impl->m_device;
    }

    ShaderStage VulkanDescriptorSetLayout::shaderStage() const noexcept
    {
        return m_impl->m_stages;
    }

    std::vector<const VulkanDescriptorLayout*> VulkanDescriptorSetLayout::descriptors() const noexcept
    {
        std::vector<const VulkanDescriptorLayout*> descriptors;
        descriptors.reserve(m_impl->m_descriptorLayouts.size());
        std::ranges::transform(m_impl->m_descriptorLayouts, std::back_inserter(descriptors), [](const auto& descriptor) { return descriptor.get(); });
        return descriptors;
    }

    const VulkanDescriptorLayout* VulkanDescriptorSetLayout::descriptor(unsigned binding) const
    {
        const auto it = std::ranges::find_if(m_impl->m_descriptorLayouts, [binding](const auto& descriptor) { return descriptor->binding() == binding; });
        if (it != m_impl->m_descriptorLayouts.end())
            return it->get();
        throw base::ArgumentOutOfRangeException(std::format("No layout has been provided for binding {}", binding));
    }

    unsigned VulkanDescriptorSetLayout::space() const noexcept
    {
        return m_impl->m_space;
    }

    unsigned VulkanDescriptorSetLayout::uniforms() const noexcept
    {
        return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER].descriptorCount;
    }

    unsigned VulkanDescriptorSetLayout::storages() const noexcept
    {
        return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].descriptorCount;
    }

    unsigned VulkanDescriptorSetLayout::images() const noexcept
    {
        return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE].descriptorCount + m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE].descriptorCount;
    }

    unsigned VulkanDescriptorSetLayout::samplers() const noexcept
    {
        return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_SAMPLER].descriptorCount;
    }

    unsigned VulkanDescriptorSetLayout::buffers() const noexcept
    {
        return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER].descriptorCount + m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER].descriptorCount;
    }

    unsigned VulkanDescriptorSetLayout::staticSamplers() const noexcept
    {
        const std::size_t result = std::ranges::count_if(m_impl->m_descriptorLayouts,
                                                         [](const auto& layout)
                                                         {
                                                             return layout->descriptorType() == DescriptorType::Sampler && layout->staticSampler();
                                                         });
        return static_cast<unsigned>(result);
    }

    unsigned VulkanDescriptorSetLayout::inputAttachments() const noexcept
    {
        return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT].descriptorCount;
    }

    std::unique_ptr<VulkanDescriptorSet> VulkanDescriptorSetLayout::allocate(const std::vector<DescriptorBinding>& bindings) const
    {
        return allocate(0, bindings);
    }

    std::unique_ptr<VulkanDescriptorSet> VulkanDescriptorSetLayout::allocate(const unsigned descriptors, const std::vector<DescriptorBinding>& bindings) const
    {
        std::scoped_lock lock(m_impl->m_mutex);

        // If there is no descriptor set available, allocate a new one
        VulkanDescriptorSet* descriptor_set = nullptr;
        if (m_impl->m_usesDescriptorIndexing || m_impl->m_freeDescriptorSets.empty())
        {
            descriptor_set = new VulkanDescriptorSet(*this, m_impl->tryAllocate(descriptors));
        } else
        {
            descriptor_set = new VulkanDescriptorSet(*this, m_impl->m_freeDescriptorSets.front());
            m_impl->m_freeDescriptorSets.pop();
        }

        // Apply the bindings
        for (unsigned i = 0; const auto& binding : bindings)
        {
            std::visit(lib::overloaded {
                           [](std::monostate) {},
                           [&descriptor_set, &binding, &i](const ISampler& sampler)
                           {
                               descriptor_set->update(binding.binding.value_or(i), dynamic_cast<const IVulkanSampler&>(sampler), binding.firstDescriptor);
                           },
                           [&descriptor_set, &binding, &i](const IBuffer& buffer)
                           {
                               descriptor_set->update(binding.binding.value_or(i),
                                                      dynamic_cast<const IVulkanBuffer&>(buffer),
                                                      binding.firstElement,
                                                      binding.elements,
                                                      binding.firstDescriptor);
                           },
                           [&descriptor_set, &binding, &i](const IImage& image)
                           {
                               descriptor_set->update(binding.binding.value_or(i),
                                                      dynamic_cast<const IVulkanImage&>(image),
                                                      binding.firstDescriptor,
                                                      binding.firstLevel,
                                                      binding.levels,
                                                      binding.firstElement,
                                                      binding.elements);
                           }
                       },
                       binding.resource);
            i++;
        }
        return std::unique_ptr<VulkanDescriptorSet>(descriptor_set);
    }

    std::vector<std::unique_ptr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(const unsigned descriptor_sets,
                                                                                                  const std::vector<std::vector<DescriptorBinding>>& bindings) const
    {
        return allocateMultiple(descriptor_sets, 0, bindings);
    }

    std::vector<std::unique_ptr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(const unsigned descriptor_sets,
                                                                                                  const std::function<std::vector<DescriptorBinding>(unsigned)>&
                                                                                                  binding_factory) const
    {
        return allocateMultiple(descriptor_sets, 0, binding_factory);
    }

    std::vector<std::unique_ptr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(const unsigned descriptor_sets,
                                                                                                  const unsigned descriptors,
                                                                                                  const std::vector<std::vector<DescriptorBinding>>& bindings) const
    {
        std::vector<std::unique_ptr<VulkanDescriptorSet>> descriptor_sets_list;

        // Allocate the descriptor sets with the provided bindings
        for (const auto& binding : bindings)
            descriptor_sets_list.push_back(allocate(descriptors, binding));

        // Allocate the remaining descriptor sets with empty bindings
        for (std::size_t i = bindings.size(); i < descriptor_sets; i++)
            descriptor_sets_list.push_back(allocate(descriptors));

        return descriptor_sets_list;
    }

    std::vector<std::unique_ptr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(unsigned descriptor_sets,
                                                                                                  unsigned descriptors,
                                                                                                  const std::function<std::vector<DescriptorBinding>(unsigned)>&
                                                                                                  binding_factory) const
    {
        std::vector<std::unique_ptr<VulkanDescriptorSet>> descriptor_sets_list;

        // Allocate the descriptor sets with the provided bindings
        for (unsigned i = 0; i < descriptor_sets; i++)
            descriptor_sets_list.push_back(allocate(descriptors, binding_factory(i)));

        return descriptor_sets_list;
    }

    void VulkanDescriptorSetLayout::free(const VulkanDescriptorSet& descriptor_set) const noexcept
    {
        // If the descriptor set layout does not use descriptor indexing, we can just push the descriptor set back to the free list.
        if (!m_impl->m_usesDescriptorIndexing)
        {
            std::scoped_lock lock(m_impl->m_mutex);
            m_impl->m_freeDescriptorSets.push(descriptor_set.handle());
            return;
        }

        // If the descriptor set layout uses descriptor indexing, we have to manually free the descriptor set.
        const auto* handle = &descriptor_set.handle();
        if (m_impl->m_descriptorSetSources.contains(handle))
        {
            const auto& pool = m_impl->m_descriptorSetSources.at(handle);
            const auto result = vkFreeDescriptorSets(m_impl->m_device.handle(), *pool, 1, handle);

            if (result != VK_SUCCESS)
                log::error("Failed to free descriptor set {0}", result);

            m_impl->m_descriptorSetSources.erase(handle);

            // If the descriptor pool is empty, remove it from the list.
            const bool is_pool_empty = std::ranges::count_if(m_impl->m_descriptorSetSources, [&](const auto& source_mapping) { return source_mapping.second == pool; }) == 0;
            if (pool != &m_impl->m_descriptorPools.back() && is_pool_empty)
            {
                vkDestroyDescriptorPool(m_impl->m_device.handle(), *pool, nullptr);

                const auto match = std::ranges::find(m_impl->m_descriptorPools, *pool);
                if (match != m_impl->m_descriptorPools.end())
                    m_impl->m_descriptorPools.erase(match);
            }
        }
    }
}
