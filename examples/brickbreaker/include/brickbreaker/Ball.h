#pragma once

#include "brickbreaker/Brick.h"
#include "brickbreaker/Paddle.h"
#include "brickbreaker/ScreenBorder.h"

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
                if (&other.gameObject()->rttiInstance() == &Brick::classRtti())
                {
                    const auto bounds = component<spark::core::components::DynamicCollider>()->bounds();
                    const auto other_bounds = other.bounds();

                    const bool top = bounds.w - other_bounds.y > 0;
                    const bool bottom = bounds.y - other_bounds.w > 0;
                    const bool left = bounds.x - other_bounds.z > 0;
                    const bool right = bounds.z - other_bounds.x > 0;

                    if (top || bottom)
                        direction = {direction.x, -direction.y};
                    else if (left || right)
                        direction = {-direction.x, direction.y};
                    else if (top && left)
                        direction = {-abs(direction.x), -abs(direction.y)};
                    else if (top && right)
                        direction = {abs(direction.x), -abs(direction.y)};
                    else if (bottom && left)
                        direction = {-abs(direction.x), abs(direction.y)};
                    else if (bottom && right)
                        direction = {abs(direction.x), abs(direction.y)};
                    else
                        direction = {-direction.x, -direction.y};

                    // Increase the velocity by 0.1%.
                    velocity *= 1.001f;

                    // Destroy the brick.
                    Destroy(const_cast<GameObject*>(other.gameObject()));
                }
                else if (&other.gameObject()->rttiInstance() == &Paddle::classRtti())
                {
                    direction = {direction.x, -direction.y};
                } else if(&other.gameObject()->rttiInstance() == &ScreenBorder::classRtti())
                {
                    if (other.gameObject()->name() == "Top Border")
                        direction = {direction.x, -direction.y};
                    else if (other.gameObject()->name() == "Left Border" || other.gameObject()->name() == "Right Border")
                        direction = {-direction.x, direction.y};
                }
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
