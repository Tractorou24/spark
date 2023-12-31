#pragma once

#include "spark/render/PushConstantsRange.h"
#include "spark/render/vk/Export.h"

#include <memory>

namespace spark::render::vk
{
    /**
     * \brief Vulkan implementation of \ref IPushConstantsRange.
     */
    class SPARK_RENDER_VK_EXPORT VulkanPushConstantsRange final : public IPushConstantsRange
    {
    public:
        /**
         * \brief Initializes a new \ref VulkanPushConstantsRange.
         * \param shader_stage The \ref ShaderStage that accesses the push constants from the range.
         * \param offset The offset relative to the parent push constants backing memory that marks the beginning of the range.
         * \param size The size of the push constants range.
         * \param space The space from which the push constants of the range will be accessible in the shader.
         * \param binding The register from which the push constants of the range will be accessible in the shader.
         *
         * \throws base::BadArgumentException if one of the following conditions is met:
         *     - \p offset is not a multiple of 4.
         *     - \p size is not a multiple of 4.
         *     - \p shader_stage is more than one bit set.
         */
        explicit VulkanPushConstantsRange(ShaderStage shader_stage, unsigned int offset, unsigned int size, unsigned int space, unsigned int binding);
        ~VulkanPushConstantsRange() override;

        VulkanPushConstantsRange(const VulkanPushConstantsRange& other) = delete;
        VulkanPushConstantsRange(VulkanPushConstantsRange&& other) noexcept = delete;
        VulkanPushConstantsRange& operator=(const VulkanPushConstantsRange& other) = delete;
        VulkanPushConstantsRange& operator=(VulkanPushConstantsRange&& other) noexcept = delete;

        /// \copydoc IPushConstantsRange::space()
        [[nodiscard]] unsigned space() const noexcept override;

        /// \copydoc IPushConstantsRange::binding()
        [[nodiscard]] unsigned binding() const noexcept override;

        /// \copydoc IPushConstantsRange::offset()
        [[nodiscard]] unsigned offset() const noexcept override;

        /// \copydoc IPushConstantsRange::size()
        [[nodiscard]] unsigned size() const noexcept override;

        /// \copydoc IPushConstantsRange::stage()
        [[nodiscard]] ShaderStage stage() const noexcept override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
