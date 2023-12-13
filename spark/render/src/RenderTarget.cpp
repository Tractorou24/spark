#include "spark/render/RenderTarget.h"

namespace spark::render
{
    struct RenderTarget::Impl
    {
        friend class RenderTarget;

    public:
        explicit Impl(std::string name,
                      const unsigned location,
                      const RenderTargetType type,
                      const Format format,
                      const bool clear_buffer,
                      const math::Vector4<float>& clear_values,
                      const bool clear_stencil,
                      const bool is_volatile,
                      const BlendState& blend_state)
            : m_type(type), m_format(format), m_clearBuffer(clear_buffer), m_clearStencil(clear_stencil), m_volatile(is_volatile), m_clearValues(clear_values),
              m_location(location), m_blendState(blend_state), m_name(std::move(name)) {}

    private:
        RenderTargetType m_type = RenderTargetType::Color;
        Format m_format = Format::None;
        bool m_clearBuffer = false, m_clearStencil = false, m_volatile = false;
        math::Vector4<float> m_clearValues = {0.0f, 0.0f, 0.0f, 0.0f};
        unsigned int m_location = 0;
        BlendState m_blendState;
        std::string m_name;
    };

    RenderTarget::RenderTarget() noexcept = default;

    RenderTarget::RenderTarget(unsigned location,
                               RenderTargetType type,
                               Format format,
                               bool clear_buffer,
                               const math::Vector4<float>& clear_values,
                               bool clear_stencil,
                               bool is_volatile,
                               const BlendState& blend_state)
        : m_impl(std::make_unique<Impl>("", location, type, format, clear_buffer, clear_values, clear_stencil, is_volatile, blend_state)) {}

    RenderTarget::RenderTarget(const std::string& name,
                               unsigned location,
                               RenderTargetType type,
                               Format format,
                               bool clear_buffer,
                               const math::Vector4<float>& clear_values,
                               bool clear_stencil,
                               bool is_volatile,
                               const BlendState& blend_state)
        : m_impl(std::make_unique<Impl>(name, location, type, format, clear_buffer, clear_values, clear_stencil, is_volatile, blend_state)) {}

    RenderTarget::~RenderTarget() noexcept = default;

    RenderTarget::RenderTarget(const RenderTarget& other)
        : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

    RenderTarget::RenderTarget(RenderTarget&& other) noexcept
        : m_impl(std::move(other.m_impl)) {}

    RenderTarget& RenderTarget::operator=(const RenderTarget& other)
    {
        if (this == &other)
            return *this;
        m_impl = std::make_unique<Impl>(*other.m_impl);
        return *this;
    }

    RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_impl = std::move(other.m_impl);
        return *this;
    }

    std::string RenderTarget::name() const noexcept
    {
        return m_impl->m_name;
    }

    unsigned RenderTarget::location() const noexcept
    {
        return m_impl->m_location;
    }

    RenderTargetType RenderTarget::type() const noexcept
    {
        return m_impl->m_type;
    }

    Format RenderTarget::format() const noexcept
    {
        return m_impl->m_format;
    }

    bool RenderTarget::clearBuffer() const noexcept
    {
        return m_impl->m_clearBuffer;
    }

    bool RenderTarget::clearStencil() const noexcept
    {
        return m_impl->m_clearStencil;
    }

    math::Vector4<float> RenderTarget::clearValues() const noexcept
    {
        return m_impl->m_clearValues;
    }

    bool RenderTarget::isVolatile() const noexcept
    {
        return m_impl->m_volatile;
    }

    IRenderTarget::BlendState RenderTarget::blendState() const noexcept
    {
        return m_impl->m_blendState;
    }
}
