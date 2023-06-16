#pragma once

#include "spark/engine/components/Transform.h"

#include "spark/core/Renderer2D.h"
#include "spark/math/Vector2.h"
#include "spark/rtti/HasRtti.h"

namespace spark::engine::components
{
    /**
     * \brief A simple component to render a circle.
     */
    class Circle final : public Component
    {
        DECLARE_SPARK_RTTI(Circle, Component)

    public:
        float radius;

    public:
        explicit Circle(GameObject* parent, const float radius)
            : Component(parent), radius(radius) {}

        void render() const override
        {
            Component::render();

            const auto position = Transform::LocalToWorld(getGameObject()->getTransform());
            core::Renderer2D::DrawCircle(position, {255, 255, 255}, radius);
        }
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::components::Circle)
