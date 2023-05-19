#pragma once

#include "pong/ui/Background.h"

#include "spark/core/Application.h"
#include "spark/engine/GameObject.h"
#include "spark/engine/components/Circle.h"
#include "spark/engine/components/Rectangle.h"
#include "spark/engine/components/Transform.h"
#include "spark/lib/Random.h"
#include "spark/math/Constants.h"
#include "spark/patterns/Signal.h"
#include "spark/rtti/HasRtti.h"

namespace pong
{
    /**
     * \brief A ball that bounces around the screen.
     */
    class Ball final : public spark::engine::GameObject
    {
        DECLARE_SPARK_RTTI(Ball, GameObject)

    public:
        spark::math::Vector2<float> direction = {};
        float velocity = 250.0f;
        spark::patterns::Signal<> onLoose;

    public:
        explicit Ball(std::string name, spark::engine::GameObject* parent, const float radius)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::engine::components::Circle>(radius);
        }

        void onSpawn() override
        {
            GameObject::onSpawn();

            // Find the paddles.
            m_leftPaddle = spark::engine::GameObject::FindByName(&getRoot(), "Left Paddle");
            SPARK_ASSERT(m_leftPaddle != nullptr)

            m_rightPaddle = spark::engine::GameObject::FindByName(&getRoot(), "Right Paddle");
            SPARK_ASSERT(m_rightPaddle != nullptr)

            // Set the paddle size (used to calculate the bounce).
            m_paddleSize = m_leftPaddle->getComponent<spark::engine::components::Rectangle>()->size;

            // Randomize the direction of the ball.
            float angle = spark::lib::Random::Number(spark::math::PI / 6, spark::math::PI / 4);
            if (spark::lib::Random::Number(0.0f, 1.0f) > 0.5f) // 50% chance to flip the angle up/down.
                angle = -angle;
            if (spark::lib::Random::Number(0.0f, 1.0f) > 0.5f) // 50% chance to flip the angle left/right.
                angle = spark::math::PI - angle;

            direction = {std::cos(angle), std::sin(angle)};
        }

        void onUpdate(const float dt) override
        {
            spark::engine::GameObject::onUpdate(dt);

            const auto [next_position, next_direction] = calculateNextFrame(getTransform()->position + direction * velocity * dt);
            if (checkLoose(next_position))
                onLoose.emit();

            getTransform()->position = next_position;
            direction = next_direction;
        }

    private:
        /**
         * \brief Calculates the new position and direction of the ball from the desired position handling the screen bounds.
         * \param next_position The desired position of the ball calculated from the current position and direction for the next frame.
         * \return A pair containing the adjusted position and direction of the ball.
         */
        [[nodiscard]] std::pair<spark::math::Vector2<float>, spark::math::Vector2<float>> calculateNextFrame(const spark::math::Vector2<float>& next_position)
        {
            const spark::math::Vector2 window_size = {
                static_cast<float>(spark::core::Application::Instance()->getWindow().getWidth()),
                static_cast<float>(spark::core::Application::Instance()->getWindow().getHeight())
            };
            const float radius = getComponent<spark::engine::components::Circle>()->radius;

            // Check if the ball is going to be outside the screen (top, down) boundaries.
            const bool hit_top_wall = next_position.y < 0;
            const bool hit_bottom_wall = next_position.y > window_size.y - radius * 2;

            // Adjust the ball's position if necessary.
            spark::math::Vector2 adjusted_position = next_position;
            if (hit_top_wall)
                adjusted_position.y = radius;
            else if (hit_bottom_wall)
                adjusted_position.y = window_size.y - radius * 2;

            // Check if the ball is hitting the top or bottom wall.
            if (hit_top_wall || hit_bottom_wall)
                return {getTransform()->position, {direction.x, -direction.y}};

            // Check if the ball is hitting the left paddle.
            if (next_position.x < m_leftPaddle->getTransform()->position.x + m_paddleSize.x &&
                next_position.y > m_leftPaddle->getTransform()->position.y - radius && next_position.y < m_leftPaddle->getTransform()->position.y + m_paddleSize.y)
            {
                spark::engine::GameObject::FindByName(&getRoot(), "Left Score")->getComponent<ui::Score>()->incrementScore.emit(1);
                return {{adjusted_position.x + radius, adjusted_position.y}, {-direction.x, direction.y}};
            }

            // Check if the ball is hitting the right paddle.
            if (next_position.x > m_rightPaddle->getTransform()->position.x - radius * 2 &&
                next_position.y > m_rightPaddle->getTransform()->position.y - radius && next_position.y < m_rightPaddle->getTransform()->position.y + m_paddleSize.y)
            {
                spark::engine::GameObject::FindByName(&getRoot(), "Right Score")->getComponent<ui::Score>()->incrementScore.emit(1);
                return {{adjusted_position.x - radius, adjusted_position.y}, {-direction.x, direction.y}};
            }

            return {adjusted_position, direction};
        }

        /**
         * \brief Checks if the ball is going to be outside the screen boundaries.
         * \param next_position The desired position of the ball calculated from the current position and direction for the next frame.
         * \return A boolean indicating if the ball is going to be outside the screen boundaries (if the player has lost).
         */
        [[nodiscard]] bool checkLoose(const spark::math::Vector2<float> next_position) const
        {
            const auto window_width = static_cast<float>(spark::core::Application::Instance()->getWindow().getWidth());

            const bool left_wall_hit = next_position.x < 0;
            const bool right_wall_hit = next_position.x > window_width - getComponent<spark::engine::components::Circle>()->radius * 2;

            if (left_wall_hit || right_wall_hit)
                return true;
            return false;
        }

    private:
        GameObject *m_leftPaddle = nullptr, *m_rightPaddle = nullptr;
        spark::math::Vector2<float> m_paddleSize = {};
    };
}

IMPLEMENT_SPARK_RTTI(pong::Ball)
