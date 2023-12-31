#pragma once

#include "spark/render/PushConstantsLayout.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanPushConstantsRange.h"

#include <memory>

namespace spark::render::vk
{
    class VulkanPipelineLayout;

    /**
     * \brief Vulkan implementation of \ref IPushConstantsLayout.
     */
    class SPARK_RENDER_VK_EXPORT VulkanPushConstantsLayout final : public PushConstantsLayout<VulkanPushConstantsRange>
    {
        friend VulkanPipelineLayout;

    public:
        /**
         * \brief Initializes a new \ref VulkanPushConstantsLayout.
         * \param ranges The ranges contained in this layout.
         * \param size The overall size (in bytes) of the push constants backing memory.
         *
         * \throws base::BadArgumentException If more than one range is specified for the same shader stage.
         */
        explicit VulkanPushConstantsLayout(std::vector<std::unique_ptr<VulkanPushConstantsRange>>&& ranges, unsigned int size);
        ~VulkanPushConstantsLayout() override;

        VulkanPushConstantsLayout(const VulkanPushConstantsLayout& other) = delete;
        VulkanPushConstantsLayout(VulkanPushConstantsLayout&& other) noexcept = delete;
        VulkanPushConstantsLayout& operator=(const VulkanPushConstantsLayout& other) = delete;
        VulkanPushConstantsLayout& operator=(VulkanPushConstantsLayout&& other) noexcept = delete;

        /**
         * \brief Gets the parent pipeline layout, the push constants are described for.
         * \return A const reference of the parent pipeline layout.
         */
        [[nodiscard]] const VulkanPipelineLayout& pipelineLayout() const;

        /// \copydoc PushConstantsLayout::size()
        [[nodiscard]] unsigned size() const noexcept override;

        /// \copydoc PushConstantsLayout::range()
        [[nodiscard]] const VulkanPushConstantsRange& range(ShaderStage stage) const override;

        /// \copydoc PushConstantsLayout::ranges()
        [[nodiscard]] std::vector<const VulkanPushConstantsRange*> ranges() const noexcept override;

    private:
        /**
         * \brief Sets the parent pipeline layout, the push constants are described for.
         * \param pipeline_layout The parent pipeline layout.
         */
        void setPipelineLayout(const VulkanPipelineLayout& pipeline_layout);

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
