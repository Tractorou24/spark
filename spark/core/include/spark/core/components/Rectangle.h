#pragma once

#include "spark/core/Application.h"
#include "spark/core/components/Transform.h"

#include "spark/math/Vector2.h"
#include "spark/math/Vector4.h"
#include "spark/rtti/HasRtti.h"

namespace spark::core::components
{
    /**
     * \brief A simple component to render a square/rectangle.
     */
    class Rectangle final : public Component
    {
        DECLARE_SPARK_RTTI(Rectangle, Component)

    public:
        math::Vector2<float> size = {50, 50};
        math::Vector4<float> color = {1.f, 1.f, 1.f, 1.f};

    public:
        explicit Rectangle(GameObject* parent)
            : Component(parent) {}

        void render() const override
        {
            Component::render();

            // Get the transform matrix for the rectangle
            auto transform_matrix = gameObject()->transform()->matrix();

            transform_matrix = glm::translate(transform_matrix, {size.x / 2.0f, size.y / 2.0f, 0.0f});
            transform_matrix = glm::scale(transform_matrix, {size.x, size.y, 1.0f});

            // Draw the rectangle
            core::Application::Instance()->window().renderer().drawQuad(transform_matrix, color);
        }
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Rectangle)
