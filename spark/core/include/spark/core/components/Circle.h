#pragma once

#include "spark/core/Application.h"
#include "spark/core/components/Transform.h"

#include "spark/math/Vector2.h"
#include "spark/rtti/HasRtti.h"

namespace spark::core::components
{
    /**
     * \brief A simple component to render a circle.
     */
    class Circle final : public Component
    {
        DECLARE_SPARK_RTTI(Circle, Component)

    public:
        float radius = 25;

    public:
        /**
         * \brief Creates a new @link Circle circle component @endlink. Defaults to a radius of 25.
         * \param parent The parent game object.
         */
        explicit Circle(GameObject* parent)
            : Component(parent) {}

        /**
         * \brief Creates a new @link Circle circle component @endlink.
         * \param radius The radius of the circle. (in pixels)
         * \param parent The parent game object.
         */
        explicit Circle(GameObject* parent, const float radius)
            : Component(parent), radius(radius) {}

        void render() const override
        {
            Component::render();

            // Get the transform matrix for the circle
            auto transform_matrix = gameObject()->transform()->matrix();
            transform_matrix = glm::translate(transform_matrix, {radius, radius, 0.0f});
            transform_matrix = glm::scale(transform_matrix, {radius * 2, radius * 2, 1.0f});

            // Draw the circle
            core::Application::Instance()->window().renderer().drawCircle(transform_matrix, radius);
        }
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Circle)
