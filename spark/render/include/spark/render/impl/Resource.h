#pragma once

#include <algorithm>

namespace spark::render
{
    template <typename Handle>
    Resource<Handle>::Resource(Handle handle) noexcept
        : m_handle(std::move(handle)) {}

    template <typename Handle>
    const Handle& Resource<Handle>::handle() const
    {
        return m_handle;
    }

    template <typename Handle>
    Handle& Resource<Handle>::handle()
    {
        return m_handle;
    }
}
