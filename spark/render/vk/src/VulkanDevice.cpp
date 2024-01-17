#include "spark/render/vk/VulkanDevice.h"
#include "spark/render/CommandQueue.h"
#include "spark/render/vk/VulkanFactory.h"
#include "spark/render/vk/VulkanGraphicsAdapter.h"
#include "spark/render/vk/VulkanQueue.h"
#include "spark/render/vk/VulkanSurface.h"
#include "spark/render/DeviceState.h"
#include "spark/render/vk/VulkanSwapChain.h"

#include "spark/base/Exception.h"
#include "spark/lib/String.h"
#include "spark/log/Logger.h"
#include "spark/math/Vector2.h"

#include "vulkan/vulkan.h"

#include <ranges>
#include <span>
#include <vector>

namespace spark::render::vk
{
    class QueueFamily
    {
    public:
        explicit QueueFamily(const unsigned id, const unsigned queue_count, const QueueType type) :
            m_id(id), m_queueCount(queue_count), m_type(type) {}

        ~QueueFamily() noexcept
        {
            m_queues.clear();
        }

        QueueFamily(const QueueFamily& other) = delete;

        QueueFamily(QueueFamily&& other) noexcept
        {
            if (this == &other)
                return;

            m_queues = std::move(other.m_queues);
            m_id = other.m_id;
            m_queueCount = other.m_queueCount;
            m_type = other.m_type;
        }

        QueueFamily& operator=(const QueueFamily& other) = delete;

        QueueFamily& operator=(QueueFamily&& other) noexcept
        {
            if (this == &other)
                return *this;

            m_queues = std::move(other.m_queues);
            m_type = other.m_type;
            m_id = other.m_id;
            m_queueCount = other.m_queueCount;
            return *this;
        }

        [[nodiscard]] QueueType type() const noexcept { return m_type; }
        [[nodiscard]] unsigned total() const noexcept { return m_queueCount; }
        [[nodiscard]] unsigned active() const noexcept { return static_cast<unsigned>(m_queues.size()); }
        [[nodiscard]] unsigned id() const noexcept { return m_id; }
        [[nodiscard]] const std::vector<std::unique_ptr<VulkanQueue>>& queues() const noexcept { return m_queues; }

    public:
        VulkanQueue* createQueue(const VulkanDevice& device, QueuePriority priority)
        {
            if (active() >= total())
            {
                log::error("Unable to create another queue for family {0}, since all {1} queues are already created.", m_id, m_queueCount);
                return nullptr;
            }

            auto queue = std::make_unique<VulkanQueue>(device, m_type, priority, m_id, active());
            m_queues.push_back(std::move(queue));
            return m_queues.back().get();
        }

    private:
        std::vector<std::unique_ptr<VulkanQueue>> m_queues;
        unsigned m_id, m_queueCount;
        QueueType m_type;
    };

    struct VulkanDevice::Impl
    {
        friend class VulkanDevice;

