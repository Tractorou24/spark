#include "spark/render/Rasterizer.h"

namespace spark::render
{
    struct Rasterizer::Impl
    {
        friend class Rasterizer;

    public:
        explicit Impl(const PolygonMode polygon_mode,
                      const CullMode cull_mode,
                      const CullOrder cull_order,
                      const float line_width,
                      DepthStencilState depth_stencil_state) noexcept
            : m_polygonMode(polygon_mode)
            , m_cullMode(cull_mode)
            , m_cullOrder(cull_order)
            , m_lineWidth(line_width)
            , m_depthStencilState(std::move(depth_stencil_state)) {}

    private:
        PolygonMode m_polygonMode = PolygonMode::Solid;
        CullMode m_cullMode = CullMode::BackFaces;
        CullOrder m_cullOrder = CullOrder::CounterClockWise;
        float m_lineWidth = 1.0f;
        DepthStencilState m_depthStencilState;
    };

    Rasterizer::Rasterizer(PolygonMode polygon_mode, CullMode cull_mode, CullOrder cull_order, float line_width, const DepthStencilState& depth_stencil_state) noexcept
        : m_impl(std::make_unique<Impl>(polygon_mode, cull_mode, cull_order, line_width, depth_stencil_state)) {}

    Rasterizer::~Rasterizer() noexcept = default;

    Rasterizer::Rasterizer(const Rasterizer& other)
        : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

    Rasterizer::Rasterizer(Rasterizer&& other) noexcept
        : m_impl(std::move(other.m_impl)) {}

    Rasterizer& Rasterizer::operator=(const Rasterizer& other)
    {
        if (this->m_impl == other.m_impl)
            return *this;

        m_impl->m_polygonMode = other.m_impl->m_polygonMode;
        m_impl->m_cullMode = other.m_impl->m_cullMode;
        m_impl->m_cullOrder = other.m_impl->m_cullOrder;
        m_impl->m_lineWidth = other.m_impl->m_lineWidth;
        m_impl->m_depthStencilState = other.m_impl->m_depthStencilState;
        return *this;
    }

    Rasterizer& Rasterizer::operator=(Rasterizer&& other) noexcept
    {
        if (this->m_impl == other.m_impl)
            return *this;

        m_impl = std::move(other.m_impl);
        return *this;
    }

    PolygonMode Rasterizer::polygonMode() const noexcept
    {
        return m_impl->m_polygonMode;
    }

    CullMode Rasterizer::cullMode() const noexcept
    {
        return m_impl->m_cullMode;
    }

    CullOrder Rasterizer::cullOrder() const noexcept
    {
        return m_impl->m_cullOrder;
    }

    float Rasterizer::lineWidth() const noexcept
    {
        return m_impl->m_lineWidth;
    }

    const DepthStencilState& Rasterizer::depthStencilState() const noexcept
    {
        return m_impl->m_depthStencilState;
    }

    float& Rasterizer::lineWidth() noexcept
    {
        return m_impl->m_lineWidth;
    }
}
