#pragma once

#include "spark/engine/Component.h"
#include "spark/engine/Export.h"
#include "spark/engine/details/AbstractGameObject.h"

#include "spark/base/Macros.h"
#include "spark/rtti/HasRtti.h"

#include <vector>

namespace spark::engine::components
{
    class Transform;
}

namespace spark::engine
{
    /**
     * \brief A GameObject is any object in the game. It contains a list of components that provides functionality to the GameObject.
     *
     * GameObjects can be parented to other GameObjects. When a GameObject is destroyed, all its children are destroyed as well with their components.
     */
    class SPARK_ENGINE_EXPORT GameObject : public rtti::HasRtti, public details::AbstractGameObject<GameObject>
    {
        DECLARE_SPARK_RTTI(GameObject)

    public:
        /**
         * \brief Instantiates a new GameObject
         * \tparam T The type of GameObject to instantiate
         * \param name The name of the GameObject
         * \param parent The parent GameObject in the scene
         * \param args The arguments to pass to the constructor of the GameObject
         * \return A pointer to the instantiated GameObject
         *
         * \details This method should only be called during the game runtime, not in editor. Use \link GameObject::GameObject \endlink for this.
         */
        template <typename T = GameObject, typename... Args> requires std::is_base_of_v<GameObject, T>
        static T* Instantiate(std::string name, GameObject* parent, const Args&... args);

        /**
         * \brief Destroys the current GameObject and all its children from the current scene.
         */
        static void Destroy(GameObject* object);

        /**
         * \brief Finds a GameObject by its UUID.
         * \param root The root GameObject to start the search from.
         * \param uuid The UUID of the GameObject to find.
         * \return A pointer to the GameObject if found, nullptr otherwise.
         */
        static GameObject* FindById(GameObject* root, const lib::Uuid& uuid);

        /**
         * \brief Finds a GameObject by its name.
         * \param root The root GameObject to start the search from.
         * \param name The name of the GameObject to find.
         * \return A pointer to the GameObject if found, nullptr otherwise.
         */
        static GameObject* FindByName(GameObject* root, const std::string& name);

    public:
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) noexcept = default;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) noexcept = default;

        /**
         * \brief Gets the UUID of the GameObject.
         * \return A const reference to the UUID of this GameObject.
         */
        [[nodiscard]] const lib::Uuid& uuid() const;

        /**
         * \brief Gets the name of the GameObject.
         * \return A const reference to the name of this GameObject.
         */
        [[nodiscard]] const std::string& name() const;

        /**
         * \brief Gets the transform component of the GameObject.
         * \return A pointer to the transform component of the GameObject.
         */
        [[nodiscard]] components::Transform* transform() const;

        /**
         * \brief Adds a component to the GameObject.
         * \param component A pointer to the component to add.
         * \param managed A boolean indicating if the component is managed by the GameObject. If true, the component will be destroyed when the GameObject is destroyed.
         */
        void addComponent(Component* component, bool managed = false);

        /**
         * \brief Adds a component of type T to the GameObject.
         * \tparam T The type of component to add.
         * \tparam Args The types of the arguments to pass to the constructor of the component.
         * \param args The arguments to pass to the constructor of the component.
         */
        template <typename T, typename... Args> requires std::is_base_of_v<Component, T>
        void addComponent(const Args&... args);

        /**
         * \brief Removes a component from the GameObject.
         * \param component A pointer to the component to remove.
         */
        void removeComponent(Component* component);

        /**
         * \brief Removes a component of the given type from the GameObject.
         * \tparam T The type of component to remove.
         */
        template <typename T> requires std::is_base_of_v<Component, T>
        void removeComponent();

        /**
         * \brief Checks if the GameObject has a component of type T.
         * \tparam T The type of component to check.
         * \return A boolean indicating if the GameObject has a component of type T.
         */
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] bool hasComponent() const;

        /**
         * \brief Gets all the components for this GameObject.
         * \return A vector of pointers to the components.
         */
        [[nodiscard]] std::vector<Component*> components() const;

        /**
         * \brief Gets a list of components of type T in any direct child of the GameObject.
         * \tparam T The type of component to get.
         * \return A vector of pointers to the component of type T, or an empty vector if no component was found.
         */
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] std::vector<T*> componentsInChildren() const;

        /**
         * \brief Gets a component of type T.
         * \tparam T The type of component to get.
         * \return A pointer to the component of type T, or nullptr if no component is not found.
         */
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] T* component() const;

        /**
         * \brief Gets a component of type T in any direct child of the GameObject.
         * \tparam T The type of component to get.
         * \return A pointer to the component of type T, or nullptr if no component is not found.
         */
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] T* componentInChildren() const;

        /**
         * \brief Gets a component of type T in any direct parent of the GameObject.
         * \tparam T The type of component to get.
         * \return A pointer to the component of type T, or nullptr if no component is not found.
         */
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]] T* componentInParent() const;

        /**
         * \brief Method called when the GameObject is spawned in the scene.
         */
        virtual void onSpawn() {}

        /**
         * \brief Method called on every frame.
         * \param dt The time in seconds since the last frame.
         */
        virtual void onUpdate(float dt) { SPARK_UNUSED(dt); }

        /**
         * \brief Method called when the GameObject is destroyed.
         */
        virtual void onDestroyed() {}

    public:
        /**
         * \brief A boolean indicating if the GameObject is shown in the scene.
         */
        bool isShown = true;

    protected:
        /**
         * \brief Instantiates a new GameObject.
         * \param name The name of the GameObject.
         * \param parent The parent of the GameObject. If nullptr, the GameObject is parented to the root GameObject.
         *
         * \details Use \link GameObject::Instantiate \endlink to instantiate a GameObject externally.
         */
        explicit GameObject(std::string name, GameObject* parent = nullptr);

    private:
        lib::Uuid m_uuid;
        std::string m_name;
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::GameObject)

#include "spark/engine/impl/GameObject.h"

namespace spark::core
{
    template <typename T>
    struct Render2D;

    template <>
    struct Render2D<engine::GameObject>
    {
        static void exec(const engine::GameObject& game_object)
        {
            if (!game_object.isShown)
                return;

            for (const auto* component : game_object.components())
                component->render();
            for (const auto* child : game_object.getChildren())
                exec(*child);
        }
    };
}
