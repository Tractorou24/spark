#pragma once

#include "spark/core/Export.h"
#include "spark/core/Scene.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace spark::core
{
    /**
     * \brief A class used to manage scenes in the game.
     */
    class SPARK_CORE_EXPORT SceneManager final
    {
    public:
        /**
         * \brief Registers a new scene. If a scene with the same name is already registered, an exception is thrown.
         * \param name A string representing the name of the scene.
         * \param scene A unique pointer to the scene to register.
         */
        static void RegisterScene(std::string name, std::unique_ptr<core::Scene> scene);

        /**
         * \brief Unregister a scene. If the scene is not registered, an exception is thrown.
         * \param name A string representing the name of the scene to unregister.
         */
        static void UnregisterScene(const std::string& name);

        /**
         * \brief Loads a scene by its name into the current \link spark::core::Application \endlink. If the scene is not registered, an exception is thrown.
         * \param name A string representing the name of the scene to load.
         */
        static void LoadScene(const std::string& name);

        /**
         * \brief Gets a scene by its name. If the scene is not registered, an exception is thrown.
         * \param name A string representing the name of the scene to get.
         * \param fail A boolean indicating if an exception should be thrown if the scene is not registered.
         * \return A unique pointer to the scene.
         */
        static std::shared_ptr<core::Scene> Scene(const std::string& name, bool fail = true);

        /**
         * \brief Unregister all scenes.
         */
        static void UnregisterAllScenes();

    private:
        static std::unordered_map<std::string, std::shared_ptr<core::Scene>> s_scenes;
    };
}
