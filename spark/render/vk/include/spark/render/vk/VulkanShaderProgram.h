#pragma once

#include "spark/render/Shader.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanShaderModule.h"

namespace spark::render::vk
{
    /**
     * \brief Vulkan implementation of a \ref ShaderProgram.
     */
    class SPARK_RENDER_VK_EXPORT VulkanShaderProgram final : public ShaderProgram<VulkanShaderModule>
    {
    public:
        /**
         * \brief Initializes a new Vulkan shader program with the given modules.
         * \param device The parent device of this shader program.
         * \param modules The modules used by this shader program.
         */
        explicit VulkanShaderProgram(const VulkanDevice& device, std::vector<std::unique_ptr<VulkanShaderModule>>&& modules);
        ~VulkanShaderProgram() override;

        VulkanShaderProgram(const VulkanShaderProgram& other) = delete;
        VulkanShaderProgram(VulkanShaderProgram&& other) noexcept = delete;
        VulkanShaderProgram& operator=(const VulkanShaderProgram& other) = delete;
        VulkanShaderProgram& operator=(VulkanShaderProgram&& other) noexcept = delete;

        /// \copydoc ShaderProgram::shaders()
        [[nodiscard]] std::vector<const VulkanShaderModule*> shaders() const noexcept override;

        /// \copydoc ShaderProgram::reflectPipelineLayout()
        [[nodiscard]] std::shared_ptr<IPipelineLayout> reflectPipelineLayout() const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
