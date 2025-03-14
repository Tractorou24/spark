#pragma once

#include "spark/core/GameObject.h"
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
        explicit Bird(std::string name, spark::core::GameObject* parent)
            : spark::core::GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Rectangle>(spark::math::Vector2<float> {15, 15.f}, spark::math::Vector4<float> {1, 1, 1, 1});
        }
    };
}

IMPLEMENT_SPARK_RTTI(boids::Bird)
