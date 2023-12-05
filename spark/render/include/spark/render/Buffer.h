#pragma once

#include "spark/render/DeviceMemory.h"
#include "spark/render/Export.h"
#include "spark/render/Mappable.h"
#include "spark/render/StateResource.h"

#include <format>

namespace spark::render
{
    /**
     * \brief Describes a buffer attribute format.
     */
    enum class BufferFormat
    {
        // ReSharper disable CppInconsistentNaming
        None    = 0x00000000,
        X16F    = 0x10000101,
        X16I    = 0x10000201,
        X16U    = 0x10000401,
        XY16F   = 0x10000102,
        XY16I   = 0x10000202,
        XY16U   = 0x10000402,
        XYZ16F  = 0x10000103,
        XYZ16I  = 0x10000203,
        XYZ16U  = 0x10000403,
        XYZW16F = 0x10000104,
        XYZW16I = 0x10000204,
        XYZW16U = 0x10000404,
        X32F    = 0x20000101,
        X32I    = 0x20000201,
        X32U    = 0x20000401,
        XY32F   = 0x20000102,
        XY32I   = 0x20000202,
        XY32U   = 0x20000402,
        XYZ32F  = 0x20000103,
        XYZ32I  = 0x20000203,
        XYZ32U  = 0x20000403,
        XYZW32F = 0x20000104,
        XYZW32I = 0x20000204,
        XYZW32U = 0x20000404
        // ReSharper restore CppInconsistentNaming
    };

    /**
     * \brief Describes the type of a \ref IBuffer.
     */
    enum class BufferType
    {
        /// \brief Describes a vertex buffer.
        Vertex = 0x00000001,

        /// \brief Describes an index buffer.
        Index = 0x00000002,

        /// \brief Describes an uniform buffer object (Vulkan).
        /// Buffers of this type can be bound to `ConstantBuffer` descriptors.
        Uniform = 0x00000003,

        /// \brief Describes a shader storage buffer object (Vulkan).
        /// Buffers of this type can be bound to `StructuredBuffer`/`RWStructuredBuffer` or `ByteAddressBuffer`/`RWByteAddressBuffer` descriptors.
        Storage = 0x00000004,

        /// \brief Describes a shader texel storage buffer object (Vulkan).
        /// Buffers of this type can be bound to `Buffer`/`RWBuffer` descriptors.
        Texel = 0x00000005,

        /// \brief Describes another type of buffer, such as samplers or images.
        /// Buffers of this type must not be bound to any descriptor, but can be used as copy/transfer targets and sources.
        Other = 0x7FFFFFFF
    };

    /**
      * \brief Defines how a buffer is used and describes how its memory is managed.
      *
      * There are three common buffer usage scenarios that are supported by the library:
      * - Static resources: such as vertex/index/constant buffers, textures or other infrequently updated buffers. In this case, the most efficient 
      *   approach is to create a buffer using \ref BufferUsage::Staging and map it from the CPU. Create a second buffer using 
      *   \ref BufferUsage::Resource and transfer the staging buffer into it.
      * - Dynamic resources: such as deformable meshes or buffers that need to be updated every frame. For such buffers use the
      *   \ref BufferUsage::Dynamic mode to prevent regular transfer overhead.
      * - Readbacks: or resources that are written on the GPU and read by the CPU. The usage mode \ref BufferUsage::Readback is designed to 
      * provide the best performance for this special case.
      */
    enum class BufferUsage
    {
        /// \brief Creates a buffer that can optimally be mapped from the CPU in order to be transferred to the GPU later.
        /// The memory for the buffer will be allocated in the DRAM (CPU or host memory). It can be optimally accessed by the CPU in order to be written. However,
        /// reading it from the GPU may be inefficient. This usage mode should be used to create a staging buffer, i.e. a buffer that is written infrequently and
        /// then transferred to another buffer, that uses \ref BufferUsage::Resource.
        Staging = 0x00000001,

        /// \brief Creates a buffer that can optimally be read by the GPU.
        /// The memory for the buffer will be allocated on the VRAM (GPU or device memory). It can be optimally accessed by the GPU in order to be read frequently.
        /// It can be written by a transfer call. Note that those come with an overhead and should only occur infrequently.
        Resource = 0x00000002,

