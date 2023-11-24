#pragma once

#include "pong/ui/Background.h"

#include "spark/audio/Sound.h"
#include "spark/core/Application.h"
#include "spark/core/Input.h"
#include "spark/core/SceneManager.h"
#include "spark/engine/GameObject.h"
#include "spark/engine/components/Rectangle.h"
#include "spark/engine/components/Transform.h"
#include "spark/path/Paths.h"

#include <numeric>

namespace pong
{
    /**
     * \brief The pong game manager handling the game logic.
     */
    class GameManager final : public spark::engine::GameObject
    {
        DECLARE_SPARK_RTTI(GameManager, GameObject)

    public:
        explicit GameManager(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent), m_hitSound(spark::path::assets_path() / "hit.ogg"), m_looseSound(spark::path::assets_path() / "loose.ogg"),
              m_menuSound(spark::path::assets_path() / "menu.ogg") { }

        void onSpawn() override
        {
            m_leftPaddle = FindByName(root(), "Left Paddle");
            m_rightPaddle = FindByName(root(), "Right Paddle");
            m_ball = dynamic_cast<Ball*>(FindByName(root(), "Ball"));

            SPARK_ASSERT(m_leftPaddle != nullptr)
            SPARK_ASSERT(m_rightPaddle != nullptr)
            SPARK_ASSERT(m_ball != nullptr)

            m_looseSlotKey = m_ball->onLoose.connect([this]
            {
                m_looseSound.play();
                spark::core::SceneManager::LoadScene("MainMenu");
            });

            m_menuSound.play();

            // Set the paddles and the ball to their initial positions.
            reset();
        }

        void onUpdate(const float dt) override
        {
            // Left paddle
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::Z))
            {
                const float next_height = m_leftPaddle->transform()->position.y - 200.0f * dt;
                m_leftPaddle->transform()->position.y = getNewPaddleHeight(m_leftPaddle, next_height);
            }
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::S))
            {
                const float requested_height = m_leftPaddle->transform()->position.y + 200.0f * dt;
                m_leftPaddle->transform()->position.y = getNewPaddleHeight(m_leftPaddle, requested_height);
            }

            // Right paddle
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::Up))
            {
                const float next_height = m_rightPaddle->transform()->position.y - 200.0f * dt;
                m_rightPaddle->transform()->position.y = getNewPaddleHeight(m_rightPaddle, next_height);
            }
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::Down))
            {
                const float requested_height = m_rightPaddle->transform()->position.y + 200.0f * dt;
                m_rightPaddle->transform()->position.y = getNewPaddleHeight(m_rightPaddle, requested_height);
            }

            // Update paddle speed from score
            if (const float score = getScore(); m_lastScore != score)
            {
                m_hitSound.play();
                m_ball->velocity += (score - m_lastScore) * 5;
                m_lastScore = score;
            }
        }

        void onDestroyed() override
        {
            m_ball->onLoose.disconnect(m_looseSlotKey);
            m_menuSound.stop();
        }

        /**
         * \brief Resets the game (paddles and ball positions).
         */
        void reset()
        {
            const spark::math::Vector2 window_size = spark::core::Application::Instance()->window().size().castTo<float>();

            m_leftPaddle->transform()->position = {10, window_size.y / 2 - 50};
            m_rightPaddle->transform()->position = {window_size.x - 25 - 10, window_size.y / 2 - 50};
            m_ball->transform()->position = {window_size.x / 2 - 25, window_size.y / 2 - 25};
            m_ball->velocity = 250.0f;

            std::ranges::for_each(FindByName(root(), "Background")->componentsInChildren<ui::Score>(), [](ui::Score* score) { score->reset(); });
        }

    private:
        /**
         * \brief Calculates the new height of the paddle from the desired position handling the screen bounds.
         * \param paddle A pointer to the paddle for which the height is calculated.
         * \param next_height The desired height of the paddle.
         * \return The new height of the top of the paddle.
         */
        [[nodiscard]] static float getNewPaddleHeight(const GameObject* paddle, const float next_height)
        {
            const auto paddle_rect = paddle->component<spark::engine::components::Rectangle>();
            const auto screen_height = static_cast<float>(spark::core::Application::Instance()->window().height());

            if (next_height < 0)
                return 0;
            if (next_height + paddle_rect->size.y > screen_height)
                return screen_height - paddle_rect->size.y;
            return next_height;
        }

        /**
         * \brief Calculates the total score of the game (sum of the scores of the players).
         * \return The total score of the game.
         */
        [[nodiscard]] float getScore()
        {
            const auto scores = FindByName(root(), "Background")->componentsInChildren<ui::Score>();
            return std::accumulate(scores.begin(),
                                   scores.end(),
                                   0.0f,
                                   [](const float acc, const class ui::Score* score)
                                   {
                                       return acc + static_cast<float>(score->getScore());
                                   });
        }

    private:
        std::size_t m_looseSlotKey = 0;
        float m_lastScore = 0;
        GameObject *m_leftPaddle = nullptr, *m_rightPaddle = nullptr;
        spark::audio::Sound m_hitSound, m_looseSound, m_menuSound;
        Ball* m_ball = nullptr;
    };
}

IMPLEMENT_SPARK_RTTI(pong::GameManager)
