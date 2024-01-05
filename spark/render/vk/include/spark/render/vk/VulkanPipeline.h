#pragma once

#include "spark/render/Resource.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/Helpers.h"
#include "spark/render/vk/VulkanDescriptorSet.h"
#include "spark/render/vk/VulkanPipelineLayout.h"
#include "spark/render/vk/VulkanShaderProgram.h"

SPARK_FWD_DECLARE_VK_HANDLE(VkPipeline)

namespace spark::render::vk
{
    class VulkanCommandBuffer;

    /**
     * \brief Vulkan implementation of \ref IPipeline.
     */
    class SPARK_RENDER_VK_EXPORT VulkanPipelineState : public virtual Pipeline<VulkanPipelineLayout, VulkanShaderProgram>, public Resource<VkPipeline>
    {
    public:
        using Resource::Resource;

        /**
         * \brief Sets the current pipeline state on the \ref ICommandBuffer.
         * \param command_buffer The \ref ICommandBuffer to set the current pipeline state on.
         */
        virtual void use(const VulkanCommandBuffer& command_buffer) const noexcept = 0;

        /**
         * \brief Binds a descriptor set on a \ref ICommandBuffer.
         * \param command_buffer The \ref ICommandBuffer to issue the bind command on.
         * \param descriptor_set The \ref IDescriptorSet to bind.
         */
        virtual void bind(const VulkanCommandBuffer& command_buffer, const VulkanDescriptorSet& descriptor_set) const noexcept = 0;
    };
}
