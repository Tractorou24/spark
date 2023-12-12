#include "spark/render/Viewport.h"

namespace spark::render
{
    struct Viewport::Impl
    {
        friend class Viewport;

    public:
        explicit Impl(const math::Rectangle<float>& rectangle, const float min_depth, const float max_depth) noexcept
            : m_rectangle(rectangle), m_minDepth(min_depth), m_maxDepth(max_depth) {}

    private:
        math::Rectangle<float> m_rectangle = {};
        float m_minDepth = 0.f;
        float m_maxDepth = 0.f;
    };

    Viewport::Viewport(const math::Rectangle<float>& rectangle, float min_depth, float max_depth) noexcept
        : m_impl(std::make_unique<Impl>(rectangle, min_depth, max_depth)) {}

    Viewport::~Viewport() noexcept = default;

    Viewport::Viewport(Viewport&& other) noexcept = default;
    Viewport& Viewport::operator=(Viewport&& other) noexcept = default;

    math::Rectangle<float> Viewport::rectangle() const noexcept
    {
        return m_impl->m_rectangle;
    }

    void Viewport::setRectangle(const math::Rectangle<float>& rectangle) noexcept
    {
        m_impl->m_rectangle = rectangle;
    }

    float Viewport::minDepth() const noexcept
    {
        return m_impl->m_minDepth;
    }

    void Viewport::setMinDepth(const float min_depth) noexcept
    {
        m_impl->m_minDepth = min_depth;
    }

    float Viewport::maxDepth() const noexcept
    {
        return m_impl->m_maxDepth;
    }

    void Viewport::setMaxDepth(const float max_depth) noexcept
    {
        m_impl->m_maxDepth = max_depth;
    }
}
