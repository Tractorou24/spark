#pragma once

#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/components/Rectangle.h"
#include "spark/core/components/Transform.h"

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
        explicit Bird(std::string name, spark::core::GameObject* parent, spark::math::Vector2<float> position)
            : spark::core::GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Rectangle>(spark::math::Vector2<float> {15, 15.f}, spark::math::Vector4<float> {1, 1, 1, 1});
            transform()->position = std::move(position);
            m_currentCellId = cell();
        }

        /**
         * \brief Gets the cell unique ID the bird is in.
         * \return The cell unique ID.
         */
        [[nodiscard]] std::size_t cell() const
        {
            const auto position = transform()->position.castTo<std::size_t>();
            return position.x / 50 * 100 + position.y / 50;
        }

        void onUpdate(const float dt) override
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

        friend bool operator==(const Bird& lhs, const Bird& rhs) { return lhs.uuid() == rhs.uuid(); }
        friend bool operator!=(const Bird& lhs, const Bird& rhs) { return !(lhs == rhs); }

    private:
        std::size_t m_currentCellId = 0;
    };
}

IMPLEMENT_SPARK_RTTI(boids::Bird)
