#pragma once

#include "boids/Bird.h"
#include "boids/SimulationData.h"

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

        /// \brief The number of cells in the scene.
        unsigned cellCount = 100;

        /// \brief The data that controls the boids behavior.
        SimulationData simulationData;

    public:
        explicit BoidsManager(std::string name, spark::core::GameObject* parent);

        /// \copydoc GameObject::onUpdate
        void onUpdate(float dt) override;

    private:
        /**
         * \brief Adjusts the number of birds in the scene, corresponding to the value of \ref boidsCount.
         */
        void adjustBirdCount();

        /**
         * \brief Adjusts the number of cells in the scene, corresponding to the value of \ref cellCount;
         */
        void adjustCellCount();

    private:
        std::unordered_map<std::size_t, std::list<Bird*>> m_birds;
    };
}

IMPLEMENT_SPARK_RTTI(boids::BoidsManager)
