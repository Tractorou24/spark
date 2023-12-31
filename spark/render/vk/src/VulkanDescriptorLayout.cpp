#include "spark/render/vk/VulkanDescriptorLayout.h"

#include "spark/base/Exception.h"

namespace spark::render::vk
{
    struct VulkanDescriptorLayout::Impl
    {
        friend VulkanDescriptorLayout;

    public:
        explicit Impl(const DescriptorType type, const unsigned binding, const std::size_t element_size, const unsigned descriptors)
            : m_elementSize(element_size), m_binding(binding), m_descriptors(descriptors), m_descriptorType(type)
        {
            switch (type) // NOLINT(clang-diagnostic-switch-enum): Other cases are handled by the default statement
            {
            case DescriptorType::ConstantBuffer:
                m_bufferType = BufferType::Uniform;
                break;
            case DescriptorType::StructuredBuffer:
            case DescriptorType::RWStructuredBuffer:
            case DescriptorType::ByteAddressBuffer:
            case DescriptorType::RWByteAddressBuffer:
                m_bufferType = BufferType::Storage;
                break;
            case DescriptorType::Buffer:
            case DescriptorType::RWBuffer:
                m_bufferType = BufferType::Storage;
                break;
            default:
                m_bufferType = BufferType::Other;
                break;
            }
        }

        explicit Impl(std::unique_ptr<IVulkanSampler>&& static_sampler, const unsigned binding)
            : VulkanDescriptorLayout::Impl(DescriptorType::Sampler, binding, 0, 1)
        {
            if (!static_sampler)
                throw base::NullPointerException("Static sampler cannot be null");
            m_staticSampler = std::move(static_sampler);
        }

    private:
        std::size_t m_elementSize;
        unsigned m_binding, m_descriptors;
        DescriptorType m_descriptorType;
        BufferType m_bufferType;
        std::unique_ptr<IVulkanSampler> m_staticSampler;
    };

    VulkanDescriptorLayout::VulkanDescriptorLayout(DescriptorType type, unsigned binding, std::size_t element_size, unsigned descriptors)
        : m_impl(std::make_unique<Impl>(type, binding, element_size, descriptors)) {}

    VulkanDescriptorLayout::VulkanDescriptorLayout(std::unique_ptr<IVulkanSampler>&& static_sampler, unsigned binding)
        : m_impl(std::make_unique<Impl>(std::move(static_sampler), binding)) {}

    VulkanDescriptorLayout::~VulkanDescriptorLayout() noexcept = default;

    std::size_t VulkanDescriptorLayout::elementSize() const noexcept
    {
        return m_impl->m_elementSize;
    }

    unsigned VulkanDescriptorLayout::binding() const noexcept
    {
        return m_impl->m_binding;
    }

    BufferType VulkanDescriptorLayout::type() const noexcept
    {
        return m_impl->m_bufferType;
    }

    DescriptorType VulkanDescriptorLayout::descriptorType() const noexcept
    {
        return m_impl->m_descriptorType;
    }

    unsigned VulkanDescriptorLayout::descriptors() const noexcept
    {
        return m_impl->m_descriptors;
    }

    const IVulkanSampler* VulkanDescriptorLayout::staticSampler() const noexcept
    {
        return m_impl->m_staticSampler.get();
    }
}
