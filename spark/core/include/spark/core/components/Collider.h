#pragma once

#include "spark/core/Component.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Transform.h"

#include "spark/math/Rectangle.h"
#include "spark/math/Vector4.h"
#include "spark/patterns/Signal.h"
#include "spark/patterns/Traverser.h"
#include "spark/rtti/HasRtti.h"

#include "glm/gtc/matrix_transform.hpp"

namespace spark::core::components
{
    /**
     * \brief A component representing a square collider.
     * \note This component is not meant to be used directly, use @ref StaticCollider or @ref DynamicCollider instead.
     */
    class Collider : public Component
    {
        DECLARE_SPARK_RTTI(Collider, Component)
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
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

    protected:
        explicit Collider(GameObject* parent, math::Rectangle<float> rectangle)
            : Component(parent), m_rectangle(std::move(rectangle)) {}

    private:
        math::Rectangle<float> m_rectangle;
    };

    /**
     * \brief A square collider that does not move and is only used for collision detection.
     */
    class StaticCollider final : public Collider
    {
        DECLARE_SPARK_RTTI(StaticCollider, Collider)

    public:
        explicit StaticCollider(GameObject* parent)
            : Collider(parent, {}) {}

        explicit StaticCollider(GameObject* parent, math::Rectangle<float> rectangle)
            : Collider(parent, std::move(rectangle)) {}
    };

    /**
     * \brief A square collider that moves and checks for collisions with other colliders.
     */
    class DynamicCollider final : public Collider
    {
        DECLARE_SPARK_RTTI(DynamicCollider, Collider)

    public:
        /// \brief Signal emitted when a collision is detected.
        patterns::Signal<const Collider&> onCollision;

    public:
        explicit DynamicCollider(GameObject* parent)
            : Collider(parent, {}) {}

        explicit DynamicCollider(GameObject* parent, math::Rectangle<float> rectangle)
            : Collider(parent, std::move(rectangle)) {}

        void onUpdate(float /*dt*/) override
        {
            // Check for collision with each collider and trigger the onCollision signal if a collision is detected
            for (const auto* collider : allColliders())
                if (collider != this)
                    if (collidesWith(*collider))
                        onCollision.emit(*collider);
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
                for (const auto* component : obj->components())
                    if (&component->rttiInstance() == &StaticCollider::classRtti() || &component->rttiInstance() == &DynamicCollider::classRtti())
                        colliders.push_back(static_cast<const Collider*>(component));
            });
            patterns::traverse_tree(gameObject()->root(), traverser);
            return colliders;
        }
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Collider)

IMPLEMENT_SPARK_RTTI(spark::core::components::StaticCollider)

IMPLEMENT_SPARK_RTTI(spark::core::components::DynamicCollider)
