#pragma once

#include "brickbreaker/Paddle.h"

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Circle.h"
#include "spark/core/components/Collider.h"
#include "spark/core/components/Transform.h"
#include "spark/lib/Random.h"
#include "spark/patterns/Signal.h"
#include "spark/rtti/HasRtti.h"

#include <numbers>

namespace brickbreaker
{
    /**
     * \brief A ball that bounces around the screen.
     */
    class Ball final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Ball, GameObject)

    public:
        spark::math::Vector2<float> direction = {};
        float velocity = 400.0f;
        spark::patterns::Signal<> onLoose;

    public:
        explicit Ball(std::string name, GameObject* parent, const float radius)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Circle>(radius);
            addComponent<spark::core::components::DynamicCollider>(spark::math::Rectangle<float> {{0, 0}, {radius * 2, radius * 2}});

            component<spark::core::components::DynamicCollider>()->onCollision.connect([this](const spark::core::components::Collider& other)
            {
                const bool is_paddle = &other.gameObject()->rttiInstance() == &Paddle::classRtti();
                const bool is_top_wall = other.gameObject()->name().find("Top") != std::string::npos;

                // Direction is inverted in the y-axis if the ball collides with the paddle or the top wall.
                if (is_paddle || is_top_wall)
                {
                    direction.y = -direction.y;
                    return;
                }

                // Else, the direction is inverted in the x-axis.
                direction.x = -direction.x;

                // Increase the velocity by 10%.
                velocity *= 1.1f;
            });
        }

        void onSpawn() override
        {
            // Set the ball in the middle of the screen.
            const auto window_size = spark::core::Application::Instance()->window().size().castTo<float>();
            transform()->position = {window_size.x / 2, window_size.y * 0.6f};

            // Randomize the direction of the ball.
            float angle = spark::lib::Random::Number(std::numbers::pi_v<float> / 6, std::numbers::pi_v<float> / 4);
            if (spark::lib::Random::Number(0.0f, 1.0f) > 0.5f) // 50% chance to flip the angle up/down.
                angle = -angle;
            if (spark::lib::Random::Number(0.0f, 1.0f) > 0.5f) // 50% chance to flip the angle left/right.
                angle = std::numbers::pi_v<float> - angle;

            direction = {std::cos(angle), std::sin(angle)};
        }

        void onUpdate(const float dt) override
        {
            transform()->position += direction * velocity * dt;
            if (checkLoose(transform()->position))
                onLoose.emit();
        }

    private:
        /**
         * \brief Checks if the ball is going to be outside the screen boundaries.
         * \param next_position The desired position of the ball calculated from the current position and direction for the next frame.
         * \return A boolean indicating if the ball is going to be outside the screen boundaries (if the player has lost).
         */
        [[nodiscard]] static bool checkLoose(const spark::math::Vector2<float>& next_position)
        {
            const auto window_height = spark::core::Application::Instance()->window().size().castTo<float>().y;
            if (next_position.y > window_height)
                return true;
            return false;
        }
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::Ball)
