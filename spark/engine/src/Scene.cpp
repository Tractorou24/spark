#include "spark/engine/Scene.h"

#include "spark/log/Logger.h"
#include "spark/patterns/Traverser.h"

namespace spark::engine
{
    Scene::Scene(GameObject* scene_root)
        : m_root(scene_root) {}

    Scene::~Scene()
    {
        onUnload();
        GameObject::Destroy(m_root);
    }

    const lib::Uuid& Scene::uuid() const
    {
        return m_uuid;
    }

    GameObject* Scene::root()
    {
        return m_root;
    }

    void Scene::onLoad()
    {
        if (m_isLoaded)
            return;

        log::info("Loading scene {}", uuid().str());

        auto traverser = spark::patterns::make_traverser<GameObject>([](auto* object)
        {
            static_cast<details::AbstractGameObject<GameObject>*>(object)->onSpawn();
        });
        spark::patterns::traverse_tree(m_root, traverser);

        m_isLoaded = true;
    }

    void Scene::onUpdate(float dt)
    {
        auto traverser = spark::patterns::make_traverser<GameObject>([&dt](auto* object)
        {
            static_cast<details::AbstractGameObject<GameObject>*>(object)->onUpdate(dt);
        });
        spark::patterns::traverse_tree(m_root, traverser);
    }

    void Scene::onRender()
    {
        auto traverser = spark::patterns::make_traverser<GameObject>([](const GameObject* object)
        {
            for (const auto* component : object->components())
                component->render();
        });
        spark::patterns::traverse_tree(m_root, traverser);
    }

    void Scene::onUnload()
    {
        if (!m_isLoaded)
            return;

        log::info("Unloading scene {}", uuid().str());

        auto traverser = spark::patterns::make_traverser<GameObject>([](auto* object)
        {
            static_cast<details::AbstractGameObject<GameObject>*>(object)->onDestroyed();
        });
        spark::patterns::traverse_tree(m_root, traverser);

        m_isLoaded = false;
        log::info("Scene {} unloaded", uuid().str());
    }
}
