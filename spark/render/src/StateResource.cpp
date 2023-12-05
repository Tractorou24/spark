#include "spark/render/StateResource.h"

namespace spark::render
{
    struct StateResource::Impl
    {
        friend class StateResource;

    public:
        explicit Impl(const std::string_view name)
            : m_name(name) {}

    private:
        std::string m_name;
    };

    StateResource::StateResource(std::string_view name) noexcept
        : m_impl(std::make_unique<Impl>(name)) {}

    StateResource::~StateResource() noexcept = default;

    StateResource::StateResource(StateResource&& other) noexcept = default;
    StateResource& StateResource::operator=(StateResource&& other) noexcept = default;

    const std::string& StateResource::name() const noexcept
    {
        return m_impl->m_name;
    }

    StateResource::StateResource() noexcept
        : m_impl(std::make_unique<Impl>("")) {}

    std::string& StateResource::name() noexcept
    {
        return m_impl->m_name;
    }
}
