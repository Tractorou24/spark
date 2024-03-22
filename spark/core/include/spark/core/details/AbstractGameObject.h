#pragma once

#include "experimental/ser/SerializerScheme.h"
#include "spark/patterns/Composite.h"

namespace spark::core
{
    class GameObject;
}

namespace spark::core::details
{
    template <typename Impl>
    struct GameObjectDeleter;

    /**
     * \brief A CRTP class to implement a GameObject. It is used to wrap the onSpawn, onUpdate and onDestroyed methods to include custom code around user implementation.
     * \tparam Impl The implementation of the GameObject.
     */
    template <typename Impl>
    class AbstractGameObject : public patterns::Composite<GameObject, GameObjectDeleter>
    {
        friend class spark::core::GameObject;
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
        explicit AbstractGameObject(GameObject* parent = nullptr);
        ~AbstractGameObject() override;

        AbstractGameObject(const AbstractGameObject& other) = delete;
        AbstractGameObject(AbstractGameObject&& other) noexcept = default;
        AbstractGameObject& operator=(const AbstractGameObject& other) = delete;
        AbstractGameObject& operator=(AbstractGameObject&& other) noexcept = default;

        /**
         * \brief Method calling the corresponding method on the implementation, the children and components.
         */
        void onSpawn();

        /**
         * \brief Method calling the corresponding method on the implementation, the children and components.
         * \param dt The time in seconds since the last frame.
         */
        void onUpdate(float dt);

        /**
         * \brief Method calling the corresponding method on the implementation, the children and components.
         */
        void onDestroyed();

    private:
        bool m_initialized = false;
        std::unordered_map<rtti::RttiBase*, std::pair<Component*, bool>> m_components;
    };

    /**
     * \brief Deleter used to call the onDestroyed method on the implementation of a GameObject.
     * \tparam Impl The implementation of the GameObject.
     */
    template <typename Impl>
    struct GameObjectDeleter
    {
        inline static std::vector<AbstractGameObject<GameObject>*> objectsToDestroy;

        static void DeleteMarkedObjects()
        {
            for (std::size_t i = 0; i < objectsToDestroy.size(); i++)
                delete objectsToDestroy[i];
            objectsToDestroy.clear();
        }

        void operator()(Impl* ptr) const
        {
            static_cast<AbstractGameObject<GameObject>*>(ptr)->onDestroyed();
            objectsToDestroy.push_back(ptr);
        }
    };
}

#include "spark/core/impl/AbstractGameObject.h"
