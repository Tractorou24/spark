#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/DescriptorSet.h"
#include "spark/render/IndexBuffer.h"
#include "spark/render/Pipeline.h"
#include "spark/render/VertexBuffer.h"

#include "spark/math/Vector2.h"

#include <memory>

namespace spark::render
{
    class SPARK_RENDER_EXPORT IGraphicsFactory
    {
    public:
        virtual ~IGraphicsFactory() = default;

        /**
         * \brief Creates a \ref IBuffer of type \p type.
         * \param type The type of buffer to create.
         * \param usage The buffer usage.
         * \param element_size The size of each element in the buffer. (in bytes)
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(BufferType type, BufferUsage usage, std::size_t element_size, unsigned elements = 1, bool allow_write = false) const
        {
            return genericCreateBuffer(type, usage, element_size, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer of type \p type.
         * \param name The buffer name.
         * \param type The buffer type.
         * \param usage The buffer usage.
         * \param element_size The size of each element in the buffer. (in bytes)
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const std::string& name,
                                                            BufferType type,
                                                            BufferUsage usage,
                                                            std::size_t element_size,
                                                            unsigned elements,
                                                            bool allow_write = false) const
        {
            return genericCreateBuffer(name, type, usage, element_size, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to the specified \p descriptor_set and \p binding.
         * \param descriptor_set The layout of the descriptor set to bind the buffer to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const IDescriptorSetLayout& descriptor_set,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            unsigned elements = 1,
                                                            bool allow_write = false) const
        {
            const auto& descriptor = descriptor_set.descriptor(binding);
            return createBuffer(descriptor->type(), usage, descriptor->elementSize(), elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to the specified \p descriptor_set and \p binding.
         * \param name The buffer name.
         * \param descriptor_set The layout of the descriptor set to bind the buffer to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const std::string& name,
                                                            const IDescriptorSetLayout& descriptor_set,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            unsigned elements = 1,
                                                            bool allow_write = false) const
        {
            const auto& descriptor = descriptor_set.descriptor(binding);
            return createBuffer(name, descriptor->type(), usage, descriptor->elementSize(), elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to the specified \p descriptor_set and \p binding.
         * \param descriptor_set The layout of the descriptor set to bind the buffer to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param element_size The size of each element in the buffer. (in bytes)
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const IDescriptorSetLayout& descriptor_set,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            std::size_t element_size,
                                                            unsigned elements,
                                                            bool allow_write = false) const
        {
            return createBuffer(descriptor_set.descriptor(binding)->type(), usage, element_size, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to a descriptor set of the specified \p pipeline, \p space and \p binding.
         * \param name The buffer name.
         * \param descriptor_set The layout of the descriptor set to bind the buffer to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param element_size The size of each element in the buffer. (in bytes)
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write The buffer usage.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const std::string& name,
                                                            const IDescriptorSetLayout& descriptor_set,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            std::size_t element_size,
                                                            unsigned elements,
                                                            bool allow_write = false) const
        {
            return createBuffer(name, descriptor_set.descriptor(binding)->type(), usage, element_size, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to a descriptor set of the specified \p pipeline, \p space and \p binding.
         * \param pipeline The pipeline that provides the descriptor set layout.
         * \param space The space the descriptor set is bound to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const IPipeline& pipeline,
                                                            unsigned space,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            unsigned elements = 1,
                                                            bool allow_write = false) const
        {
            return createBuffer(pipeline.layout()->descriptorSet(space), binding, usage, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to the specified \p descriptor_set and \p binding.
         * \param name The buffer name.
         * \param pipeline The pipeline that provides the descriptor set layout.
         * \param space The space the descriptor set is bound to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const std::string& name,
                                                            const IPipeline& pipeline,
                                                            unsigned space,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            unsigned elements = 1,
                                                            bool allow_write = false) const
        {
            return createBuffer(name, pipeline.layout()->descriptorSet(space), binding, usage, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to a descriptor set of the specified \p pipeline, \p space and \p binding.
         * \param pipeline The pipeline that provides the descriptor set layout.
         * \param space The space the descriptor set is bound to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param element_size The size of each element in the buffer. (in bytes)
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const IPipeline& pipeline,
                                                            unsigned space,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            std::size_t element_size,
                                                            unsigned elements,
                                                            bool allow_write = false) const
        {
            return createBuffer(pipeline.layout()->descriptorSet(space), binding, usage, element_size, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IBuffer that can be bound to a descriptor set of the specified \p pipeline, \p space and \p binding.
         * \param name The buffer name.
         * \param pipeline The pipeline that provides the descriptor set layout.
         * \param space The space the descriptor set is bound to.
         * \param binding The binding of the descriptor set to bind the buffer to.
         * \param usage The buffer usage.
         * \param element_size The size of each element in the buffer. (in bytes)
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IBuffer.
         */
        [[nodiscard]] std::unique_ptr<IBuffer> createBuffer(const std::string& name,
                                                            const IPipeline& pipeline,
                                                            unsigned space,
                                                            unsigned binding,
                                                            BufferUsage usage,
                                                            std::size_t element_size,
                                                            unsigned elements = 1,
                                                            bool allow_write = false) const
        {
            return createBuffer(name, pipeline.layout()->descriptorSet(space), binding, usage, element_size, elements, allow_write);
        }

