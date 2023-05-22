#include "spark/core/SceneManager.h"
#include "spark/core/Application.h"

#include <ranges>

namespace spark::core
{
    std::unordered_map<std::string, std::shared_ptr<engine::Scene>> SceneManager::s_scenes;

    void SceneManager::RegisterScene(std::string name, std::unique_ptr<engine::Scene> scene)
    {
        if (s_scenes.contains(name))
            SPARK_BASE_THROW_EXCEPTION(base::UnknownException, "Can't register 2 scenes with the same name");
        s_scenes.emplace(std::move(name), std::move(scene));
    }

    void SceneManager::UnregisterScene(const std::string& name)
    {
        if (!s_scenes.contains(name))
            SPARK_BASE_THROW_EXCEPTION(base::UnknownException, "Can't unregister a not registered scene");
        s_scenes.erase(name);
    }

    void SceneManager::LoadScene(const std::string& name)
    {
        // TODO: Allow a loaded scene to be loaded multiple times without seeing the modifications made in the previous runtime.
        if (!s_scenes.contains(name))
            SPARK_BASE_THROW_EXCEPTION(base::UnknownException, "Can't load a not registered scene");
        Application::Instance()->setScene(s_scenes.at(name));
    }

    std::shared_ptr<engine::Scene> SceneManager::GetScene(const std::string& name, const bool fail)
    {
        if (!s_scenes.contains(name))
        {
            if (fail)
                SPARK_BASE_THROW_EXCEPTION(base::UnknownException, "Can't get a not registered scene");
            return nullptr;
        }
        return s_scenes.at(name);
    }

    void SceneManager::UnregisterAllScenes()
    {
        s_scenes.clear();
    }
}
