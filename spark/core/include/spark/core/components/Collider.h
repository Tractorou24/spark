#pragma once

#include "spark/core/Component.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Transform.h"

#include "spark/math/Rectangle.h"
#include "spark/math/Vector4.h"
#include "spark/patterns/Signal.h"
#include "spark/patterns/Traverser.h"

namespace spark::core::components
{
    /**
     * \brief A component representing a square collider.
     *
     * Checks for collision with all other colliders in the scene at every frame and emits a signal on collision.
     * This is not efficient, but works for now. A better solution would be to use a quad-tree to store all colliders.
     */
    class Collider final : public Component
    {
        DECLARE_SPARK_RTTI(Collider, Component)
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
        patterns::Signal<> onCollision;

    public:
        explicit Collider(GameObject* parent)
            : Component(parent) {}

        explicit Collider(GameObject* parent, math::Rectangle<float> rectangle)
            : Component(parent), m_rectangle(std::move(rectangle)) {}

        /**
         * \brief Gets the bounds of the rectangle in screen space.
         * \return A @ref spark::math::Vector4<float> containing the bounds of the rectangle. [xMin, yMin, xMax, yMax]
         */
        [[nodiscard]] math::Vector4<float> bounds() const
        {
            // Get the transform matrix of [0,0] of the rectangle
            auto matrix = gameObject()->transform()->matrix();
            matrix = glm::translate(matrix, {m_rectangle.position.x, m_rectangle.position.y, 0});

            // Get the transform matrix of [3,3] of the rectangle
            auto matrix4 = glm::translate(matrix, {m_rectangle.extent.x, m_rectangle.extent.y, 0});

            // Return the bounds of the rectangle
            return {matrix[3][0], matrix[3][1], matrix4[3][0], matrix4[3][1]};
        }

        /**
         * \brief Checks if the collider is colliding with another collider.
         * \param other The other collider to check for collision.
         * \return `true` if the colliders are colliding, `false` otherwise.
         */
        [[nodiscard]] bool collidesWith(const Collider& other) const
        {
            const math::Vector4<float> this_bounds = bounds();
            const math::Vector4<float> other_bounds = other.bounds();

            // Check if the colliders are colliding using this formula:
            //   - x1 <= x2 + w2 && x1 + w1 >= x2 && y1 <= y2 + h2 && y1 + h1 >= y2)
            if (this_bounds.x <= other_bounds.z && this_bounds.z >= other_bounds.x && this_bounds.y <= other_bounds.w && this_bounds.w >= other_bounds.y)
                return true;
            return false;
        }

        void onUpdate(float /*dt*/) override
        {
            // Check for collision with each collider and trigger the onCollision signal if a collision is detected
            for (const auto& collider : allColliders())
                if (collider != this)
                    if (collidesWith(*collider))
                        onCollision.emit();
        }

    private:
        /**
         * \brief Gets all colliders in the scene.
         * \return A @ref std::vector of all colliders in the scene.
         */
        [[nodiscard]] std::vector<const Collider*> allColliders()
        {
            std::vector<const Collider*> colliders;
            auto traverser = patterns::make_traverser<GameObject>([&](const GameObject* obj)
            {
                if (obj->hasComponent<Collider>())
                    colliders.push_back(obj->component<Collider>());
            });
            patterns::traverse_tree(gameObject()->root(), traverser);
            return colliders;
        }

    private:
        math::Rectangle<float> m_rectangle;
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Collider)
