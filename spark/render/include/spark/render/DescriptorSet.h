#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/DescriptorBinding.h"
#include "spark/render/Export.h"
#include "spark/render/Sampler.h"
#include "spark/render/ShaderStages.h"

#include <algorithm>
#include <memory>

namespace spark::render
{
    /**
     * \brief Describes the type of a \ref IDescriptor.
     * \note In theory, we can declare a writable descriptor in any format, the rendering back-end might not necessarily support writing in a specific format.
     */
    enum class DescriptorType
    {
        // ReSharper disable CppInconsistentNaming

        /// \brief A read-only uniform buffer in Vulkan. In GLSL, use the `uniform` keyword to access a uniform buffer. In HLSL, use the `ConstantBuffer` keyword.
        ConstantBuffer = 0x00000001,

        /// \brief A shader storage buffer object in Vulkan. In GLSL, use the `buffer` keyword to access storage buffers. In HLSL, use the `StructuredBuffer` keyword.
        /// A storage buffer is read-only by default. If you want to create a writable storage buffer, use `WritableStorage` instead.
        /// The difference between \c ConstantBuffer and storage buffers is, that storage buffers can have variable length. However, they are typically less efficient.
        StructuredBuffer = 0x00000002,

        /// \brief A writable shader storage object in Vulkan. In GLSL, use the `buffer` keyword to access storage buffers. In HLSL, use the `RWStructuredBuffer` keyword.
        RWStructuredBuffer = 0x00000012,

        /// \brief A read-only sampled image. In GLSL, use the `uniform texture` keywords to access the texture. In HLSL, use the `Texture` keywords.
        /// Textures are read-only by default. If you want to create a writable texture, use the `WritableTexture` instead.
        /// Note, that textures are typically not be accessed directly, but instead are sampled using a \ref ISampler.
        Texture = 0x00000003,

        /// \brief A writable image. In GLSL, use the `uniform image` keywords to access the texture. In HLSL, use the `RWTexture` keywords.
        RWTexture = 0x00000013,

        /// \brief A sampler state of a texture or image.
        Sampler = 0x00000004,

        /// \brief The result of a render target from an earlier render pass. Maps to a `SubpassInput` in HLSL.
        InputAttachment = 0x00000005,

        /// \brief Represents a read-only texel buffer (uniform texel buffer). Use the `uniform imageBuffer` keyword in GLSL to access the buffer. In HLSL, use the `Buffer` keyword.
        Buffer = 0x00000006,

        /// \brief Represents a writable texel buffer (storage texel buffer). Use the `uniform imageBuffer` keyword in GLSL to access the buffer. In HLSL, use the `RWBuffer` keyword.
        RWBuffer = 0x00000016,

        /// \brief Represents an unformatted buffer. In GLSL, use the `buffer` keyword to access byte address buffers. In HLSL, use the `ByteAddressBuffer` keyword.
        ByteAddressBuffer = 0x00000007,

        /// \brief Represents an unformatted writable buffer. In GLSL, use the `buffer` keyword to access byte address buffers. In HLSL, use the `RWByteAddressBuffer` keyword.
        RWByteAddressBuffer = 0x00000017,

        // ReSharper restore CppInconsistentNaming
    };

    /**
     * \brief Describes a the layout of a single descriptor within a \ref DescriptorSet.
     *
     * A common metaphor for a descriptor to think of it as a "pointer for the GPU". Basically, a descriptor points to a buffer in a shader. A descriptor 
     * can have different types and sizes. The types a descriptor can have are described by the \ref DescriptorType.
     * 
     * If the descriptor is a sampler, it can either be a dynamic or static sampler. A dynamic sampler needs to be bound during runtime just like any other
     * descriptor by calling \ref IDescriptorSet::update(). A static sampler is defined alongside the descriptor layout and is automatically set
     * when the pipeline that uses the descriptor layout gets bound. In this case, the descriptor must not be updated with another sampler. If a descriptor
     * layout describes a static sampler, the \ref IDescriptorLayout::staticSampler() returns a pointer to the static sampler state.
     */
    class SPARK_RENDER_EXPORT IDescriptorLayout : public IBufferLayout
    {
    public:
        ~IDescriptorLayout() noexcept override = default;

        /**
         * \brief Gets the type of the descriptor.
         * \return A \ref DescriptorType representing the type of the descriptor.
         */
        [[nodiscard]] virtual DescriptorType descriptorType() const noexcept = 0;

        /**
         * \brief Gets the number of descriptors in the descriptor array, or `0xFFFFFFFF` if the descriptor is unbounded.
         * \return The number of descriptors in the descriptor array, or `0xFFFFFFFF` if the descriptor is unbounded.
         *
         * \note If the descriptor is unbounded, the size of the array must be specified when allocating a \ref IDescriptorSet using the `descriptors` parameter.
         */
        [[nodiscard]] virtual unsigned int descriptors() const noexcept = 0;

