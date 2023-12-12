#include "spark/render/Scissor.h"

namespace spark::render
{
    struct Scissor::Impl
    {
        friend class Scissor;

    public:
        explicit Impl(const math::Rectangle<float>& rectangle) noexcept
            : m_rectangle(rectangle) {}

    private:
        math::Rectangle<float> m_rectangle = {};
    };

    Scissor::Scissor(const math::Rectangle<float>& rectangle) noexcept
        : m_impl(std::make_unique<Impl>(rectangle)) {}

    Scissor::~Scissor() noexcept = default;

    math::Rectangle<float> Scissor::rectangle() const noexcept
    {
        return m_impl->m_rectangle;
    }

    void Scissor::setRectangle(const math::Rectangle<float>& rectangle) noexcept
    {
        m_impl->m_rectangle = rectangle;
    }
}