    public:
        explicit Impl(VulkanDevice* parent, const VulkanGraphicsAdapter& adapter, std::unique_ptr<VulkanSurface>&& surface, std::span<std::string> extensions)
            : m_parent(parent), m_adapter(adapter), m_surface(std::move(surface))
        {
            if (!m_surface)
                throw base::NullPointerException("The surface must be initialized.");

            // Add the requested extensions
            m_extensions.assign(extensions.begin(), extensions.end());

            // Add the required extensions
            m_extensions.emplace_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
            m_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

            // Load the queue families
            uint32_t queue_families = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(m_adapter.handle(), &queue_families, nullptr);

            std::vector<VkQueueFamilyProperties> family_properties(queue_families);
            vkGetPhysicalDeviceQueueFamilyProperties(m_adapter.handle(), &queue_families, family_properties.data());

            std::ranges::transform(family_properties,
                                   std::back_inserter(m_families),
                                   [i = 0](const VkQueueFamilyProperties& family_property) mutable
                                   {
                                       auto type = QueueType::None;

                                       if (family_property.queueFlags & VK_QUEUE_COMPUTE_BIT)
                                           type |= QueueType::Compute;
                                       if (family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                                           type |= QueueType::Graphics;
                                       if (family_property.queueFlags & VK_QUEUE_TRANSFER_BIT)
                                           type |= QueueType::Transfer;

                                       return QueueFamily(i++, family_property.queueCount, type);
                                   });
        }

        ~Impl()
        {
            m_deviceState.clear();
            m_families.clear();
            m_swapChain.reset();
            m_surface.reset();
        }

        VkDevice initialize()
        {
            if (!m_adapter.validateDeviceExtensions(m_extensions))
                throw base::BadArgumentException("Some required device extensions are not supported by the system.");

            std::vector<const char*> required_extensions;
            std::ranges::transform(m_extensions, std::back_inserter(required_extensions), [](const auto& extension) { return extension.c_str(); });

            // Create queues
            m_graphicsQueue = this->createQueue(QueueType::Graphics, QueuePriority::Realtime, std::as_const(*m_surface).handle());
            m_transferQueue = this->createQueue(QueueType::Transfer, QueuePriority::Normal);
            m_bufferQueue = this->createQueue(QueueType::Transfer, QueuePriority::Normal);
            m_computeQueue = this->createQueue(QueueType::Compute, QueuePriority::Normal);

            if (!m_graphicsQueue)
                throw base::NullPointerException("Unable to find a fitting command queue to present the specified surface.");

            if (!m_transferQueue)
            {
                log::warning("Unable to find a fitting command queue for transfer operations. Using the graphics queue instead.");
                m_transferQueue = m_graphicsQueue;
            }

            if (!m_bufferQueue)
            {
                log::warning("Unable to find a fitting command queue for buffer operations. Using the graphics queue instead.");
                m_bufferQueue = m_graphicsQueue;
            }

            if (!m_computeQueue)
            {
                log::warning("Unable to find a fitting command queue for compute operations. Using the graphics queue instead.");
                m_computeQueue = m_graphicsQueue;
            }

            // Define used queue families
            std::vector<std::vector<float>> queue_priorities;
            std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
            auto filtered_queues = m_families | std::views::filter([](const auto& family) { return family.active() > 0; });
            std::ranges::transform(filtered_queues,
                                   std::back_inserter(queue_create_infos),
                                   [&](const auto& family)
                                   {
                                       std::vector<float> priorities(family.active());
                                       std::ranges::transform(family.queues(),
                                                              std::back_inserter(priorities),
                                                              [](const auto& queue) { return static_cast<float>(queue->priority()) / 100.f; });
                                       queue_priorities.push_back(std::move(priorities));

                                       VkDeviceQueueCreateInfo queue_info = {
                                           .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                           .queueFamilyIndex = family.id(),
                                           .queueCount = family.active(),
                                           .pQueuePriorities = queue_priorities.back().data(),
                                       };
                                       return queue_info;
                                   });

            // Allow geometry and tessellation shader stages
            VkPhysicalDeviceFeatures device_features = {
                .geometryShader = true,
                .tessellationShader = true,
                .samplerAnisotropy = true
            };

            VkPhysicalDeviceVulkan13Features device_features_1_3 = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
                .synchronization2 = true
            };

            VkPhysicalDeviceVulkan12Features device_features_1_2 = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
                .pNext = &device_features_1_3,
                .descriptorIndexing = true,
                .shaderInputAttachmentArrayDynamicIndexing = true,
                .shaderUniformTexelBufferArrayDynamicIndexing = true,
                .shaderStorageTexelBufferArrayDynamicIndexing = true,
                .shaderUniformBufferArrayNonUniformIndexing = true,
                .shaderSampledImageArrayNonUniformIndexing = true,
                .shaderStorageBufferArrayNonUniformIndexing = true,
                .shaderStorageImageArrayNonUniformIndexing = true,
                .shaderInputAttachmentArrayNonUniformIndexing = true,
                .shaderUniformTexelBufferArrayNonUniformIndexing = true,
                .shaderStorageTexelBufferArrayNonUniformIndexing = true,
                .descriptorBindingSampledImageUpdateAfterBind = true,
                .descriptorBindingStorageImageUpdateAfterBind = true,
                .descriptorBindingStorageBufferUpdateAfterBind = true,
                .descriptorBindingUniformTexelBufferUpdateAfterBind = true,
                .descriptorBindingStorageTexelBufferUpdateAfterBind = true,
                .descriptorBindingUpdateUnusedWhilePending = true,
                .descriptorBindingPartiallyBound = true,
                .descriptorBindingVariableDescriptorCount = true,
                .runtimeDescriptorArray = true,
                .hostQueryReset = true,
                .timelineSemaphore = true
            };

