#pragma once

#include "spark/core/Component.h"
#include "spark/core/Export.h"
#include "spark/core/GameObject.h"

#include "spark/math/Vector2.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace spark::core::components
{
    /**
     * \brief A component that stores the position of a game object.
     */
    class SPARK_CORE_EXPORT Transform final : public Component
    {
        DECLARE_SPARK_RTTI(Transform, Component)

    public:
        math::Vector2<float> position = {0.f, 0.f};
        float rotation = 0;
        math::Vector2<float> scale = {1.f, 1.f};

    public:
        explicit Transform(GameObject* parent)
            : Component(parent) {}

        friend bool operator==(const Transform& lhs, const Transform& rhs) { return lhs.position == rhs.position; }
        friend bool operator!=(const Transform& lhs, const Transform& rhs) { return !(lhs == rhs); }

        /**
         * \brief Gets the transformation matrix of the transform.
         * \return A \ref glm::mat4 representing the transformation matrix.
         */
        glm::mat4 matrix() const
        {
            glm::mat4 matrix(1.f);

            // Apply local transform
            matrix = glm::translate(matrix, {gameObject()->transform()->position.x, gameObject()->transform()->position.y, 0.f});
            matrix = glm::rotate(matrix, gameObject()->transform()->rotation, {0.0f, 0.0f, 1.0f});
            matrix = glm::scale(matrix, {gameObject()->transform()->scale.x, gameObject()->transform()->scale.y, 1.0f});

            // Apply parent transforms
            const GameObject* parent = gameObject()->parent();
            while (parent)
            {
                const Transform& parent_transform = *parent->transform();
                matrix = glm::translate(matrix, {parent_transform.position.x, parent_transform.position.y, 0.f});
                matrix = glm::rotate(matrix, parent_transform.rotation, {0.0f, 0.0f, 1.0f});
                matrix = glm::scale(matrix, {parent_transform.scale.x, parent_transform.scale.y, 1.0f});
                parent = parent_transform.gameObject()->parent();
            }

            return matrix;
        }
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Transform)
