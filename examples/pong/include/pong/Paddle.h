#pragma once

#include "spark/core/Application.h"
#include "spark/engine/GameObject.h"
#include "spark/engine/components/Rectangle.h"
#include "spark/engine/components/Transform.h"
#include "spark/rtti/HasRtti.h"

namespace pong
{
    /**
     * \brief A paddle that can be moved up and down.
     */
    class Paddle final : public spark::engine::GameObject
    {
        DECLARE_SPARK_RTTI(Paddle, GameObject)

    public:
        explicit Paddle(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::engine::components::Rectangle>();
            component<spark::engine::components::Rectangle>()->size = {25, 100};
        }
    };
}

IMPLEMENT_SPARK_RTTI(pong::Paddle)
