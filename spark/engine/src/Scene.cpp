#include "spark/engine/Scene.h"

#include "spark/log/Logger.h"

namespace spark::engine
{
    Scene::Scene(engine::GameObject* scene_root)
        : m_root(scene_root) {}

    Scene::~Scene()
    {
        onUnload();
        delete m_root;
    }

    const lib::Uuid& Scene::getUuid() const
    {
        return m_uuid;
    }

    engine::GameObject* Scene::getRoot()
    {
        return m_root;
    }

    void Scene::onLoad()
    {
        if (m_isLoaded)
            return;

        SPARK_CORE_INFO("Loading scene {}", getUuid().str());
        getRoot()->traverse([](engine::GameObject* object)
        {
            object->onSpawn();
        });
        m_isLoaded = true;
    }

    void Scene::onUpdate(float dt)
    {
        getRoot()->traverse([&dt](engine::GameObject* object)
        {
            object->onUpdate(dt);
        });
    }

    void Scene::onUnload()
    {
        if (!m_isLoaded)
            return;

        SPARK_CORE_INFO("Unloading scene {}", getUuid().str());
        getRoot()->traverse([](engine::GameObject* object)
        {
            object->onDestroyed();
        });
        m_isLoaded = false;
        SPARK_CORE_INFO("Scene {} unloaded", getUuid().str());
    }
}