            // Enable extended dynamic state
            const VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state_features = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
                .pNext = &device_features_1_2,
                .extendedDynamicState = true
            };

            // Create the device
            const VkDeviceCreateInfo device_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext = &extended_dynamic_state_features,
                .queueCreateInfoCount = static_cast<unsigned>(queue_create_infos.size()),
                .pQueueCreateInfos = queue_create_infos.data(),
                .enabledExtensionCount = static_cast<unsigned>(required_extensions.size()),
                .ppEnabledExtensionNames = required_extensions.data(),
                .pEnabledFeatures = &device_features,
            };

            VkDevice device = VK_NULL_HANDLE;
            if (vkCreateDevice(m_adapter.handle(), &device_info, nullptr, &device) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create Vulkan device.");
            return device;
        }

        [[nodiscard]] static bool IsFlagSet(auto val, auto flag)
        {
            return (static_cast<unsigned>(val) & static_cast<unsigned>(flag)) == static_cast<unsigned>(flag);
        }

        void createSwapChain(Format format, const math::Vector2<unsigned>& frame_buffer_size, unsigned frame_buffers)
        {
            m_swapChain = std::make_unique<VulkanSwapChain>(*m_parent, format, frame_buffer_size, frame_buffers);
        }

        void createQueues() const
        {
            m_graphicsQueue->bind();
            m_transferQueue->bind();
            m_bufferQueue->bind();
            m_computeQueue->bind();
        }

        VulkanQueue* createQueue(QueueType type, const QueuePriority priority)
        {
            const auto match = type == QueueType::Transfer
                                   ? std::ranges::find_if(m_families, [](const auto& family) { return family.type() == QueueType::Transfer; })
                                   : std::ranges::find_if(m_families, [&type](const auto& family) { return IsFlagSet(family.type(), type); });

            return match == m_families.end() ? nullptr : match->createQueue(*m_parent, priority);
        }

        VulkanQueue* createQueue(QueueType type, const QueuePriority priority, const VkSurfaceKHR& surface)
        {
            const auto match = std::ranges::find_if(m_families,
                                                    [&](const auto& family)
                                                    {
                                                        if (!IsFlagSet(family.type(), type))
                                                            return false;

                                                        VkBool32 can_present = VK_FALSE;
                                                        vkGetPhysicalDeviceSurfaceSupportKHR(m_adapter.handle(), family.id(), surface, &can_present);

                                                        return static_cast<bool>(can_present);
                                                    });

            if (match != m_families.end())
                return match->createQueue(*m_parent, priority);
            return nullptr;
        }

    private:
        VulkanDevice* m_parent;
        const VulkanGraphicsAdapter& m_adapter;
        DeviceState m_deviceState;
        std::vector<std::string> m_extensions;

        std::vector<QueueFamily> m_families;
        std::unique_ptr<VulkanSurface> m_surface;
        std::unique_ptr<VulkanSwapChain> m_swapChain;
        std::unique_ptr<VulkanFactory> m_factory;
        VulkanQueue* m_graphicsQueue;
        VulkanQueue* m_transferQueue;
        VulkanQueue* m_bufferQueue;
        VulkanQueue* m_computeQueue;
    };

    VulkanDevice::VulkanDevice(const VulkanGraphicsAdapter& adapter,
                               std::unique_ptr<VulkanSurface>&& surface,
                               const std::span<std::string> extensions)
        : VulkanDevice(adapter, std::move(surface), Format::B8G8R8A8_SRGB, {1280, 720}, 3, extensions) {}

    VulkanDevice::VulkanDevice(const VulkanGraphicsAdapter& adapter,
                               std::unique_ptr<VulkanSurface>&& surface,
                               const Format format,
                               const math::Vector2<unsigned>& frame_buffer_size,
                               const unsigned frame_buffers,
                               std::span<std::string> extensions)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(this, adapter, std::move(surface), extensions))
    {
        auto device_extensions = adapter.deviceExtensions();
        auto device_validation_layers = adapter.deviceValidationLayers();

        log::debug("Creating Vulkan device {{ Surface: {0}, Adapter: {1}, Extensions: {2} }}...",
                   reinterpret_cast<const void*>(m_impl->m_surface.get()),
                   adapter.deviceId(),
                   lib::join(enabledExtensions(), ", "));
        log::debug("--------------------------------------------------------------------------");
        log::debug("Vendor: {0:#0x}", adapter.vendorId());
        log::debug("Driver Version: {0:#0x}", adapter.driverVersion());
        log::debug("API Version: {0:#0x}", adapter.apiVersion());
        log::debug("Dedicated Memory: {0} Bytes", adapter.dedicatedVideoMemory());
        log::debug("--------------------------------------------------------------------------");
        log::debug("Available extensions: {0}", lib::join(device_extensions, ", "));
        log::debug("Validation layers: {0}", lib::join(device_validation_layers, ", "));
        log::debug("--------------------------------------------------------------------------");

        if (!extensions.empty())
            log::info("Enabled validation layers: {0}", lib::join(extensions, ", "));

        handle() = m_impl->initialize();
        m_impl->createQueues();
        m_impl->m_factory = std::make_unique<VulkanFactory>(*this);
        m_impl->createSwapChain(format, frame_buffer_size, frame_buffers);
    }

    VulkanDevice::~VulkanDevice()
    {
        m_impl.reset();
        vkDestroyDevice(handle(), nullptr);
    }

    std::span<std::string> VulkanDevice::enabledExtensions() const noexcept
    {
        return m_impl->m_extensions;
    }

    MultiSamplingLevel VulkanDevice::maximumMultiSamplingLevel(const Format format) const noexcept
    {
        const auto limits = m_impl->m_adapter.limits();
        VkSampleCountFlags sample_counts = limits.framebufferColorSampleCounts;

        if (helpers::has_depth(format) && helpers::has_stencil(format))
            sample_counts = limits.framebufferDepthSampleCounts & limits.framebufferStencilSampleCounts;
        else if (helpers::has_depth(format))
            sample_counts = limits.framebufferDepthSampleCounts;
        else if (helpers::has_stencil(format))
            sample_counts = limits.framebufferStencilSampleCounts;

        if (sample_counts & VK_SAMPLE_COUNT_64_BIT)
            return MultiSamplingLevel::X64;
        if (sample_counts & VK_SAMPLE_COUNT_32_BIT)
            return MultiSamplingLevel::X32;
        if (sample_counts & VK_SAMPLE_COUNT_16_BIT)
            return MultiSamplingLevel::X16;
        if (sample_counts & VK_SAMPLE_COUNT_8_BIT)
            return MultiSamplingLevel::X8;
        if (sample_counts & VK_SAMPLE_COUNT_4_BIT)
            return MultiSamplingLevel::X4;
        if (sample_counts & VK_SAMPLE_COUNT_2_BIT)
            return MultiSamplingLevel::X2;
        return MultiSamplingLevel::X1;
    }

    double VulkanDevice::ticksPerMillisecond() const noexcept
    {
        return 1000000.0 / static_cast<double>(graphicsAdapter().limits().timestampPeriod);
    }

    void VulkanDevice::wait() const
    {
        if (vkDeviceWaitIdle(handle()) != VK_SUCCESS)
            throw base::UnknownException("Failed to wait for device to become idle.");
    }

    DeviceState& VulkanDevice::state() noexcept
    {
        return m_impl->m_deviceState;
    }

    const VulkanSurface& VulkanDevice::surface() const noexcept
    {
        return *m_impl->m_surface;
    }

    const VulkanGraphicsAdapter& VulkanDevice::graphicsAdapter() const noexcept
    {
        return m_impl->m_adapter;
    }

    const VulkanFactory& VulkanDevice::factory() const noexcept
    {
        return *m_impl->m_factory;
    }

    const VulkanSwapChain& VulkanDevice::swapChain() const noexcept
    {
        return *m_impl->m_swapChain;
    }

    VulkanSwapChain& VulkanDevice::swapChain() noexcept
    {
        return *m_impl->m_swapChain;
    }

    const VulkanQueue& VulkanDevice::graphicsQueue() const noexcept
    {
        return *m_impl->m_graphicsQueue;
    }

    const VulkanQueue& VulkanDevice::transferQueue() const noexcept
    {
        return *m_impl->m_transferQueue;
    }

    const VulkanQueue& VulkanDevice::bufferQueue() const noexcept
    {
        return *m_impl->m_bufferQueue;
    }

    const VulkanQueue& VulkanDevice::computeQueue() const noexcept
    {
        return *m_impl->m_computeQueue;
    }
}
