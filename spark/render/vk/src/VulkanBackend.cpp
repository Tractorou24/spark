#include "spark/render/vk/VulkanBackend.h"

#include "spark/base/Exception.h"
#include "spark/lib/String.h"

#include "vulkan/vulkan.h"

namespace spark::render::vk
{
    struct VulkanBackend::Impl
    {
        friend class VulkanBackend;

    public:
        explicit Impl(VulkanBackend* parent, std::span<std::string> extensions, std::span<std::string> validation_layers)
            : m_parent(parent)
        {
            m_extensions.assign(extensions.begin(), extensions.end());
            m_layers.assign(validation_layers.begin(), validation_layers.end());

            // Add mandatory extensions
            m_extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        }

        VkInstance initialize()
        {
            // Check if all extensions are available
            if (!VulkanBackend::ValidateInstanceExtensions(m_extensions))
                throw base::BadArgumentException("Some required Vulkan extensions are not supported by the system.");

            std::vector<const char*> required_extensions(m_extensions.size());
            std::ranges::transform(m_extensions, required_extensions.begin(), [](const auto& extension) { return extension.c_str(); });

            // Check if all extensions are available.
            if (!VulkanBackend::ValidateInstanceLayers(m_layers))
                throw base::BadArgumentException("Some required Vulkan layers are not supported by the system.");

            std::vector<const char*> enabled_layers(m_layers.size());
            std::ranges::transform(m_layers, enabled_layers.begin(), [](const auto& layer) { return layer.c_str(); });

            // Define Vulkan app
            // TODO: Make this configurable
            constexpr VkApplicationInfo app_info = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "Vulkan Application",
                .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
                .pEngineName = "SPARK",
                .engineVersion = VK_MAKE_VERSION(0, 1, 0),
                .apiVersion = VK_API_VERSION_1_3,
            };

            // Create Vulkan instance
            const VkInstanceCreateInfo instance_info = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &app_info,
                .enabledLayerCount = static_cast<unsigned>(enabled_layers.size()),
                .ppEnabledLayerNames = enabled_layers.data(),
                .enabledExtensionCount = static_cast<unsigned>(required_extensions.size()),
                .ppEnabledExtensionNames = required_extensions.data(),
            };

