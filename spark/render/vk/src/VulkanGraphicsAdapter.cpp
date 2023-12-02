#include "spark/render/vk/VulkanGraphicsAdapter.h"

#include "spark/log/Logger.h"

#include "vulkan/vulkan.h"

#include <algorithm>
#include <numeric>
#include <ranges>

namespace spark::render::vk
{
    struct VulkanGraphicsAdapter::Impl
    {
        friend class VulkanGraphicsAdapter;

    public:
        explicit Impl(VulkanGraphicsAdapter* parent)
            : m_parent(parent)
        {
            m_limits = properties().limits;
        }

        [[nodiscard]] VkPhysicalDeviceProperties properties() const noexcept
        {
            VkPhysicalDeviceProperties properties = {};
            vkGetPhysicalDeviceProperties(m_parent->handle(), &properties);
            return properties;
        }

        [[nodiscard]] VkPhysicalDeviceIDProperties idProperties() const noexcept
        {
            VkPhysicalDeviceIDProperties id_properties = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES
            };

            VkPhysicalDeviceProperties2 id_properties2 = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
                .pNext = &id_properties
            };

            vkGetPhysicalDeviceProperties2(m_parent->handle(), &id_properties2);
            return id_properties;
        }

    private:
        VulkanGraphicsAdapter* m_parent;
        VkPhysicalDeviceLimits m_limits = {};
    };

    VulkanGraphicsAdapter::VulkanGraphicsAdapter(const VkPhysicalDevice adapter)
        : Resource(adapter), m_impl(std::make_unique<Impl>(this)) {}

    VulkanGraphicsAdapter::~VulkanGraphicsAdapter() = default;

    std::string VulkanGraphicsAdapter::name() const noexcept
    {
        return m_impl->properties().deviceName;
    }

    lib::Uuid VulkanGraphicsAdapter::uuid() const noexcept
    {
        return lib::Uuid(m_impl->idProperties().deviceUUID);
    }

    unsigned VulkanGraphicsAdapter::vendorId() const noexcept
    {
        return m_impl->properties().vendorID;
    }

    unsigned VulkanGraphicsAdapter::deviceId() const noexcept
    {
        return m_impl->properties().deviceID;
    }

    GraphicsAdapterType VulkanGraphicsAdapter::type() const noexcept
    {
        switch (m_impl->properties().deviceType) // NOLINT(clang-diagnostic-switch-enum) Only handle documented values.
        {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return GraphicsAdapterType::CPU;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return GraphicsAdapterType::GPU;
        default:
            return GraphicsAdapterType::Other;
        }
    }

    unsigned VulkanGraphicsAdapter::driverVersion() const noexcept
    {
        return m_impl->properties().driverVersion;
    }

    unsigned VulkanGraphicsAdapter::apiVersion() const noexcept
    {
        return m_impl->properties().apiVersion;
    }

    unsigned long long VulkanGraphicsAdapter::dedicatedVideoMemory() const noexcept
    {
        VkPhysicalDeviceMemoryProperties memory_properties = {};
        vkGetPhysicalDeviceMemoryProperties(handle(), &memory_properties);

        std::vector heaps(memory_properties.memoryHeaps, memory_properties.memoryHeaps + memory_properties.memoryHeapCount);
        return std::accumulate(heaps.begin(),
                               heaps.end(),
                               0ull,
                               [](const unsigned long long total, const VkMemoryHeap& heap)
                               {
                                   return total + (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ? heap.size : 0);
                               });
    }

    VkPhysicalDeviceLimits VulkanGraphicsAdapter::limits() const noexcept
    {
        return m_impl->m_limits;
    }

    std::vector<std::string> VulkanGraphicsAdapter::deviceExtensions() const noexcept
    {
        // Get extension count
        unsigned int extension_count = 0;
        vkEnumerateDeviceExtensionProperties(handle(), nullptr, &extension_count, nullptr);

        // Get extension properties
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(handle(), nullptr, &extension_count, extensions.data());

        // Transform the properties into a vector of string (extension names)
        std::vector<std::string> extension_names(extension_count);
        std::ranges::transform(extensions, extension_names.begin(), [](const VkExtensionProperties& extension) -> std::string { return extension.extensionName; });
        return extension_names;
    }

    bool VulkanGraphicsAdapter::validateDeviceExtensions(const std::vector<std::string>& extensions) const noexcept
    {
        const auto available_extensions = deviceExtensions();
        return std::ranges::all_of(extensions,
                                   [&](const auto& extension)
                                   {
                                       // Find the extension in the available extensions (case-insensitive)
                                       const bool found = std::ranges::find_if(available_extensions,
                                                                               [&extension](const auto& str)
                                                                               {
                                                                                   return std::ranges::equal(str,
                                                                                                             extension,
                                                                                                             [&](const char a, const char b)
                                                                                                             {
                                                                                                                 return std::tolower(a) == std::tolower(b);
                                                                                                             });
                                                                               }) != available_extensions.end();

                                       if (!found)
                                           spark::log::error("Requested extension {} is not supported by adapter {}.", extension, name());
                                       return found;
                                   });
    }

    std::vector<std::string> VulkanGraphicsAdapter::deviceValidationLayers() const noexcept
    {
        // Get layer count
        unsigned int layer_count = 0;
        vkEnumerateDeviceLayerProperties(handle(), &layer_count, nullptr);

        // Get layer properties
        std::vector<VkLayerProperties> layers(layer_count);
        vkEnumerateDeviceLayerProperties(handle(), &layer_count, layers.data());

        // Transform the properties into a vector of string (layer names)
        std::vector<std::string> layer_names(layer_count);
        std::ranges::transform(layers, layer_names.begin(), [](const VkLayerProperties& layer) -> std::string { return layer.layerName; });
        return layer_names;
    }

    bool VulkanGraphicsAdapter::validateDeviceValidationLayers(const std::vector<std::string>& layers) const noexcept
    {
        const auto available_layers = deviceValidationLayers();
        return std::ranges::all_of(layers,
                                   [&](const auto& layer)
                                   {
                                       // Find the layer in the available layers (case-insensitive)
                                       const bool found = std::ranges::find_if(available_layers,
                                                                               [&layer](const auto& str)
                                                                               {
                                                                                   return std::ranges::equal(str,
                                                                                                             layer,
                                                                                                             [&](const char a, const char b)
                                                                                                             {
                                                                                                                 return std::tolower(a) == std::tolower(b);
                                                                                                             });
                                                                               }) != available_layers.end();

                                       if (!found)
                                           spark::log::error("Requested validation layer {} is not supported by adapter {}.", layer, name());
                                       return found;
                                   });
    }
}