        /**
         * \brief Creates a \ref IVertexBuffer based on \p layout. 
         * \param layout The vertex buffer layout.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \return The created \ref IVertexBuffer.
         *
         * A vertex buffer can be used by different \link IRenderPipeline render pipelines \endlink as long as they share a common input assembler state.
         * The size of the buffer is computed from the element size vertex buffer layout, times the number of elements given by the \p elements parameter.
         */
        [[nodiscard]] std::unique_ptr<IVertexBuffer> createVertexBuffer(const IVertexBufferLayout& layout, BufferUsage usage, unsigned elements = 1) const
        {
            return genericCreateVertexBuffer(layout, usage, elements);
        }

        /**
         * \brief Creates a \ref IVertexBuffer based on \p layout.
         * \param name The buffer name.
         * \param layout The vertex buffer layout.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \return The created \ref IVertexBuffer.
         *
         * A vertex buffer can be used by different \link IRenderPipeline render pipelines \endlink as long as they share a common input assembler state.
         * The size of the buffer is computed from the element size vertex buffer layout, times the number of elements given by the \p elements parameter.
         */
        [[nodiscard]] std::unique_ptr<IVertexBuffer> createVertexBuffer(const std::string& name,
                                                                        const IVertexBufferLayout& layout,
                                                                        BufferUsage usage,
                                                                        unsigned elements = 1) const
        {
            return genericCreateVertexBuffer(name, layout, usage, elements);
        }

