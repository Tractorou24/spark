#pragma once

#include "spark/render/Buffer.h"
#include "spark/render/DescriptorBinding.h"
#include "spark/render/Export.h"
#include "spark/render/Sampler.h"
#include "spark/render/ShaderStages.h"

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