            VkInstance instance = VK_NULL_HANDLE;
            if (vkCreateInstance(&instance_info, nullptr, &instance) != VK_SUCCESS)
                throw base::NullPointerException("Failed to create Vulkan instance.");
            return instance;
        }

        void loadAdapters()
        {
            uint32_t adapters = 0;
            vkEnumeratePhysicalDevices(m_parent->handle(), &adapters, nullptr);

            std::vector<VkPhysicalDevice> handles(adapters);
            vkEnumeratePhysicalDevices(m_parent->handle(), &adapters, handles.data());

            std::ranges::transform(handles, std::back_inserter(m_adapters), [](const auto& handle) { return std::make_unique<VulkanGraphicsAdapter>(handle); });
        }

    private:
        VulkanBackend* m_parent;
        std::vector<std::unique_ptr<VulkanGraphicsAdapter>> m_adapters;
        std::unordered_map<std::string, std::unique_ptr<VulkanDevice>> m_devices;
        std::vector<std::string> m_extensions;
        std::vector<std::string> m_layers;
    };

    VulkanBackend::VulkanBackend(std::span<std::string> extensions, std::span<std::string> validation_layers)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(this, extensions, validation_layers))
    {
        handle() = m_impl->initialize();
        m_impl->loadAdapters();
    }

    VulkanBackend::~VulkanBackend()
    {
        m_impl.reset();
        vkDestroyInstance(handle(), nullptr);
    }

    std::unique_ptr<VulkanSurface> VulkanBackend::createSurface(const std::function<VkSurfaceKHR(const VkInstance&)>& predicate) const
    {
        auto surface = predicate(handle());
        return std::make_unique<VulkanSurface>(surface, handle());
    }

    bool VulkanBackend::ValidateInstanceLayers(std::span<std::string> validation_layers) noexcept
    {
        auto available_layers = VulkanBackend::InstanceValidationLayers();

        return std::ranges::all_of(validation_layers,
                                   [&available_layers](const auto& layer)
                                   {
                                       auto match = std::ranges::find_if(available_layers,
                                                                         [&layer](const auto& str)
                                                                         {
                                                                             return std::ranges::equal(str,
                                                                                                       layer,
                                                                                                       [](const char a, const char b)
                                                                                                       {
                                                                                                           return std::tolower(a) == std::tolower(b);
                                                                                                       });
                                                                         });

                                       if (match == available_layers.end())
                                           log::error("Validation layer {0} is not supported by this instance.", layer);

                                       return match != available_layers.end();
                                   });
    }

    std::vector<std::string> VulkanBackend::InstanceValidationLayers() noexcept
    {
        unsigned layers = 0;
        vkEnumerateInstanceLayerProperties(&layers, nullptr);

        std::vector<VkLayerProperties> properties(layers);
        vkEnumerateInstanceLayerProperties(&layers, properties.data());

        std::vector<std::string> result;
        std::ranges::transform(properties, std::back_inserter(result), [](const auto& layer) { return std::string(layer.layerName); });
        return result;
    }

    std::span<std::string> VulkanBackend::enabledValidationLayers() const noexcept
    {
        return m_impl->m_layers;
    }

    bool VulkanBackend::ValidateInstanceExtensions(std::span<std::string> extensions) noexcept
    {
        auto available_extensions = VulkanBackend::AvailableInstanceExtensions();

        return std::ranges::all_of(extensions,
                                   [&available_extensions](const auto& extension)
                                   {
                                       auto match = std::ranges::find_if(available_extensions,
                                                                         [&extension](const auto& str)
                                                                         {
                                                                             return std::ranges::equal(str,
                                                                                                       extension,
                                                                                                       [](const char a, const char b)
                                                                                                       {
                                                                                                           return std::tolower(a) == std::tolower(b);
                                                                                                       });
                                                                         });

                                       if (match == available_extensions.end())
                                           log::error("Extension {0} is not supported by this instance.", extension);

                                       return match != available_extensions.end();
                                   });
    }

    std::vector<std::string> VulkanBackend::AvailableInstanceExtensions() noexcept
    {
        unsigned extensions = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensions, nullptr);

        std::vector<VkExtensionProperties> properties(extensions);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensions, properties.data());

        std::vector<std::string> result;
        std::ranges::transform(properties, std::back_inserter(result), [](const auto& property) { return std::string(property.extensionName); });
        return result;
    }

    std::vector<const VulkanGraphicsAdapter*> VulkanBackend::adapters() const noexcept
    {
        std::vector<const VulkanGraphicsAdapter*> result;
        result.reserve(m_impl->m_adapters.size());
        std::ranges::transform(m_impl->m_adapters, std::back_inserter(result), [](const auto& adapter) { return adapter.get(); });
        return result;
    }

    const VulkanGraphicsAdapter* VulkanBackend::findAdapter(const std::optional<lib::Uuid>& id) const
    {
        const auto match = std::ranges::find_if(m_impl->m_adapters, [&id](const auto& adapter) { return !id.has_value() || adapter->uuid() == id; });
        if (match == m_impl->m_adapters.end())
            return nullptr;
        return match->get();
    }

    VulkanDevice* VulkanBackend::device(const std::string& name) noexcept
    {
        if (!m_impl->m_devices.contains(name))
            return nullptr;
        return m_impl->m_devices[name].get();
    }

    const VulkanDevice* VulkanBackend::device(const std::string& name) const noexcept
    {
        if (!m_impl->m_devices.contains(name))
            return nullptr;
        return m_impl->m_devices[name].get();
    }

    void VulkanBackend::registerDevice(std::string name, std::unique_ptr<VulkanDevice>&& device)
    {
        if (m_impl->m_devices.contains(name))
            throw base::BadArgumentException(std::format("The backend already contains a device with the name '{0}'", name));
        m_impl->m_devices.emplace(std::move(name), std::move(device));
    }

    void VulkanBackend::releaseDevice(const std::string& name)
    {
        if (!m_impl->m_devices.contains(name))
            return;

        m_impl->m_devices[name].get()->wait();
        m_impl->m_devices.erase(name);
    }
}
