#include "spark/engine/GameObject.h"
#include "spark/engine/components/Transform.h"

#include "spark/patterns/Traverser.h"

#include <ranges>

namespace spark::engine
{
    void GameObject::Destroy(GameObject* object)
    {
        details::GameObjectDeleter<GameObject>()(object);
    }

    GameObject* GameObject::FindById(GameObject* root, const lib::Uuid& uuid)
    {
        GameObject* found = nullptr;
        auto traverser = patterns::make_traverser<GameObject>([&uuid, &found](GameObject* obj)
        {
            if (obj->getUuid() == uuid)
            {
                if (found != nullptr)
                    throw base::UnknownException("Found multiple GameObjects with the same UUID!");
                found = obj;
            }
        });
        patterns::traverse_tree(root, traverser);
        return found;
    }

    GameObject* GameObject::FindByName(GameObject* root, const std::string& name)
    {
        GameObject* found = nullptr;
        auto traverser = patterns::make_traverser<GameObject>([&name, &found](GameObject* obj)
        {
            if (obj->getName() == name)
            {
                if (found != nullptr)
                    throw base::UnknownException("Found multiple GameObjects with the same name!");
                found = obj;
            }
        });
        patterns::traverse_tree(root, traverser);
        return found;
    }

    GameObject::GameObject(std::string name, GameObject* parent)
        : AbstractGameObject(parent), m_name(std::move(name))
    {
        addComponent<components::Transform>();
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
