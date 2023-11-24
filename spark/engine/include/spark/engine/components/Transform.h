#pragma once

#include "spark/engine/Component.h"
#include "spark/engine/Export.h"
#include "spark/engine/GameObject.h"

#include "spark/math/Vector2.h"

namespace spark::engine::components
{
    /**
     * \brief A component that stores the position of a game object.
     */
    class SPARK_ENGINE_EXPORT Transform final : public Component
    {
        DECLARE_SPARK_RTTI(Transform, Component)

    public:
        math::Vector2<float> position = {};

    public:
        static math::Vector2<float> LocalToWorld(const Transform* object)
        {
            math::Vector2<float> position = {};

            auto* game_object = object->gameObject();
            while (game_object->getParent() != nullptr)
            {
                position += game_object->transform()->position;
                game_object = game_object->getParent();
            }
            return position;
        }

    public:
        explicit Transform(GameObject* parent)
            : Component(parent) {}

        friend bool operator==(const Transform& lhs, const Transform& rhs) { return lhs.position == rhs.position; }
        friend bool operator!=(const Transform& lhs, const Transform& rhs) { return !(lhs == rhs); }
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::components::Transform)
