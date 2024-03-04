#pragma once

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Collider.h"
#include "spark/core/components/Rectangle.h"
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
        explicit Brick(std::string name, GameObject* parent, const spark::math::Vector2<float>& size)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Rectangle>();
            component<spark::core::components::Rectangle>()->size = size;

            addComponent<spark::core::components::StaticCollider>(spark::math::Rectangle({0, 0}, size));
        }
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::Brick)
