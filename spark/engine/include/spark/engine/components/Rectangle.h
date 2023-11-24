#pragma once

#include "spark/core/Renderer2D.h"

#include "spark/engine/components/Transform.h"
#include "spark/math/Vector2.h"
#include "spark/rtti/HasRtti.h"

namespace spark::engine::components
{
    /**
     * \brief A simple component to render a square/rectangle.
     */
    class Rectangle final : public Component
    {
        DECLARE_SPARK_RTTI(Rectangle, Component)

    public:
        math::Vector2<float> size = {50, 50};

    public:
        explicit Rectangle(GameObject* parent)
            : Component(parent) {}

        void render() const override
        {
            Component::render();

            const auto position = Transform::LocalToWorld(gameObject()->transform());
            core::Renderer2D::DrawQuad(position, size, {255, 255, 255});
        }
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::components::Rectangle)