        /**
         * \brief Gets the state of the sampler if it is a static sampler.
         * \return A pointer to \ref ISampler containing the state of the sampler if the descriptor is a static sampler, or `nullptr` if the descriptor is not a static sampler.
         */
        [[nodiscard]] virtual const ISampler* staticSampler() const noexcept = 0;
    };

    /**
     * \brief Interface for a descriptor set.
     */
    class SPARK_RENDER_EXPORT IDescriptorSet
    {
    public:
        virtual ~IDescriptorSet() noexcept = default;

        /**
         * \brief Updates a constant buffer within the \ref IDescriptorSet.
         * \param binding The buffer binding point.
         * \param buffer The constant buffer to write to the \ref IDescriptorSet.
         * \param buffer_element The index of the first element to write to the \ref IDescriptorSet.
         * \param elements The number of elements to write to the \ref IDescriptorSet, or `0` to write all elements starting from \p buffer_element.
         * \param first_descriptor The index of the first descriptor in the descriptor array to update.
         */
        void update(unsigned int binding, const IBuffer& buffer, unsigned int buffer_element = 0, unsigned int elements = 0, unsigned int first_descriptor = 0) const { genericUpdate(binding, buffer, buffer_element, elements, first_descriptor); }

        /**
         * \brief Updates a texture within the current descriptor set.
         * \param binding The binding point of the texture.
         * \param texture The texture to write to the descriptor set.
         * \param descriptor The index of the descriptor in the descriptor array to bind the texture to.
         * \param first_level The index of the first mip-map level to bind.
         * \param levels The number of mip-map levels to bind. A value of `0` binds all available levels, starting at \p firstLevel.
         * \param first_layer The index of the first layer to bind.
         * \param layers The number of layers to bind. A value of `0` binds all available layers, starting at \p firstLayer.
         */
        void update(unsigned int binding, const IImage& texture, unsigned int descriptor = 0, unsigned int first_level = 0, unsigned int levels = 0, unsigned int first_layer = 0, unsigned int layers = 0) const { genericUpdate(binding, texture, descriptor, first_level, levels, first_layer, layers); }

        /**
         * \brief Updates a sampler within the current descriptor set.
         * \param binding The binding point of the sampler.
         * \param sampler The sampler to write to the descriptor set.
         * \param descriptor The index of the descriptor in the descriptor array to bind the sampler to.
         */
        void update(unsigned int binding, const ISampler& sampler, unsigned int descriptor = 0) const { genericUpdate(binding, sampler, descriptor); }

        /**
         * \brief Attaches an image as an input attachment to a descriptor bound at \p binding.
         * \param binding The input attachment binding point.
         * \param image The image to bind to the input attachment descriptor.
         */
        void attach(unsigned int binding, const IImage& image) const { genericAttach(binding, image); }

    private:
        /// @{
        /// \brief Private method used to allow the replacement of the generic methods by a custom \ref IBuffer type.
        virtual void genericUpdate(unsigned int binding, const IBuffer& buffer, unsigned int buffer_element, unsigned int elements, unsigned int first_descriptor) const = 0;
        virtual void genericUpdate(unsigned int binding, const IImage& texture, unsigned int descriptor, unsigned int first_level, unsigned int levels, unsigned int first_layer, unsigned int layers) const = 0;
        virtual void genericUpdate(unsigned int binding, const ISampler& sampler, unsigned int descriptor) const = 0;
        virtual void genericAttach(unsigned int binding, const IImage& image) const = 0;
        /// @}
    };
}

template <>
struct std::formatter<spark::render::DescriptorType> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::DescriptorType type, auto& ctx)
    {
        switch (type)
        {
        case spark::render::DescriptorType::ConstantBuffer:
            return std::format_to(ctx.out(), "ConstantBuffer");
        case spark::render::DescriptorType::StructuredBuffer:
            return std::format_to(ctx.out(), "StructuredBuffer");
        case spark::render::DescriptorType::RWStructuredBuffer:
            return std::format_to(ctx.out(), "RWStructuredBuffer");
        case spark::render::DescriptorType::Texture:
            return std::format_to(ctx.out(), "Texture");
        case spark::render::DescriptorType::RWTexture:
            return std::format_to(ctx.out(), "RWTexture");
        case spark::render::DescriptorType::Sampler:
            return std::format_to(ctx.out(), "Sampler");
        case spark::render::DescriptorType::InputAttachment:
            return std::format_to(ctx.out(), "InputAttachment");
        case spark::render::DescriptorType::Buffer:
            return std::format_to(ctx.out(), "Buffer");
        case spark::render::DescriptorType::RWBuffer:
            return std::format_to(ctx.out(), "RWBuffer");
        case spark::render::DescriptorType::ByteAddressBuffer:
            return std::format_to(ctx.out(), "ByteAddressBuffer");
        case spark::render::DescriptorType::RWByteAddressBuffer:
            return std::format_to(ctx.out(), "RWByteAddressBuffer");
        }
        return std::format_to(ctx.out(), "Unknown");
    }
};
