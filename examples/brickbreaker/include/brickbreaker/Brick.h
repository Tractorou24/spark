#pragma once

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Collider.h"
#include "spark/core/components/Rectangle.h"
#include "spark/lib/Random.h"
#include "spark/rtti/HasRtti.h"

namespace brickbreaker
{
    /**
     * \brief A brick that can collide with a ball and be destroyed by it.
     */
    class Brick final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Brick, GameObject)

    public:
        explicit Brick(std::string name, GameObject* parent)
            : Brick(std::move(name), parent, {50, 20}) {}

        explicit Brick(std::string name, GameObject* parent, const spark::math::Vector2<float>& size)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Rectangle>();
            component<spark::core::components::Rectangle>()->size = size;
            component<spark::core::components::Rectangle>()->color = spark::lib::Random::ElementInRange(s_allowedColors);

            addComponent<spark::core::components::StaticCollider>(spark::math::Rectangle({0, 0}, size));
        }

    private:
        inline static constexpr std::array<spark::math::Vector4<float>, 16> s_allowedColors = {
            {
                {1.0f, 0.0f, 0.0f, 1.0f}, // Red
                {0.0f, 1.0f, 0.0f, 1.0f}, // Green
                {0.0f, 0.0f, 1.0f, 1.0f}, // Blue
                {1.0f, 1.0f, 0.0f, 1.0f}, // Yellow
                {0.0f, 1.0f, 1.0f, 1.0f}, // Cyan
                {1.0f, 0.0f, 1.0f, 1.0f}, // Magenta
                {0.5f, 0.5f, 0.5f, 1.0f}, // Gray
                {1.0f, 0.5f, 0.0f, 1.0f}, // Orange
                {0.5f, 0.0f, 1.0f, 1.0f}, // Purple
                {0.0f, 0.5f, 1.0f, 1.0f}, // Light Blue
                {0.5f, 1.0f, 0.0f, 1.0f}, // Lime
                {1.0f, 0.0f, 0.5f, 1.0f}, // Pink
                {0.0f, 1.0f, 0.5f, 1.0f}, // Sea Green
                {0.5f, 0.0f, 0.5f, 1.0f}, // Dark Purple
                {0.0f, 0.5f, 0.5f, 1.0f}, // Teal
                {0.5f, 0.5f, 0.0f, 1.0f}, // Olive
            }
        };
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::Brick)

SPARK_SERIALIZE_RTTI_CLASS(brickbreaker::Brick)
