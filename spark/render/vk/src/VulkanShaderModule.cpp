#include"spark/render/vk/VulkanShaderModule.h"
#include"spark/render/vk/VulkanDevice.h"

#include "spark/base/Exception.h"

#include "vulkan/vulkan.h"

#include <fstream>

namespace
{
    /**
     * \brief Reads a stream in into a string (binary mode)
     * \param stream The stream to read 
     * \return A \ref std::string containing the contents of the stream
     */
    std::string read_file(std::istream& stream)
    {
        return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
    }

    /**
     * \brief Reads a file in into a string (binary mode)
     * \param path The path to the file to read
     * \return A \ref std::string containing the contents of the file
     */
    std::string read_file(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
            throw spark::base::CouldNotOpenFileException(std::format("Failed to open shader file: {}", path.generic_string()));

        return read_file(file);
    }
}

namespace spark::render::vk
{
    struct VulkanShaderModule::Impl
    {
        friend VulkanShaderModule;

    public:
        explicit Impl(const VulkanDevice& device, const ShaderStage type, const std::filesystem::path& file_path, std::string entry_point)
            : m_stage(type), m_fileName(file_path.filename().string()), m_entryPoint(std::move(entry_point)), m_device(device) {}

        VkShaderModule init(const std::string& byte_code)
        {
            const VkShaderModuleCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .codeSize = byte_code.size(),
                .pCode = reinterpret_cast<const uint32_t*>(byte_code.c_str())
            };

            VkShaderModule shader = {};
            if (vkCreateShaderModule(m_device.handle(), &create_info, nullptr, &shader) != VK_SUCCESS)
                throw base::CouldNotCreateShaderException(std::format("Failed to create shader module from file: {}", m_fileName));

            m_byteCode = byte_code;
            return shader;
        }

    private:
        ShaderStage m_stage = ShaderStage::Other;
        std::string m_fileName, m_entryPoint, m_byteCode;
        const VulkanDevice& m_device;
    };

    VulkanShaderModule::VulkanShaderModule(const VulkanDevice& device, ShaderStage type, const std::filesystem::path& file_name, const std::string& entry_point)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(device, type, file_name, entry_point))
    {
        handle() = m_impl->init(read_file(file_name));
    }

    VulkanShaderModule::VulkanShaderModule(const VulkanDevice& device, ShaderStage type, std::istream& stream, const std::string& name, const std::string& entry_point)
        : Resource(VK_NULL_HANDLE), m_impl(std::make_unique<Impl>(device, type, name, entry_point))
    {
        handle() = m_impl->init(read_file(stream));
    }

    VulkanShaderModule::~VulkanShaderModule()
    {
        vkDestroyShaderModule(m_impl->m_device.handle(), this->handle(), nullptr);
    }

    const std::string& VulkanShaderModule::byteCode() const noexcept
    {
        return m_impl->m_byteCode;
    }

    ShaderStage VulkanShaderModule::stage() const noexcept
    {
        return m_impl->m_stage;
    }

    const std::string& VulkanShaderModule::fileName() const noexcept
    {
        return m_impl->m_fileName;
    }

    const std::string& VulkanShaderModule::entryPoint() const noexcept
    {
        return m_impl->m_entryPoint;
    }
}
