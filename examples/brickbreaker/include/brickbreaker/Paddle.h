#pragma once

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/components/Collider.h"
#include "spark/core/components/Rectangle.h"
#include "spark/rtti/HasRtti.h"

namespace brickbreaker
{
    /**
     * \brief A paddle that can be moved left and right.
     */
    class Paddle final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Paddle, GameObject)

    public:
        float speed = 500.0f;

    public:
        explicit Paddle(std::string name, GameObject* parent, spark::math::Vector2<float> size)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Rectangle>();
            component<spark::core::components::Rectangle>()->size = std::move(size);

            addComponent<spark::core::components::StaticCollider>(spark::math::Rectangle({0, 0}, size));
        }

        void onSpawn() override
        {
            // Set the paddle at the bottom of the screen
            const spark::math::Vector2 window_size = spark::core::Application::Instance()->window().size().castTo<float>();
            transform()->position = {window_size.x - 25 - 10, window_size.y - 50};
        }

        void onUpdate(const float dt) override
        {
            const float next_position = spark::core::Input::MousePosition().x;
            transform()->position.x = newPaddlePosition(next_position, dt);
        }

    private:
        /**
         * \brief Calculates the new position of the paddle from the desired position handling the screen bounds.
         * \param next_position The desired position of the paddle.
         * \param dt The time since the last frame.
         * \return The new position of the top of the paddle.
         */
        [[nodiscard]] float newPaddlePosition(const float next_position, const float dt) const
        {
            const auto paddle_rect = component<spark::core::components::Rectangle>();
            const auto screen_width = spark::core::Application::Instance()->window().size().castTo<float>().x;

            // Move the paddle towards the desired position, but not faster than the speed
            const auto raw_pos = transform()->position.x + std::clamp(next_position - paddle_rect->size.x / 2 - transform()->position.x, -speed * dt, speed * dt);
            return std::clamp(raw_pos, 0.0f, screen_width - paddle_rect->size.x);
        }
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::Paddle)
