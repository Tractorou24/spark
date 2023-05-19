#pragma once

#include <algorithm>

namespace spark::engine
{
    template <typename T, typename... Args> requires std::is_base_of_v<GameObject, T>
    T* GameObject::Instantiate(std::string name, engine::GameObject* parent, const Args&... args)
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
        removeComponent(getComponent<T>());
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    bool GameObject::hasComponent() const
    {
        return getComponent<T>() != nullptr;
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    std::vector<T*> GameObject::getComponentsInChildren() const
    {
        std::vector<T*> components;
        for (const auto* child : getChildren())
        {
            // On the object
            auto* component = child->getComponent<T>();
            if (component != nullptr)
                components.push_back(component);

            // On the children
            auto child_components = child->getComponentsInChildren<T>();
            std::copy(child_components.begin(), child_components.end(), std::back_inserter(components));
        }
        return components;
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    T* GameObject::getComponent() const
    {
        const auto it = m_components.find(&T::getClassRtti());
        if (it == m_components.cend())
            return nullptr;
        return static_cast<T*>(it->second.first);
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    T* GameObject::getComponentInChildren() const
    {
        const auto& children = getChildren();
        for (const auto* child : children)
        {
            auto* component = child->getComponent<T>();
            if (component != nullptr)
                return component;
        }
        return nullptr;
    }

    template <typename T> requires std::is_base_of_v<Component, T>
    T* GameObject::getComponentInParent() const
    {
        return getParent()->getComponent<T>();
    }
}
