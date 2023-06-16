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
        object->getParent()->remove(object);
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
                    SPARK_BASE_THROW_EXCEPTION(base::UnknownException, "Found multiple GameObjects with the same UUID!");
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
                    SPARK_BASE_THROW_EXCEPTION(base::UnknownException, "Found multiple GameObjects with the same name!");
                found = obj;
            }
        });
        return found;
    }

    GameObject::GameObject(std::string name, GameObject* parent)
        : Composite<GameObject>(parent), m_name(std::move(name))
    {
        addComponent<components::Transform>();
    }

    GameObject::~GameObject()
    {
        // Ensure onDestroyed() was called
        SPARK_CORE_ASSERT(!m_initialized)

        for (const auto& [component, managed] : m_components | std::views::values)
            if (managed)
                delete component;

        traverse([this](GameObject* obj)
        {
            // Ensure we don't destroy the object we're currently destroying, results in an infinite loop otherwise
            if (obj == this)
                return;

            obj->getParent()->remove(obj);
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
            SPARK_BASE_THROW_EXCEPTION(base::BadArgumentException, "Unable to add the same component twice!");

        m_components.insert({&component->getRttiInstance(), {component, managed}});
        if (m_initialized)
            component->onAttach();
    }

    void GameObject::removeComponent(Component* component)
    {
        if (!m_components.contains(&component->getRttiInstance()))
            SPARK_BASE_THROW_EXCEPTION(base::BadArgumentException, "Unable to remove a non-existing component!");

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

    void GameObject::onSpawn()
    {
        SPARK_CORE_ASSERT(!m_initialized)
        std::ranges::for_each(m_components | std::views::values | std::views::keys,
                              [](Component* component)
                              {
                                  component->onAttach();
                              });
        m_initialized = true;
    }

    void GameObject::onUpdate(float dt)
    {
        std::ranges::for_each(m_components | std::views::values | std::views::keys,
                              [&dt](Component* component)
                              {
                                  component->onUpdate(dt);
                              });
    }

    void GameObject::onDestroyed()
    {
        SPARK_CORE_ASSERT(m_initialized)
        std::ranges::for_each(m_components | std::views::values | std::views::keys,
                              [](Component* component)
                              {
                                  component->onDetach();
                              });
        m_initialized = false;
    }
}