        /**
         * \brief Creates a \ref IVertexBuffer based on \p layout.
         * \param layout The vertex buffer layout.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \return The created \ref IVertexBuffer.
         *
         * An index buffer can be used by different \link IRenderPipeline render pipelines \endlink as long as they share a common input assembler state.
         * The size of the buffer is computed from the element size index buffer layout, times the number of elements given by the \p elements parameter.
         */
        [[nodiscard]] std::unique_ptr<IIndexBuffer> createIndexBuffer(const IIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const
        {
            return genericCreateIndexBuffer(layout, usage, elements);
        }

        /**
         * \brief Creates a \ref IVertexBuffer based on \p layout.
         * \param name The buffer name.
         * \param layout The vertex buffer layout.
         * \param usage The buffer usage.
         * \param elements The number of elements in the buffer. (if the buffer is an array).
         * \return The created \ref IVertexBuffer.
         *
         * An index buffer can be used by different \link IRenderPipeline render pipelines \endlink as long as they share a common input assembler state.
         * The size of the buffer is computed from the element size index buffer layout, times the number of elements given by the \p elements parameter.
         */
        [[nodiscard]] std::unique_ptr<IIndexBuffer> createIndexBuffer(const std::string& name, const IIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const
        {
            return genericCreateIndexBuffer(name, layout, usage, elements);
        }

        /**
         * \brief Creates an \ref Image used as a render target attachment.
         * \param format The image format.
         * \param size The extent of the image.
         * \param samples The number of samples the image should be sampled with.
         * \return The created \ref IImage.
         */
        [[nodiscard]] std::unique_ptr<IImage> createAttachment(Format format,
                                                               const math::Vector2<unsigned>& size,
                                                               MultiSamplingLevel samples = MultiSamplingLevel::X1) const
        {
            return genericCreateAttachment(format, size, samples);
        }

        /**
         * \brief Creates an \ref Image used as a render target attachment.
         * \param name The image name.
         * \param format The image format.
         * \param size The extent of the image.
         * \param samples The number of samples the image should be sampled with.
         * \return The created \ref IImage.
         */
        [[nodiscard]] std::unique_ptr<IImage> createAttachment(const std::string& name,
                                                               Format format,
                                                               const math::Vector2<unsigned>& size,
                                                               MultiSamplingLevel samples = MultiSamplingLevel::X1) const
        {
            return genericCreateAttachment(name, format, size, samples);
        }

        /**
         * \brief Creates a \link IImage texture \endlink.
         * \param format The image format.
         * \param size The extent of the image.
         * \param dimension The image dimension.
         * \param levels The number of mipmap levels.
         * \param layers The number of layers.
         * \param samples The number of samples the image should be sampled with.
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IImage.
         *
         * A texture is always backed by GPU-only visible memory and thus can only be transferred to/from. Thus you typically have to create a buffer using 
         * \ref IGraphicsFactory::createBuffer() first that holds the actual image bytes. You than can transfer/copy the contents into the texture.
         */
        [[nodiscard]] std::unique_ptr<IImage> createTexture(Format format,
                                                            const math::Vector3<unsigned>& size,
                                                            ImageDimensions dimension = ImageDimensions::DIM_2,
                                                            unsigned levels = 1,
                                                            unsigned layers = 1,
                                                            MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                                            bool allow_write = false) const
        {
            return genericCreateTexture(format, size, dimension, levels, layers, samples, allow_write);
        }

        /**
         * \brief Creates a \link IImage texture \endlink.
         * \param name The image name.
         * \param format The image format.
         * \param size The extent of the image.
         * \param dimension The image dimension.
         * \param levels The number of mipmap levels.
         * \param layers The number of layers.
         * \param samples The number of samples the image should be sampled with.
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref IImage.
         *
         * A texture is always backed by GPU-only visible memory and thus can only be transferred to/from. Thus you typically have to create a buffer using
         * \ref IGraphicsFactory::createBuffer() first that holds the actual image bytes. You than can transfer/copy the contents into the texture.
         */
        [[nodiscard]] std::unique_ptr<IImage> createTexture(const std::string& name,
                                                            Format format,
                                                            const math::Vector3<unsigned>& size,
                                                            ImageDimensions dimension = ImageDimensions::DIM_2,
                                                            unsigned levels = 1,
                                                            unsigned layers = 1,
                                                            MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                                            bool allow_write = false) const
        {
            return genericCreateTexture(name, format, size, dimension, levels, layers, samples, allow_write);
        }

        /**
         * \brief Creates an array of \link IImage textures \endlink.
         * \param elements The number of textures to create.
         * \param format The images format.
         * \param size The extent of each image.
         * \param dimension The dimension of each image.
         * \param layers The number of layers of each image.
         * \param levels The number of mipmap levels of each image.
         * \param samples The number of samples of each image.
         * \param allow_write `true` to allow the resource to be bound to a read/write descriptor.
         * \return The created \ref std::vector of textures.
         */
        [[nodiscard]] std::vector<std::unique_ptr<IImage>> createTextures(std::size_t elements,
                                                                          Format format,
                                                                          const math::Vector3<unsigned>& size,
                                                                          ImageDimensions dimension = ImageDimensions::DIM_2,
                                                                          unsigned layers = 1,
                                                                          unsigned levels = 1,
                                                                          MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                                                          bool allow_write = false) const
        {
            return genericCreateTextures(elements, format, size, dimension, layers, levels, samples, allow_write);
        }

        /**
         * \brief Creates a \link ISampler texture sampler \endlink.
         * \param mag_filter The filter mode to use for magnification.
         * \param min_filter The filter mode to use for minification.
         * \param border_u The border mode to use along the u-axis.
         * \param border_v The border mode to use along the v-axis.
         * \param border_w The border mode to use along the w-axis.
         * \param mip_map_mode The mip map mode to use.
         * \param mip_map_bias The mip map bias to use.
         * \param max_lod The maximum level of detail to use.
         * \param min_lod The minimum level of detail to use.
         * \param anisotropy The anisotropy level to use.
         * \return The created \ref ISampler.
         */
        [[nodiscard]] std::unique_ptr<ISampler> createSampler(FilterMode mag_filter = FilterMode::Nearest,
                                                              FilterMode min_filter = FilterMode::Nearest,
                                                              BorderMode border_u = BorderMode::Repeat,
                                                              BorderMode border_v = BorderMode::Repeat,
                                                              BorderMode border_w = BorderMode::Repeat,
                                                              MipMapMode mip_map_mode = MipMapMode::Nearest,
                                                              float mip_map_bias = 0.f,
                                                              float max_lod = std::numeric_limits<float>::max(),
                                                              float min_lod = 0.f,
                                                              float anisotropy = 0.f) const
        {
            return genericCreateSampler(mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy);
        }

        /**
         * \brief Creates a \link ISampler texture sampler \endlink.
         * \param name The sampler name.
         * \param mag_filter The filter mode to use for magnification.
         * \param min_filter The filter mode to use for minification.
         * \param border_u The border mode to use along the u-axis.
         * \param border_v The border mode to use along the v-axis.
         * \param border_w The border mode to use along the w-axis.
         * \param mip_map_mode The mip map mode to use.
         * \param mip_map_bias The mip map bias to use.
         * \param max_lod The maximum level of detail to use.
         * \param min_lod The minimum level of detail to use.
         * \param anisotropy The anisotropy level to use.
         * \return The created \ref ISampler.
         */
        [[nodiscard]] std::unique_ptr<ISampler> createSampler(const std::string& name,
                                                              FilterMode mag_filter = FilterMode::Nearest,
                                                              FilterMode min_filter = FilterMode::Nearest,
                                                              BorderMode border_u = BorderMode::Repeat,
                                                              BorderMode border_v = BorderMode::Repeat,
                                                              BorderMode border_w = BorderMode::Repeat,
                                                              MipMapMode mip_map_mode = MipMapMode::Nearest,
                                                              float mip_map_bias = 0.f,
                                                              float max_lod = std::numeric_limits<float>::max(),
                                                              float min_lod = 0.f,
                                                              float anisotropy = 0.f) const
        {
            return genericCreateSampler(name, mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy);
        }

        /**
         * \brief Creates an array of \link ISampler texture samplers \endlink.
         * \param elements The number of samplers to create.
         * \param mag_filter The filter mode to use for magnification of each sampler.
         * \param min_filter The filter mode to use for minification of each sampler.
         * \param border_u The border mode to use along the u-axis of each sampler.
         * \param border_v The border mode to use along the v-axis of each sampler.
         * \param border_w The border mode to use along the w-axis of each sampler.
         * \param mip_map_mode The mip map mode to use of each sampler.
         * \param mip_map_bias The mip map bias to use of each sampler.
         * \param max_lod The maximum level of detail to use of each sampler.
         * \param min_lod The minimum level of detail to use of each sampler.
         * \param anisotropy The anisotropy level to use of each sampler.
         * \return The created \ref std::vector of samplers.
         */
        [[nodiscard]] std::vector<std::unique_ptr<ISampler>> createSamplers(std::size_t elements,
                                                                            FilterMode mag_filter = FilterMode::Nearest,
                                                                            FilterMode min_filter = FilterMode::Nearest,
                                                                            BorderMode border_u = BorderMode::Repeat,
                                                                            BorderMode border_v = BorderMode::Repeat,
                                                                            BorderMode border_w = BorderMode::Repeat,
                                                                            MipMapMode mip_map_mode = MipMapMode::Nearest,
                                                                            float mip_map_bias = 0.f,
                                                                            float max_lod = std::numeric_limits<float>::max(),
                                                                            float min_lod = 0.f,
                                                                            float anisotropy = 0.f) const
        {
            return genericCreateSamplers(elements, mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy);
        }

    private:
        // @formatter:off
        /// @{
        /// \brief Private methods used to allow replacement of the generic methods by custom types.
        [[nodiscard]] virtual std::unique_ptr<IBuffer> genericCreateBuffer(BufferType type, BufferUsage usage, std::size_t element_size, unsigned elements, bool allow_write) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IBuffer> genericCreateBuffer(const std::string& name, BufferType type, BufferUsage usage, std::size_t element_size, unsigned elements, bool allow_write) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IVertexBuffer> genericCreateVertexBuffer(const IVertexBufferLayout& layout, BufferUsage usage, unsigned elements) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IVertexBuffer> genericCreateVertexBuffer(const std::string& name, const IVertexBufferLayout& layout, BufferUsage usage, unsigned elements) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IIndexBuffer> genericCreateIndexBuffer(const IIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IIndexBuffer> genericCreateIndexBuffer(const std::string& name, const IIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IImage> genericCreateAttachment(Format format, const math::Vector2<unsigned>& size, MultiSamplingLevel samples) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IImage> genericCreateAttachment(const std::string& name, Format format, const math::Vector2<unsigned>& size, MultiSamplingLevel samples) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IImage> genericCreateTexture(Format format, const math::Vector3<unsigned>& size, ImageDimensions dimension, unsigned levels, unsigned layers, MultiSamplingLevel samples, bool allow_write) const = 0;
        [[nodiscard]] virtual std::unique_ptr<IImage> genericCreateTexture(const std::string& name, Format format, const math::Vector3<unsigned>& size, ImageDimensions dimension, unsigned levels, unsigned layers, MultiSamplingLevel samples, bool allow_write) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<IImage>> genericCreateTextures(std::size_t elements, Format format, const math::Vector3<unsigned>& size, ImageDimensions dimension, unsigned layers, unsigned levels, MultiSamplingLevel samples, bool allow_write) const = 0;
        [[nodiscard]] virtual std::unique_ptr<ISampler> genericCreateSampler(FilterMode mag_filter, FilterMode min_filter, BorderMode border_u, BorderMode border_v, BorderMode border_w, MipMapMode mip_map_mode, float mip_map_bias, float max_lod, float min_lod, float anisotropy) const = 0;
        [[nodiscard]] virtual std::unique_ptr<ISampler> genericCreateSampler(const std::string& name, FilterMode mag_filter, FilterMode min_filter, BorderMode border_u, BorderMode border_v, BorderMode border_w, MipMapMode mip_map_mode, float mip_map_bias, float max_lod, float min_lod, float anisotropy) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<ISampler>> genericCreateSamplers(std::size_t elements, FilterMode mag_filter, FilterMode min_filter, BorderMode border_u, BorderMode border_v, BorderMode border_w, MipMapMode mip_map_mode, float mip_map_bias, float max_lod, float min_lod, float anisotropy) const = 0;
        /// @}
        // @formatter:on
    };

    /**
     * \brief Describes a factory that creates objects for a \ref IGraphicsDevice.
     * \tparam DescriptorLayoutType Type of the descriptor layout. (inherits from \ref IDescriptorSetLayout)
     * \tparam RawBufferType Type of the raw buffer. (inherits from \ref IBuffer)
     * \tparam VertexBufferType Type of the vertex buffer. (inherits from \ref IVertexBuffer)
     * \tparam IndexBufferType Type of the index buffer. (inherits from \ref IIndexBuffer)
     * \tparam ImageType Type of the image. (inherits from \ref IImage)
     * \tparam SamplerType Type of the sampler. (inherits from \ref ISampler)
     */
    template <typename DescriptorLayoutType, typename RawBufferType, typename VertexBufferType, typename IndexBufferType, typename ImageType, typename SamplerType>
    class GraphicsFactory : public IGraphicsFactory
    {
    public:
        using descriptor_layout_type = DescriptorLayoutType;
        using vertex_buffer_type = VertexBufferType;
        using vertex_buffer_layout_type = typename vertex_buffer_type::vertex_buffer_layout_type;
        using index_buffer_type = IndexBufferType;
        using index_buffer_layout_type = typename index_buffer_type::index_buffer_layout_type;
        using buffer_type = RawBufferType;
        using image_type = ImageType;
        using sampler_type = SamplerType;

    public:
        /// \copydoc IGraphicsFactory::createBuffer()
        [[nodiscard]] virtual std::unique_ptr<buffer_type> createBuffer(BufferType type,
                                                                        BufferUsage usage,
                                                                        std::size_t element_size,
                                                                        unsigned elements,
                                                                        bool allow_write = false) const = 0;

        /// \copydoc IGraphicsFactory::createBuffer()
        [[nodiscard]] virtual std::unique_ptr<buffer_type> createBuffer(const std::string& name,
                                                                        BufferType type,
                                                                        BufferUsage usage,
                                                                        std::size_t element_size,
                                                                        unsigned elements,
                                                                        bool allow_write = false) const = 0;

        /// \copydoc IGraphicsFactory::createVertexBuffer()
        [[nodiscard]] virtual std::unique_ptr<vertex_buffer_type> createVertexBuffer(const vertex_buffer_layout_type& layout, BufferUsage usage, unsigned elements = 1) const =
        0;

        /// \copydoc IGraphicsFactory::createVertexBuffer()
        [[nodiscard]] virtual std::unique_ptr<vertex_buffer_type> createVertexBuffer(const std::string& name,
                                                                                     const vertex_buffer_layout_type& layout,
                                                                                     BufferUsage usage,
                                                                                     unsigned elements = 1) const = 0;

        /// \copydoc IGraphicsFactory::createVertexBuffer()
        [[nodiscard]] virtual std::unique_ptr<index_buffer_type> createIndexBuffer(const index_buffer_layout_type& layout, BufferUsage usage, unsigned elements) const = 0;

        /// \copydoc IGraphicsFactory::createIndexBuffer()
        [[nodiscard]] virtual std::unique_ptr<index_buffer_type> createIndexBuffer(const std::string& name,
                                                                                   const index_buffer_layout_type& layout,
                                                                                   BufferUsage usage,
                                                                                   unsigned elements) const = 0;

        /// \copydoc IGraphicsFactory::createAttachment()
        [[nodiscard]] virtual std::unique_ptr<image_type> createAttachment(Format format,
                                                                           const math::Vector2<unsigned>& size,
                                                                           MultiSamplingLevel samples = MultiSamplingLevel::X1) const = 0;

        /// \copydoc IGraphicsFactory::createAttachment()
        [[nodiscard]] virtual std::unique_ptr<image_type> createAttachment(const std::string& name,
                                                                           Format format,
                                                                           const math::Vector2<unsigned>& size,
                                                                           MultiSamplingLevel samples = MultiSamplingLevel::X1) const = 0;

        /// \copydoc IGraphicsFactory::createTexture()
        [[nodiscard]] virtual std::unique_ptr<image_type> createTexture(Format format,
                                                                        const math::Vector3<unsigned>& size,
                                                                        ImageDimensions dimension = ImageDimensions::DIM_2,
                                                                        unsigned levels = 1,
                                                                        unsigned layers = 1,
                                                                        MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                                                        bool allow_write = false) const = 0;

        /// \copydoc IGraphicsFactory::createTexture()
        [[nodiscard]] virtual std::unique_ptr<image_type> createTexture(const std::string& name,
                                                                        Format format,
                                                                        const math::Vector3<unsigned>& size,
                                                                        ImageDimensions dimension = ImageDimensions::DIM_2,
                                                                        unsigned levels = 1,
                                                                        unsigned layers = 1,
                                                                        MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                                                        bool allow_write = false) const = 0;

        /// \copydoc IGraphicsFactory::createTextures()
        [[nodiscard]] virtual std::vector<std::unique_ptr<image_type>> createTextures(std::size_t elements,
                                                                                      Format format,
                                                                                      const math::Vector3<unsigned>& size,
                                                                                      ImageDimensions dimension = ImageDimensions::DIM_2,
                                                                                      unsigned layers = 1,
                                                                                      unsigned levels = 1,
                                                                                      MultiSamplingLevel samples = MultiSamplingLevel::X1,
                                                                                      bool allow_write = false) const = 0;

        /// \copydoc IGraphicsFactory::createSampler()
        [[nodiscard]] virtual std::unique_ptr<sampler_type> createSampler(FilterMode mag_filter = FilterMode::Nearest,
                                                                          FilterMode min_filter = FilterMode::Nearest,
                                                                          BorderMode border_u = BorderMode::Repeat,
                                                                          BorderMode border_v = BorderMode::Repeat,
                                                                          BorderMode border_w = BorderMode::Repeat,
                                                                          MipMapMode mip_map_mode = MipMapMode::Nearest,
                                                                          float mip_map_bias = 0.f,
                                                                          float max_lod = std::numeric_limits<float>::max(),
                                                                          float min_lod = 0.f,
                                                                          float anisotropy = 0.f) const = 0;

        /// \copydoc IGraphicsFactory::createSampler()
        [[nodiscard]] virtual std::unique_ptr<sampler_type> createSampler(const std::string& name,
                                                                          FilterMode mag_filter = FilterMode::Nearest,
                                                                          FilterMode min_filter = FilterMode::Nearest,
                                                                          BorderMode border_u = BorderMode::Repeat,
                                                                          BorderMode border_v = BorderMode::Repeat,
                                                                          BorderMode border_w = BorderMode::Repeat,
                                                                          MipMapMode mip_map_mode = MipMapMode::Nearest,
                                                                          float mip_map_bias = 0.f,
                                                                          float max_lod = std::numeric_limits<float>::max(),
                                                                          float min_lod = 0.f,
                                                                          float anisotropy = 0.f) const = 0;

        /// \copydoc IGraphicsFactory::createSamplers()
        [[nodiscard]] virtual std::vector<std::unique_ptr<sampler_type>> createSamplers(std::size_t elements,
                                                                                        FilterMode mag_filter = FilterMode::Nearest,
                                                                                        FilterMode min_filter = FilterMode::Nearest,
                                                                                        BorderMode border_u = BorderMode::Repeat,
                                                                                        BorderMode border_v = BorderMode::Repeat,
                                                                                        BorderMode border_w = BorderMode::Repeat,
                                                                                        MipMapMode mip_map_mode = MipMapMode::Nearest,
                                                                                        float mip_map_bias = 0.f,
                                                                                        float max_lod = std::numeric_limits<float>::max(),
                                                                                        float min_lod = 0.f,
                                                                                        float anisotropy = 0.f) const = 0;

    private:
        // @formatter:off
        [[nodiscard]] std::unique_ptr<IBuffer> genericCreateBuffer(render::BufferType type, BufferUsage usage, std::size_t element_size, unsigned elements, bool allow_write) const final { return createBuffer(type, usage, element_size, elements, allow_write); }
        [[nodiscard]] std::unique_ptr<IBuffer> genericCreateBuffer(const std::string& name, render::BufferType type, BufferUsage usage, std::size_t element_size, unsigned elements, bool allow_write) const final { return createBuffer(name, type, usage, element_size, elements, allow_write); }
        [[nodiscard]] std::unique_ptr<IVertexBuffer> genericCreateVertexBuffer(const IVertexBufferLayout& layout, BufferUsage usage, unsigned elements) const final { return createVertexBuffer(dynamic_cast<const vertex_buffer_layout_type&>(layout), usage, elements); }
        [[nodiscard]] std::unique_ptr<IVertexBuffer> genericCreateVertexBuffer(const std::string& name, const IVertexBufferLayout& layout, BufferUsage usage, unsigned elements) const final { return createVertexBuffer(name, dynamic_cast<const vertex_buffer_layout_type&>(layout), usage, elements); }
        [[nodiscard]] std::unique_ptr<IIndexBuffer> genericCreateIndexBuffer(const IIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const final { return createIndexBuffer(dynamic_cast<const index_buffer_layout_type&>(layout), usage, elements); }
        [[nodiscard]] std::unique_ptr<IIndexBuffer> genericCreateIndexBuffer(const std::string& name, const IIndexBufferLayout& layout, BufferUsage usage, unsigned elements) const final { return createIndexBuffer(name, dynamic_cast<const index_buffer_layout_type&>(layout), usage, elements); }
        [[nodiscard]] std::unique_ptr<IImage> genericCreateAttachment(Format format, const math::Vector2<unsigned>& size, MultiSamplingLevel samples) const final { return createAttachment(format, size, samples); }
        [[nodiscard]] std::unique_ptr<IImage> genericCreateAttachment(const std::string& name, Format format, const math::Vector2<unsigned>& size, MultiSamplingLevel samples) const final { return createAttachment(name, format, size, samples); }
        [[nodiscard]] std::unique_ptr<IImage> genericCreateTexture(Format format, const math::Vector3<unsigned>& size, ImageDimensions dimension, unsigned levels, unsigned layers, MultiSamplingLevel samples, bool allow_write) const final { return createTexture(format, size, dimension, levels, layers, samples, allow_write); }
        [[nodiscard]] std::unique_ptr<IImage> genericCreateTexture(const std::string& name, Format format, const math::Vector3<unsigned>& size, ImageDimensions dimension, unsigned levels, unsigned layers, MultiSamplingLevel samples, bool allow_write) const final { return createTexture(name, format, size, dimension, levels, layers, samples, allow_write); }
        [[nodiscard]] std::vector<std::unique_ptr<IImage>> genericCreateTextures(std::size_t elements, Format format, const math::Vector3<unsigned>& size, ImageDimensions dimension, unsigned layers, unsigned levels, MultiSamplingLevel samples, bool allow_write) const final
        {
            auto tmp = createTextures(elements, format, size, dimension, layers, levels, samples, allow_write);
            std::vector<std::unique_ptr<IImage>> result;
            result.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(result), [](auto& ptr) { return std::unique_ptr<IImage>(std::move(ptr)); });
            return result;
        }
        [[nodiscard]] std::unique_ptr<ISampler> genericCreateSampler(FilterMode mag_filter, FilterMode min_filter, BorderMode border_u, BorderMode border_v, BorderMode border_w, MipMapMode mip_map_mode, float mip_map_bias, float max_lod, float min_lod, float anisotropy) const final { return createSampler(mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy); }
        [[nodiscard]] std::unique_ptr<ISampler> genericCreateSampler(const std::string& name, FilterMode mag_filter, FilterMode min_filter, BorderMode border_u, BorderMode border_v, BorderMode border_w, MipMapMode mip_map_mode, float mip_map_bias, float max_lod, float min_lod, float anisotropy) const final { return createSampler(name, mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy); }
        [[nodiscard]] std::vector<std::unique_ptr<ISampler>> genericCreateSamplers(std::size_t elements, FilterMode mag_filter, FilterMode min_filter, BorderMode border_u, BorderMode border_v, BorderMode border_w, MipMapMode mip_map_mode, float mip_map_bias, float max_lod, float min_lod, float anisotropy) const final
        {
            auto tmp = createSamplers(elements, mag_filter, min_filter, border_u, border_v, border_w, mip_map_mode, mip_map_bias, max_lod, min_lod, anisotropy);
            std::vector<std::unique_ptr<ISampler>> result;
            result.reserve(tmp.size());
            std::ranges::transform(tmp, std::back_inserter(result), [](auto& ptr) { return std::unique_ptr<ISampler>(std::move(ptr)); });
            return result;
        }
        // @formatter:on
    };
}
