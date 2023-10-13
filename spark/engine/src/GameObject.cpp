#include "spark/engine/GameObject.h"
#include "spark/engine/components/Transform.h"

#include <ranges>

namespace spark::engine
{
    void GameObject::Destroy(GameObject* object)
    {
        object->traverse([&object](GameObject* obj)
        {
            // Ensure we don't destroy the object we're currently destroying, results in an infinite loop otherwise
            if (obj == object)
                return;

            Destroy(obj);
        });
        object->onDestroyed();
        delete object;
    }

    GameObject* GameObject::FindById(GameObject* root, const lib::Uuid& uuid)
    {
        GameObject* found = nullptr;
        root->traverse([&uuid, &found](GameObject* obj)
        {
            if (obj->getUuid() == uuid)
            {
                if (found != nullptr)
                    throw base::UnknownException("Found multiple GameObjects with the same UUID!");
                found = obj;
            }
        });
        return found;
    }

    GameObject* GameObject::FindByName(GameObject* root, const std::string& name)
    {
        GameObject* found = nullptr;
        root->traverse([&name, &found](GameObject* obj)
        {
            if (obj->getName() == name)
            {
                if (found != nullptr)
                    throw base::UnknownException("Found multiple GameObjects with the same name!");
                found = obj;
            }
        });
        return found;
    }

    GameObject::GameObject(std::string name, GameObject* parent)
        : AbstractGameObject(parent), m_name(std::move(name))
    {
        addComponent<components::Transform>();
    }

    GameObject::~GameObject()
    {
        traverse([this](GameObject* obj)
        {
            // Ensure we don't destroy the object we're currently destroying, results in an infinite loop otherwise
            if (obj == this)
                return;
            delete obj;
        });
    }

    const lib::Uuid& GameObject::getUuid() const
    {
        return m_uuid;
    }

    const std::string& GameObject::getName() const
    {
        return m_name;
    }

    components::Transform* GameObject::getTransform() const
    {
        return getComponent<components::Transform>();
    }

    void GameObject::addComponent(Component* component, bool managed)
    {
        if (m_components.contains(&component->getRttiInstance()))
            throw base::BadArgumentException("Unable to add the same component twice!");

        m_components.insert({&component->getRttiInstance(), {component, managed}});
        if (m_initialized)
            component->onAttach();
    }

    void GameObject::removeComponent(Component* component)
    {
        if (!m_components.contains(&component->getRttiInstance()))
            throw base::BadArgumentException("Unable to remove a non-existing component!");

        m_components.erase(&component->getRttiInstance());
        component->onDetach();
    }

    std::vector<Component*> GameObject::getComponents() const
    {
        std::vector<Component*> components;
        for (const auto& component : m_components | std::views::values | std::views::keys)
            components.push_back(component);
        return components;
    }
}
