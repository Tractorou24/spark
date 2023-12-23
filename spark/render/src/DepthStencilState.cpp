#include "spark/render/DepthStencilState.h"

namespace spark::render
{
    struct DepthStencilState::Impl
    {
        friend class DepthStencilState;

    public:
        explicit Impl(const DepthState& depth_state, const DepthBias& depth_bias, const StencilState& stencil_state) noexcept
            : m_depthState(depth_state), m_depthBias(depth_bias), m_stencilState(stencil_state) {}

    private:
        DepthState m_depthState;
        DepthBias m_depthBias;
        StencilState m_stencilState;
    };

    DepthStencilState::DepthStencilState() noexcept
        : m_impl(std::make_unique<Impl>(DepthState(), DepthBias(), StencilState())) {}

    DepthStencilState::DepthStencilState(const DepthState& depth_state, const DepthBias& depth_bias, const StencilState& stencil_state) noexcept
        : m_impl(std::make_unique<Impl>(depth_state, depth_bias, stencil_state)) {}

    DepthStencilState::~DepthStencilState() noexcept = default;

    DepthStencilState::DepthStencilState(const DepthStencilState& other)
        : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

    DepthStencilState::DepthStencilState(DepthStencilState&& other) noexcept
        : m_impl(std::move(other.m_impl)) {}

    DepthStencilState& DepthStencilState::operator=(const DepthStencilState& other)
    {
        if (this->m_impl == other.m_impl)
            return *this;

        m_impl->m_depthState = other.m_impl->m_depthState;
        m_impl->m_depthBias = other.m_impl->m_depthBias;
        m_impl->m_stencilState = other.m_impl->m_stencilState;
        return *this;
    }

    DepthStencilState& DepthStencilState::operator=(DepthStencilState&& other) noexcept
    {
        if (this->m_impl == other.m_impl)
            return *this;

        m_impl = std::move(other.m_impl);
        return *this;
    }

    DepthStencilState::DepthState& DepthStencilState::depthState() const noexcept
    {
        return m_impl->m_depthState;
    }

    DepthStencilState::DepthBias& DepthStencilState::depthBias() const noexcept
    {
        return m_impl->m_depthBias;
    }

    DepthStencilState::StencilState& DepthStencilState::stencilState() const noexcept
    {
        return m_impl->m_stencilState;
    }
}
