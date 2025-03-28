#include "boids/Bird.h"
#include "boids/SimulationData.h"

#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/components/Rectangle.h"
#include "spark/core/components/Transform.h"
#include "spark/math/Vector2.h"
#include "spark/math/Vector4.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <list>
#include <numbers>
#include <string>
#include <utility>

namespace boids
{
    Bird::Bird(std::string name,
               spark::core::GameObject* parent,
               spark::math::Vector2<float> position,
               const SimulationData* simulation_settings,
               std::function<std::list<Bird*>(std::size_t)> birds_in_cell_fn)
        : spark::core::GameObject(std::move(name), parent)
        , m_simulationSettings(simulation_settings)
        , m_birdsInCellFn(std::move(birds_in_cell_fn))
    {
        if (!m_simulationSettings)
            throw spark::base::NullPointerException("Simulation settings cannot be null");

        addComponent<spark::core::components::Rectangle>(spark::math::Vector2<float> {2.5f, 2.5f}, spark::math::Vector4<float> {1, 1, 1, 1});
        transform()->position = std::move(position);
        m_currentCellId = cell();
    }

    void Bird::setCellCount(const std::size_t new_count)
    {
        if (new_count == 0)
            throw spark::base::BadArgumentException("The cell size cannot be zero.");
        if (m_cellCount == new_count)
            return;

        m_cellCount = new_count;
    }

    std::size_t Bird::cell() const
    {
        const auto position = transform()->position.castTo<std::size_t>();
        const auto cell_size = spark::core::Application::Instance()->window().size().castTo<std::size_t>() / m_cellCount;
        return position.x / cell_size.x + position.y / cell_size.y * m_cellCount;
    }

    void Bird::onUpdate(const float dt)
    {
        const auto nearby_birds = m_birdsInCellFn(m_currentCellId);

        // Initialize force vectors for the three boids rules
        spark::math::Vector2<float> separation {0.0f, 0.0f};
        spark::math::Vector2<float> alignment {0.0f, 0.0f};
        spark::math::Vector2<float> cohesion {0.0f, 0.0f};

        unsigned neighbor_count = 0;

        for (const Bird* other : nearby_birds)
        {
            if (other == this)
                continue;

            // Vector pointing from the current bird to the other bird
            const spark::math::Vector2<float> offset = other->transform()->position - transform()->position;
            const float distance = offset.norm();

            // Skip birds that are too far away
            if (distance > m_simulationSettings->maxDistance)
                continue;

            // Angle between the current bird's direction and the offset vector
            const float dot_product = m_direction.dot(offset.normalized());
            const float angle = std::acos(std::clamp(dot_product, -1.0f, 1.0f)) * (180.0f / std::numbers::pi_v<float>);

            // Skip birds outside the field of view
            if (angle > m_simulationSettings->fieldOfView / 2.0f)
                continue;
            neighbor_count++;

            // Separation: steer to avoid crowding local flockmates
            if (distance < m_simulationSettings->maxDistance)
                separation -= offset.normalized() * (m_simulationSettings->maxDistance / (distance + 0.0001f)); // Closer the neighbor, stronger the repulsion

            // Alignment: steer towards the average heading of local flockmates
            alignment += other->m_direction;

            // Cohesion: steer to move toward the average position of local flockmates
            cohesion += other->transform()->position;
        }

        // Calculate the final steering force
        spark::math::Vector2<float> steering {0.0f, 0.0f};

        if (neighbor_count > 0)
        {
            separation = separation.normalized() * m_simulationSettings->separationWeight;

            alignment = alignment / static_cast<float>(neighbor_count);
            alignment = alignment.normalized() * m_simulationSettings->alignmentWeight;

            cohesion = cohesion / static_cast<float>(neighbor_count) - transform()->position;
            cohesion = cohesion.normalized() * m_simulationSettings->cohesionWeight;

            // Combine all flocking forces
            steering = separation + alignment + cohesion;
        }

        // Goal-seeking to the mouse position
        const auto mouse_position = spark::core::Input::MousePosition();
        const auto mouse_direction = (mouse_position - transform()->position).normalized();

        if (m_simulationSettings->followMouse)
        {
            if (steering.norm() > 0.0001f)
            {
                constexpr float mouse_influence = 0.8f;
                const float steering_factor = 2.0f * dt; // Turning speed
                m_direction = (m_direction + (steering + mouse_direction * mouse_influence) * steering_factor).normalized();
            } else
            {
                // If no neighbors, gradually turn toward mouse
                const float turn_factor = 2.0f * dt;
                m_direction = (m_direction + mouse_direction * turn_factor).normalized();
            }
        } else
        {
            // Only use flocking behavior if not following mouse
            if (steering.norm() > 0.0001f)
            {
                const float steering_factor = 2.0f * dt;
                m_direction = (m_direction + steering * steering_factor).normalized();
            }
        }

        // Update the position with the new direction
        transform()->position += m_direction * m_simulationSettings->maxSpeed * dt;

        // Update the cell if changed
        if (const auto new_cell = cell(); m_currentCellId != new_cell)
        {
            const auto old_cell = m_currentCellId;
            m_currentCellId = new_cell;
            onCellChanged.emit(this, old_cell);
        }
    }

    void Bird::onDestroyed()
    {
        onCellChanged.clear();
    }
}
