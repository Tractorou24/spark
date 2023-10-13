#pragma once

#include "spark/patterns/Composite.h"

namespace spark::engine
{
    class GameObject;
}

namespace spark::engine::details
{
    /**
     * \brief A CRTP class to implement a GameObject. It is used to wrap the onSpawn, onUpdate and onDestroyed methods to include custom code around user implementation.
     * \tparam Impl The implementation of the GameObject.
     */
    template <typename Impl>
    class AbstractGameObject : public patterns::Composite<GameObject>
    {
        friend class spark::engine::GameObject;

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
}

#include "spark/engine/impl/AbstractGameObject.h"