#pragma once

#include "spark/render/Resource.h"
#include "spark/render/Shader.h"
#include "spark/render/ShaderStages.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"

#include <filesystem>
#include <string>

SPARK_FWD_DECLARE_VK_HANDLE(VkShaderModule)

namespace spark::render::vk
{
    class VulkanDevice;

    /**
     * \brief Vulkan implementation of a \ref IShaderModule.
     */
    class SPARK_RENDER_VK_EXPORT VulkanShaderModule final : public IShaderModule, public Resource<VkShaderModule>
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanShaderModule from a file.
         * \param device The parent \ref VulkanDevice this shader has been created from.
         * \param type The \ref ShaderStage of this shader.
         * \param file_name The path to the shader file.
         * \param entry_point The name of the entry point of the shader.
         *
         * \throws base::CouldNotOpenFileException If the file could not be opened.
         * \throws base::CouldNotCreateShaderException If the shader could not be created.
         */
        explicit VulkanShaderModule(const VulkanDevice& device, ShaderStage type, const std::filesystem::path& file_name, const std::string& entry_point = "main");

        /**
         * \brief Initializes a new \ref VulkanShaderModule from a \ref std::istream.
         * \param device The parent \ref VulkanDevice this shader has been created from.
         * \param type The \ref ShaderStage of this shader.
         * \param stream The \ref std::istream containing the shader byte code.
         * \param name The file name of the shader.
         * \param entry_point The name of the entry point of the shader.
         *
         * \throws base::CouldNotCreateShaderException If the shader could not be created.
         */
        explicit VulkanShaderModule(const VulkanDevice& device, ShaderStage type, std::istream& stream, const std::string& name, const std::string& entry_point = "main");
        ~VulkanShaderModule() override;

        VulkanShaderModule(const VulkanShaderModule& other) = delete;
        VulkanShaderModule(VulkanShaderModule&& other) noexcept = delete;
        VulkanShaderModule& operator=(const VulkanShaderModule& other) = delete;
        VulkanShaderModule& operator=(VulkanShaderModule&& other) noexcept = delete;

        /**
         * \brief Gets the shader byte code.
         * \return A \ref std::string containing the shader byte code.
         */
        [[nodiscard]] const std::string& byteCode() const noexcept;

        /// \copydoc IShaderModule::stage()
        [[nodiscard]] ShaderStage stage() const noexcept override;

        //// \copydoc IShaderModule::fileName()
        [[nodiscard]] const std::string& fileName() const noexcept override;

        /// \copydoc IShaderModule::entryPoint()
        [[nodiscard]] const std::string& entryPoint() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