        /// \brief Creates a buffer that can be optimally mapped by the CPU and is preferred to be optimally read by the GPU.
        /// Dynamic buffers are used when the content is expected to be changed every frame. They do not require transfer calls, but may not be read as efficiently
        /// as \ref BufferUsage::Resource buffers.
        Dynamic = 0x00000010,

        /// \brief Creates a buffer that can be written by the GPU and read by the CPU.
        Readback = 0x00000100
    };

    /**
     * \brief Describes the semantic of a buffer attribute.
     */
    enum class AttributeSemantic
    {
        /// \brief The attribute contains a bi-normal vector.
        Binormal = 0x00000001,

        /// \brief The attribute contains blend indices.
        BlendIndices = 0x00000002,

        /// \brief The attribute contains blend weights.
        BlendWeight = 0x00000003,

        /// \brief The attribute contains a color value.
        Color = 0x00000004,

        /// \brief The attribute contains a normal vector.
        Normal = 0x00000005,

        /// \brief The attribute contains a position vector.
        Position = 0x00000006,

        /// \brief The attribute contains a pre-transformed position vector.
        TransformedPosition = 0x00000007,

        /// \brief The attribute contains a point size.
        PointSize = 0x00000008,

        /// \brief The attribute contains a tangent vector.
        Tangent = 0x00000009,

        /// \brief The attribute contains a texture coordinate.
        TextureCoordinate = 0x0000000A,

        /// \brief The attribute is a generic, unknown semantic.
        Unknown = 0x7FFFFFFF
    };

    /**
     * \brief Base interface for all buffers.
     */
    class SPARK_RENDER_EXPORT IBuffer : public IDeviceMemory, public IMappable, public virtual IStateResource
    {
    public:
        ~IBuffer() noexcept override = default;

        /**
         * \brief Gets the type of the buffer.
         * \return A \ref BufferType representing the type of the buffer.
         */
        [[nodiscard]] virtual BufferType type() const = 0;
    };

    /**
     * \brief Describes the layout of a buffer.
     */
    class SPARK_RENDER_EXPORT IBufferLayout
    {
    public:
        virtual ~IBufferLayout() noexcept = default;

        /**
         * \brief Gets the size of a single element in the buffer.
         * \return The size of a single element in the buffer.
         */
        [[nodiscard]] virtual std::size_t elementSize() const noexcept = 0;

        /**
         * \brief Gets the binding point the buffer will be bound to.
         * \return The binding point the buffer will be bound to.
         */
        [[nodiscard]] virtual unsigned int binding() const noexcept = 0;

        /**
         * \brief Gets the type of the buffer.
         * \return A \ref BufferType representing the type of the buffer.
         */
        [[nodiscard]] virtual BufferType type() const noexcept = 0;
    };

    /**
     * \brief Stores metadata about a buffer attribute, member or field of a descriptor or buffer.
     */
    class SPARK_RENDER_EXPORT BufferAttribute final
    {
    public:
        /**
         * \brief Initializes an empty buffer attribute.
         */
        BufferAttribute() noexcept;

        /**
         * \brief Initializes a new buffer attribute with the specified parameters.
         * \param location The location the buffer attribute is bound to.
         * \param offset The offset of the buffer attribute relative to the start of the buffer.
         * \param format The format of the buffer attribute.
         * \param semantic The semantic of the buffer attribute.
         * \param semantic_index The semantic index of the buffer attribute.
         */
        BufferAttribute(unsigned int location, unsigned int offset, BufferFormat format, AttributeSemantic semantic, unsigned int semantic_index) noexcept;
        ~BufferAttribute() noexcept;

        BufferAttribute(const BufferAttribute& other) = delete;
        BufferAttribute(BufferAttribute&& other) noexcept;
        BufferAttribute& operator=(const BufferAttribute& other) = delete;
        BufferAttribute& operator=(BufferAttribute&& other) noexcept;

        /**
         * \brief Gets the location the buffer attribute is bound to.
         * \return The location the buffer attribute is bound to.
         *
         * \note Locations can only be specified with Vulkan, any other backend will ignore this value and use their implicitly generated locations.
         */
        [[nodiscard]] unsigned int location() const noexcept;

        /**
         * \brief Gets the format of the buffer attribute.
         * \return A \ref BufferFormat representing the format of the buffer attribute.
         */
        [[nodiscard]] BufferFormat format() const noexcept;

        /**
         * \brief Gets the offset of the buffer attribute relative to the start of the buffer.
         * \return The offset of the buffer attribute relative to the start of the buffer.
         */
        [[nodiscard]] unsigned int offset() const noexcept;

