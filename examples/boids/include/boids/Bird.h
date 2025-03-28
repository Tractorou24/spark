#pragma once

#include "boids/SimulationData.h"

#include "spark/core/GameObject.h"
#include "spark/math/Vector2.h"
#include "spark/patterns/Signal.h"

#include <list>

namespace boids
{
    /**
     * \brief A simple bird contained in the boids simulation.
     */
    class Bird final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Bird, GameObject)

    public:
        spark::patterns::Signal<Bird*, std::size_t> onCellChanged;

    public:
        explicit Bird(std::string name,
                      spark::core::GameObject* parent,
                      spark::math::Vector2<float> position,
                      const SimulationData* simulation_settings,
                      std::function<std::list<Bird*>(std::size_t)> birds_in_cell_fn);

        /**
         * \brief Changes the number of cells a bird could be in.
         * \param new_count The new cell count.
         *
         * \note The \ref onCellChanged signal is not emitted.
         */
        void setCellCount(std::size_t new_count);

        /**
         * \brief Gets the cell unique ID the bird is in.
         * \return The cell unique ID.
         */
        [[nodiscard]] std::size_t cell() const;

        /// \copydoc GameObject::onUpdate
        void onUpdate(float dt) override;

        /// \copydoc GameObject::onDestroyed
        void onDestroyed() override;

        friend bool operator==(const Bird& lhs, const Bird& rhs) { return lhs.uuid() == rhs.uuid(); }
        friend bool operator!=(const Bird& lhs, const Bird& rhs) { return !(lhs == rhs); }

    private:
        const SimulationData* m_simulationSettings = nullptr;
        std::function<std::list<Bird*>(std::size_t)> m_birdsInCellFn;
        spark::math::Vector2<float> m_direction = {0, 0};
        std::size_t m_cellCount = 100;
        std::size_t m_currentCellId = 0;
    };
}

IMPLEMENT_SPARK_RTTI(boids::Bird)
