#include "spark/engine/Scene.h"

#include "spark/log/Logger.h"

namespace spark::engine
{
    Scene::Scene(GameObject* scene_root)
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

    GameObject* Scene::getRoot()
    {
        return m_root;
    }

    void Scene::onLoad()
    {
        if (m_isLoaded)
            return;

        SPARK_CORE_INFO("Loading scene {}", getUuid().str());
        getRoot()->traverse([](details::AbstractGameObject<GameObject>* object)
        {
            object->onSpawn();
        });
        m_isLoaded = true;
    }

    void Scene::onUpdate(float dt)
    {
        getRoot()->traverse([&dt](details::AbstractGameObject<GameObject>* object)
        {
            object->onUpdate(dt);
        });
    }

    void Scene::onUnload()
    {
        if (!m_isLoaded)
            return;

        SPARK_CORE_INFO("Unloading scene {}", getUuid().str());
        getRoot()->traverse([](details::AbstractGameObject<GameObject>* object)
        {
            object->onDestroyed();
        });
        m_isLoaded = false;
        SPARK_CORE_INFO("Scene {} unloaded", getUuid().str());
    }
}
