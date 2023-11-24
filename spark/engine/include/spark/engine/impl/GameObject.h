#pragma once

#include <algorithm>

namespace spark::engine
{
    template <typename T, typename... Args> requires std::is_base_of_v<GameObject, T>
    T* GameObject::Instantiate(std::string name, GameObject* parent, const Args&... args)
    {
        T* object = new T(std::move(name), parent, args...);
        if (parent)
            if (parent->m_initialized)
                object->onSpawn();
        return object;
    }

    template <typename T, typename... Args> requires std::is_base_of_v<Component, T>
    void GameObject::addComponent(const Args&... args)
    {
        addComponent(new T(this, args...), true);
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    void GameObject::removeComponent()
    {
        removeComponent(component<T>());
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    bool GameObject::hasComponent() const
    {
        return component<T>() != nullptr;
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    std::vector<T*> GameObject::componentsInChildren() const
    {
        std::vector<T*> components;
        for (const auto* child : children())
        {
            // On the object
            auto* component = child->component<T>();
            if (component != nullptr)
                components.push_back(component);

            // On the children
            auto child_components = child->componentsInChildren<T>();
            std::copy(child_components.begin(), child_components.end(), std::back_inserter(components));
        }
        return components;
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    T* GameObject::component() const
    {
        const auto it = m_components.find(&T::getClassRtti());
        if (it == m_components.cend())
            return nullptr;
        return static_cast<T*>(it->second.first);
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    T* GameObject::componentInChildren() const
    {
        for (const auto* child : children())
        {
            auto* component = child->component<T>();
            if (component != nullptr)
                return component;
        }
        return nullptr;
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    T* GameObject::componentInParent() const
    {
        return parent()->component<T>();
    }
}
