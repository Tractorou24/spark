#pragma once

#include <algorithm>
#include <ranges>

namespace spark::engine::details
{
    template<typename Impl>
    AbstractGameObject<Impl>::AbstractGameObject(GameObject* parent)
        : Composite(parent)
    {
    }

    template<typename Impl>
    AbstractGameObject<Impl>::~AbstractGameObject()
    {
        // Ensure onDestroyed() was called
        SPARK_CORE_ASSERT(!m_initialized)

        for (const auto& [component, managed] : m_components | std::views::values)
            if (managed)
                delete component;
    }

    template <typename Impl>
    void AbstractGameObject<Impl>::onSpawn()
    {
        SPARK_CORE_ASSERT(!m_initialized)
        static_cast<Impl*>(this)->onSpawn();
        std::ranges::for_each(m_components | std::views::values | std::views::keys,
                              [](Component* component)
                              {
                                  component->onAttach();
                              });
        m_initialized = true;
    }

    template <typename Impl>
    void AbstractGameObject<Impl>::onUpdate(float dt)
    {
        static_cast<Impl*>(this)->onUpdate(dt);
        std::ranges::for_each(m_components | std::views::values | std::views::keys,
                              [&dt](Component* component)
                              {
                                  component->onUpdate(dt);
                              });
    }

    template <typename Impl>
    void AbstractGameObject<Impl>::onDestroyed()
    {
        SPARK_CORE_ASSERT(m_initialized)
        std::ranges::for_each(m_components | std::views::values | std::views::keys,
                              [](Component* component)
                              {
                                  component->onDetach();
                              });
        static_cast<Impl*>(this)->onDestroyed();
        m_initialized = false;
    }
}
