#pragma once

#include "spark/render/InputAttachementMapping.h"
#include "spark/render/RenderTarget.h"
#include "spark/render/vk/Export.h"

#include <memory>

namespace spark::render::vk
{
    class VulkanRenderPass;

    /**
     * \brief Vulkan implementation of \ref IInputAttachmentMapping.
     */
    class SPARK_RENDER_VK_EXPORT VulkanInputAttachmentMapping final : public InputAttachmentMapping<VulkanRenderPass>
    {
    public:
        /// \brief Initializes a default \ref VulkanInputAttachmentMapping.
        explicit VulkanInputAttachmentMapping();

        /**
         * \brief Initializes a \ref VulkanInputAttachmentMapping.
         * \param render_pass The \ref VulkanRenderPass to fetch the input attachment from.
         * \param render_target The \ref RenderTarget of the \ref RenderPass that is used for the input attachment.
         * \param location The location to bind the input attachment to.
         */
        explicit VulkanInputAttachmentMapping(const VulkanRenderPass& render_pass, const RenderTarget& render_target, unsigned location);
        ~VulkanInputAttachmentMapping() override;

        VulkanInputAttachmentMapping(const VulkanInputAttachmentMapping& other);
        VulkanInputAttachmentMapping(VulkanInputAttachmentMapping&& other) noexcept;
        VulkanInputAttachmentMapping& operator=(const VulkanInputAttachmentMapping& other);
        VulkanInputAttachmentMapping& operator=(VulkanInputAttachmentMapping&& other) noexcept;

        /// \copydoc InputAttachmentMapping::source()
        [[nodiscard]] const VulkanRenderPass* source() const noexcept override;

        /// \copydoc InputAttachmentMapping::renderTarget()
        [[nodiscard]] const RenderTarget& renderTarget() const noexcept override;

        /// \copydoc InputAttachmentMapping::location()
        [[nodiscard]] unsigned location() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
