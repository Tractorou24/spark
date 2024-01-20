#pragma once

#include "spark/render/GraphicsFactory.h"
#include "spark/render/vk/Export.h"
#include "spark/render/vk/VulkanBuffer.h"
#include "spark/render/vk/VulkanDescriptorLayout.h"
#include "spark/render/vk/VulkanImage.h"
#include "spark/render/vk/VulkanIndexBuffer.h"
#include "spark/render/vk/VulkanVertexBuffer.h"

namespace spark::render::vk
{
    /**
     * \brief Vulkan implementation of \ref IGraphicsFactory.
     */
    class SPARK_RENDER_VK_EXPORT VulkanFactory final : public GraphicsFactory<VulkanDescriptorLayout, IVulkanBuffer, IVulkanVertexBuffer, IVulkanIndexBuffer, IVulkanImage,
                                                                              IVulkanSampler>
    {
    public:
        /**
         * \brief Creates a new \ref VulkanFactory.
         * \param device The device the factory should produce objects for.
         */
        explicit VulkanFactory(const VulkanDevice& device);
        ~VulkanFactory() override;

        VulkanFactory(const VulkanFactory& other) = delete;
        VulkanFactory(VulkanFactory&& other) noexcept = delete;
        VulkanFactory& operator=(const VulkanFactory& other) = delete;
        VulkanFactory& operator=(VulkanFactory&& other) noexcept = delete;

        /// \copydoc IGraphicsFactory::createBuffer()
        [[nodiscard]] std::unique_ptr<IVulkanBuffer> createBuffer(BufferType type,
                                                                  BufferUsage usage,
                                                                  std::size_t element_size,
                                                                  unsigned elements,
                                                                  bool allow_write) const override;

        /// \copydoc IGraphicsFactory::createBuffer()
        [[nodiscard]] std::unique_ptr<IVulkanBuffer> createBuffer(const std::string& name,
                                                                  BufferType type,
                                                                  BufferUsage usage,
                                                                  std::size_t element_size,
                                                                  unsigned elements,
                                                                  bool allow_write) const override;

        /// \copydoc IGraphicsFactory::createVertexBuffer()
        [[nodiscard]] std::unique_ptr<IVulkanVertexBuffer> createVertexBuffer(const VulkanVertexBufferLayout& layout, BufferUsage usage, unsigned elements) const override;

        /// \copydoc IGraphicsFactory::createVertexBuffer()
        [[nodiscard]] std::unique_ptr<IVulkanVertexBuffer> createVertexBuffer(const std::string& name,
                                                                              const VulkanVertexBufferLayout& layout,
                                                                              BufferUsage usage,
                                                                              unsigned elements) const override;

        /// \copydoc IGraphicsFactory::createIndexBuffer()
        [[nodiscard]] std::unique_ptr<IVulkanIndexBuffer> createIndexBuffer(const VulkanIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const override;

        /// \copydoc IGraphicsFactory::createIndexBuffer()
        [[nodiscard]] std::unique_ptr<IVulkanIndexBuffer> createIndexBuffer(const std::string& name,
                                                                            const VulkanIndexBufferLayout& layout,
                                                                            BufferUsage usage,
                                                                            unsigned elements) const override;

        /// \copydoc IGraphicsFactory::createAttachment()
        [[nodiscard]] std::unique_ptr<IVulkanImage> createAttachment(Format format, const math::Vector2<unsigned>& size, MultiSamplingLevel samples) const override;

        /// \copydoc IGraphicsFactory::createAttachment()
        [[nodiscard]] std::unique_ptr<IVulkanImage> createAttachment(const std::string& name,
                                                                     Format format,
                                                                     const math::Vector2<unsigned>& size,
                                                                     MultiSamplingLevel samples) const override;

        /// \copydoc IGraphicsFactory::createTexture()
        [[nodiscard]] std::unique_ptr<IVulkanImage> createTexture(Format format,
                                                                  const math::Vector3<unsigned>& size,
                                                                  ImageDimensions dimension,
                                                                  unsigned levels,
                                                                  unsigned layers,
                                                                  MultiSamplingLevel samples,
                                                                  bool allow_write) const override;

        /// \copydoc IGraphicsFactory::createTexture()
        [[nodiscard]] std::unique_ptr<IVulkanImage> createTexture(const std::string& name,
                                                                  Format format,
                                                                  const math::Vector3<unsigned>& size,
                                                                  ImageDimensions dimension,
                                                                  unsigned levels,
                                                                  unsigned layers,
                                                                  MultiSamplingLevel samples,
                                                                  bool allow_write) const override;

        /// \copydoc IGraphicsFactory::createTextures()
        [[nodiscard]] std::vector<std::unique_ptr<IVulkanImage>> createTextures(std::size_t elements,
                                                                                Format format,
                                                                                const math::Vector3<unsigned>& size,
                                                                                ImageDimensions dimension,
                                                                                unsigned layers,
                                                                                unsigned levels,
                                                                                MultiSamplingLevel samples,
                                                                                bool allow_write) const override;

        /// \copydoc IGraphicsFactory::createSampler()
        [[nodiscard]] std::unique_ptr<IVulkanSampler> createSampler(FilterMode mag_filter,
                                                                    FilterMode min_filter,
                                                                    BorderMode border_u,
                                                                    BorderMode border_v,
                                                                    BorderMode border_w,
                                                                    MipMapMode mip_map_mode,
                                                                    float mip_map_bias,
                                                                    float max_lod,
                                                                    float min_lod,
                                                                    float anisotropy) const override;

        /// \copydoc IGraphicsFactory::createSampler()
        [[nodiscard]] std::unique_ptr<IVulkanSampler> createSampler(const std::string& name,
                                                                    FilterMode mag_filter,
                                                                    FilterMode min_filter,
                                                                    BorderMode border_u,
                                                                    BorderMode border_v,
                                                                    BorderMode border_w,
                                                                    MipMapMode mip_map_mode,
                                                                    float mip_map_bias,
                                                                    float max_lod,
                                                                    float min_lod,
                                                                    float anisotropy) const override;

        /// \copydoc IGraphicsFactory::createSamplers()
        [[nodiscard]] std::vector<std::unique_ptr<IVulkanSampler>> createSamplers(std::size_t elements,
                                                                                  FilterMode mag_filter,
                                                                                  FilterMode min_filter,
                                                                                  BorderMode border_u,
                                                                                  BorderMode border_v,
                                                                                  BorderMode border_w,
                                                                                  MipMapMode mip_map_mode,
                                                                                  float mip_map_bias,
                                                                                  float max_lod,
                                                                                  float min_lod,
                                                                                  float anisotropy) const override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
