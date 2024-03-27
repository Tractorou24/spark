#pragma once

#include <algorithm>

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

                    // Check where is the ball in relation to the brick
                    const float ball_x_start = bounds.x + component<spark::core::components::Circle>()->radius * 0.05f;
                    const float ball_x_end = bounds.x + component<spark::core::components::Circle>()->radius * 0.95f;
                    const float ball_y_start = bounds.y + component<spark::core::components::Circle>()->radius * 0.05f;
                    const float ball_y_end = bounds.y + component<spark::core::components::Circle>()->radius * 0.95f;
                    const float brick_x = other_bounds.x + other.gameObject()->component<spark::core::components::Rectangle>()->size.x / 2;
                    const float brick_y = other_bounds.y + other.gameObject()->component<spark::core::components::Rectangle>()->size.y / 2;

                    const bool ball_left = (ball_x_start < brick_x && ball_y_start < other_bounds.w && ball_y_start > other_bounds.y) || (ball_x_end < brick_x && ball_y_end <
                        other_bounds.w && ball_y_end > other_bounds.y);
                    const bool ball_right = (ball_x_start > brick_x && ball_y_start < other_bounds.w && ball_y_start > other_bounds.y) || (ball_x_end > brick_x && ball_y_end <
                        other_bounds.w && ball_y_end > other_bounds.y);
                    const bool ball_top = (ball_y_start < brick_y && ball_x_start < other_bounds.z && ball_x_start > other_bounds.x) || (ball_y_end < brick_y && ball_x_end <
                        other_bounds.z && ball_x_end > other_bounds.x);
                    const bool ball_bottom = (ball_y_start > brick_y && ball_x_start < other_bounds.z && ball_x_start > other_bounds.x) || (ball_y_end > brick_y && ball_x_end <
                        other_bounds.z && ball_x_end > other_bounds.x);

                    // Check if the ball hit the brick from the top, bottom, left or right and the center position
                    const bool top = bounds.w - other_bounds.y > 0;
                    const bool bottom = bounds.y - other_bounds.w < 0;
                    const bool left = bounds.z - other_bounds.x > 0;
                    const bool right = bounds.x - other_bounds.z < 0;

                    if (top && !m_goingUp && ball_top)
                    {
                        direction = {direction.x, -direction.y};
                        m_goingUp = true;
                    }
                    if (left && !m_goingLeft && ball_left)
                    {
                        direction = {-direction.x, direction.y};
                        m_goingLeft = true;
                    }
                    if (bottom && m_goingUp && ball_bottom)
                    {
                        direction = {direction.x, -direction.y};
                        m_goingUp = false;
                    }
                    if (right && m_goingLeft && ball_right)
                    {
                        direction = {-direction.x, direction.y};
                        m_goingLeft = false;
                    }

                    // Increase the velocity by 0.1%.
                    velocity *= 1.001f;

                    // Destroy the brick.
                    Destroy(const_cast<GameObject*>(other.gameObject()));
                } else if (&other.gameObject()->rttiInstance() == &Paddle::classRtti())
                {
                    /*
                     * Calculate the percentage of the ball position from the paddle perspective
                     *
                     * Angle data:
                     * - range `[0%, 45%]`, the angle is change proportionally to the paddle position on the left side
                     * - range `[45%, 55%]`, the angle is 0°,
                     * - range `[55%, 100%]`, the angle is change proportionally to the paddle position on the right side
                    */
                    const float paddle_length = m_paddle->component<spark::core::components::Rectangle>()->size.x;
                    const float paddle_position_x = m_paddle->transform()->position.x;
                    const float ball_center_x = transform()->position.x + component<spark::core::components::Circle>()->radius;
                    const float ball_position_on_paddle = (ball_center_x - paddle_position_x) / paddle_length;

                    if (ball_position_on_paddle < 0.45f) // 0% to 45%
                    {
                        const float angle = std::numbers::pi_v<float> * ball_position_on_paddle;
                        direction = {-std::cos(angle), -std::sin(angle)};
                        m_goingUp = true;
                        m_goingLeft = true;
                    } else if (ball_position_on_paddle > 0.45f && ball_position_on_paddle < 0.55f) // 45% to 55%
                    {
                        direction = {0, -1};
                        m_goingUp = true;
                        m_goingLeft = false;
                    } else if (ball_position_on_paddle > 0.55f) // 55% to 100%
                    {
                        const float angle = std::numbers::pi_v<float> * (1 - ball_position_on_paddle);
                        direction = {std::cos(angle), -std::sin(angle)};
                        m_goingUp = true;
                        m_goingLeft = false;
                    }
                } else if (&other.gameObject()->rttiInstance() == &ScreenBorder::classRtti())
                {
                    if (other.gameObject()->name() == "Top Border")
                    {
                        transform()->position.y = transform()->position.y + 5;
                        direction = {direction.x, -direction.y};
                        m_goingUp = false;
                    } else if (other.gameObject()->name() == "Left Border")
                    {
                        transform()->position.x = transform()->position.x + 5;
                        direction = {-direction.x, direction.y};
                        m_goingLeft = false;
                    } else if (other.gameObject()->name() == "Right Border")
                    {
                        transform()->position.x = transform()->position.x - 5;
                        direction = {-direction.x, direction.y};
                        m_goingLeft = true;
                    }
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

            m_paddle = FindByName(root(), "Paddle");
            SPARK_ASSERT(m_paddle != nullptr);
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

    private:
        GameObject* m_paddle = nullptr;
        bool m_goingUp = false;
        bool m_goingDown = false;
        bool m_goingLeft = false;
        bool m_goingRight = false;
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::Ball)
