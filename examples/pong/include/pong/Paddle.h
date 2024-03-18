#pragma once

#include "experimental/ser/SerializerScheme.h"
#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Rectangle.h"
#include "spark/rtti/HasRtti.h"

namespace pong
{
    /**
     * \brief A paddle that can be moved up and down.
     */
    class Paddle final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Paddle, GameObject)

    public:
        explicit Paddle(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Rectangle>();
            component<spark::core::components::Rectangle>()->size = {25, 100};
        }
    };
}

IMPLEMENT_SPARK_RTTI(pong::Paddle)

SPARK_SERIALIZE_RTTI_CLASS(pong::Paddle)