        /**
         * \brief Gets the semantic of the buffer attribute.
         * \return A \ref AttributeSemantic representing the semantic of the buffer attribute.
         *
         * \note Semantics are only used in DirectX and HLSL.
         */
        [[nodiscard]] AttributeSemantic semantic() const noexcept;

        /**
         * \brief Gets the semantic index of the buffer attribute.
         * \return The semantic index of the buffer attribute.
         *
         * \note Semantic indices are only used in DirectX and HLSL.
         */
        [[nodiscard]] unsigned int semanticIndex() const noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}

template <>
struct std::formatter<spark::render::BufferFormat> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::BufferFormat format, auto& ctx)
    {
        switch (format)
        {
        case spark::render::BufferFormat::None:
            return std::format_to(ctx.out(), "None");
        case spark::render::BufferFormat::X16F:
            return std::format_to(ctx.out(), "X16F");
        case spark::render::BufferFormat::X16I:
            return std::format_to(ctx.out(), "X16I");
        case spark::render::BufferFormat::X16U:
            return std::format_to(ctx.out(), "X16U");
        case spark::render::BufferFormat::XY16F:
            return std::format_to(ctx.out(), "XY16F");
        case spark::render::BufferFormat::XY16I:
            return std::format_to(ctx.out(), "XY16I");
        case spark::render::BufferFormat::XY16U:
            return std::format_to(ctx.out(), "XY16U");
        case spark::render::BufferFormat::XYZ16F:
            return std::format_to(ctx.out(), "XYZ16F");
        case spark::render::BufferFormat::XYZ16I:
            return std::format_to(ctx.out(), "XYZ16I");
        case spark::render::BufferFormat::XYZ16U:
            return std::format_to(ctx.out(), "XYZ16U");
        case spark::render::BufferFormat::XYZW16F:
            return std::format_to(ctx.out(), "XYZW16F");
        case spark::render::BufferFormat::XYZW16I:
            return std::format_to(ctx.out(), "XYZW16I");
        case spark::render::BufferFormat::XYZW16U:
            return std::format_to(ctx.out(), "XYZW16U");
        case spark::render::BufferFormat::X32F:
            return std::format_to(ctx.out(), "X32F");
        case spark::render::BufferFormat::X32I:
            return std::format_to(ctx.out(), "X32I");
        case spark::render::BufferFormat::X32U:
            return std::format_to(ctx.out(), "X32U");
        case spark::render::BufferFormat::XY32F:
            return std::format_to(ctx.out(), "XY32F");
        case spark::render::BufferFormat::XY32I:
            return std::format_to(ctx.out(), "XY32I");
        case spark::render::BufferFormat::XY32U:
            return std::format_to(ctx.out(), "XY32U");
        case spark::render::BufferFormat::XYZ32F:
            return std::format_to(ctx.out(), "XYZ32F");
        case spark::render::BufferFormat::XYZ32I:
            return std::format_to(ctx.out(), "XYZ32I");
        case spark::render::BufferFormat::XYZ32U:
            return std::format_to(ctx.out(), "XYZ32U");
        case spark::render::BufferFormat::XYZW32F:
            return std::format_to(ctx.out(), "XYZW32F");
        case spark::render::BufferFormat::XYZW32I:
            return std::format_to(ctx.out(), "XYZW32I");
        case spark::render::BufferFormat::XYZW32U:
            return std::format_to(ctx.out(), "XYZW32U");
        }
        return std::format_to(ctx.out(), "Unknown");
    }
};

template <>
struct std::formatter<spark::render::BufferType> : std::formatter<std::string_view>
{
    static constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    static constexpr auto format(const spark::render::BufferType type, auto& ctx)
    {
        switch (type)
        {
        case spark::render::BufferType::Vertex:
            return std::format_to(ctx.out(), "Vertex");
        case spark::render::BufferType::Index:
            return std::format_to(ctx.out(), "Index");
        case spark::render::BufferType::Uniform:
            return std::format_to(ctx.out(), "Uniform");
        case spark::render::BufferType::Storage:
            return std::format_to(ctx.out(), "Storage");
        case spark::render::BufferType::Texel:
            return std::format_to(ctx.out(), "Texel");
        case spark::render::BufferType::Other:
            return std::format_to(ctx.out(), "Other");
        }
        return std::format_to(ctx.out(), "Unknown");
    }
};
