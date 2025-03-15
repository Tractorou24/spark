#include "boids/Bird.h"

#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/components/Rectangle.h"
#include "spark/core/components/Transform.h"
#include "spark/math/Vector2.h"
#include "spark/math/Vector4.h"

#include <cstddef>
#include <string>
#include <utility>

namespace boids
{
    Bird::Bird(std::string name, spark::core::GameObject* parent, spark::math::Vector2<float> position)
        : spark::core::GameObject(std::move(name), parent)
    {
        addComponent<spark::core::components::Rectangle>(spark::math::Vector2<float> {15, 15.f}, spark::math::Vector4<float> {1, 1, 1, 1});
        transform()->position = std::move(position);
        m_currentCellId = cell();
    }

    std::size_t Bird::cell() const
    {
        const auto position = transform()->position.castTo<std::size_t>();
        return position.x / 50 * 100 + position.y / 50;
    }

    void Bird::onUpdate(const float dt)
    {
        const auto mouse_position = spark::core::Input::MousePosition();
        const auto direction = (mouse_position - transform()->position).normalized();

        transform()->position += direction * 100.f * dt;

        // Update the cell position if it changed
        if (const auto new_cell = cell(); m_currentCellId != new_cell)
        {
            auto old_cell = m_currentCellId;
            m_currentCellId = new_cell;
            onCellChanged.emit(this, std::move(old_cell));
        }
    }
}
