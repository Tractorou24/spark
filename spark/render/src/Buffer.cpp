#include "spark/render/Buffer.h"

namespace spark::render
{
    struct BufferAttribute::Impl
    {
        friend BufferAttribute;

    public:
        Impl(const unsigned int location,
             const unsigned int offset,
             const BufferFormat format,
             const AttributeSemantic semantic,
             const unsigned int semantic_index) noexcept
            : m_location(location)
            , m_offset(offset)
            , m_format(format)
            , m_semantic(semantic)
            , m_semanticIndex(semantic_index) {}

    private:
        unsigned int m_location;
        unsigned int m_offset;
        BufferFormat m_format;
        AttributeSemantic m_semantic;
        unsigned int m_semanticIndex;
    };

    BufferAttribute::BufferAttribute() noexcept
        : m_impl(std::make_unique<Impl>(0, 0, BufferFormat::None, AttributeSemantic::Unknown, 0)) {}

    BufferAttribute::BufferAttribute(unsigned int location, unsigned int offset, BufferFormat format, AttributeSemantic semantic, unsigned int semantic_index) noexcept
        : m_impl(std::make_unique<Impl>(location, offset, format, semantic, semantic_index)) {}

    BufferAttribute::~BufferAttribute() noexcept = default;

    BufferAttribute::BufferAttribute(BufferAttribute&& other) noexcept
        : m_impl(std::move(other.m_impl)) {}

    BufferAttribute& BufferAttribute::operator=(BufferAttribute&& other) noexcept
    {
        if (&other == this)
            return *this;
        other.m_impl = std::move(m_impl);
        return *this;
    }

    unsigned BufferAttribute::location() const noexcept
    {
        return m_impl->m_location;
    }

    BufferFormat BufferAttribute::format() const noexcept
    {
        return m_impl->m_format;
    }

    unsigned BufferAttribute::offset() const noexcept
    {
        return m_impl->m_offset;
    }

    AttributeSemantic BufferAttribute::semantic() const noexcept
    {
        return m_impl->m_semantic;
    }

    unsigned BufferAttribute::semanticIndex() const noexcept
    {
        return m_impl->m_semanticIndex;
    }
}
