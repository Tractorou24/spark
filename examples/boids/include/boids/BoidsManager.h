#pragma once

#include "boids/Bird.h"

#include "spark/core/GameObject.h"

namespace boids
{
    /**
     * \brief The manager of the boids simulation. Holds the global settings.
     */
    class BoidsManager final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(BoidsManager, GameObject)

    public:
        /// \brief The number of boids (birds) in the scene.
        unsigned boidsCount = 10;

    public:
        explicit BoidsManager(std::string name, spark::core::GameObject* parent);

        /// \copydoc GameObject::onUpdate
        void onUpdate(float dt) override;

    private:
        /**
         * \brief Adjusts the number of birds in the scene, corresponding to the value of \ref boidsCount.
         */
        void adjustBirdCount();

    private:
        GameObject* m_birdContainer = nullptr;
        std::unordered_map<std::size_t, std::list<const Bird*>> m_birds;
    };
}

IMPLEMENT_SPARK_RTTI(boids::BoidsManager)
