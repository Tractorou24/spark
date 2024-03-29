#pragma once

#include "pong/ui/Background.h"

#include "experimental/ser/FileSerializer.h"
#include "spark/audio/Sound.h"
#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/SceneManager.h"
#include "spark/core/components/Rectangle.h"
#include "spark/core/components/Transform.h"
#include "spark/lib/UuidGenerator.h"
#include "spark/log/Logger.h"
#include "spark/path/Paths.h"

#include <numeric>

namespace pong
{
    /**
     * \brief The pong game manager handling the game logic.
     */
    class GameManager final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(GameManager, GameObject)
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
        explicit GameManager(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent), m_hitSound(spark::path::assets_path() / "hit.ogg"), m_looseSound(spark::path::assets_path() / "loose.ogg"),
              m_menuSound(spark::path::assets_path() / "menu.ogg") {}

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
            m_loadGameSlotId = spark::core::Input::keyPressedEvents[spark::base::KeyCodes::O].connect([]
            {
                // Get last game file
                const auto saves_path = spark::path::assets_path() / "saves";

                if (!std::filesystem::exists(saves_path))
                {
                    spark::log::info("Saves directory not found, creating it.");
                    std::filesystem::create_directory(saves_path);
                }

                std::set<std::filesystem::path, decltype([](const std::filesystem::path& lhs, const std::filesystem::path& rhs)
                {
                    return std::filesystem::last_write_time(lhs) > std::filesystem::last_write_time(rhs);
                })> files;
                for (const auto& entry : std::filesystem::directory_iterator(saves_path))
                    if (entry.is_regular_file())
                        files.insert(entry.path());

                if (files.empty())
                {
                    spark::log::error("No save found in {}", saves_path.generic_string());
                    return;
                }

                spark::log::info("Loading save {}", files.begin()->filename().string());

                // Load the game scene
                spark::core::SceneManager::LoadScene("Game");

                // Deserialize it into the loaded scene (so it does not call reset())
                experimental::ser::FileSerializer deserializer(*files.begin(), true);
                deserializer >> *spark::core::SceneManager::Scene("Game").get();
            });
            m_saveSlotKey = spark::core::Input::keyPressedEvents[spark::base::KeyCodes::P].connect([]
            {
                // Generate the save file
                const auto saves_path = spark::path::assets_path() / "saves";
                if (!std::filesystem::exists(saves_path))
                {
                    spark::log::info("Saves directory not found, creating it.");
                    std::filesystem::create_directory(saves_path);
                }

                const auto file_path = saves_path / spark::lib::UuidGenerator().generate().str();
                spark::log::info("Saving game to {}", file_path.filename().string());

                // Write it
                experimental::ser::FileSerializer serializer(file_path, false);
                serializer << *spark::core::SceneManager::Scene("Game").get();
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
                m_leftPaddle->transform()->position.y = newPaddleHeight(m_leftPaddle, next_height);
            }
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::S))
            {
                const float requested_height = m_leftPaddle->transform()->position.y + 200.0f * dt;
                m_leftPaddle->transform()->position.y = newPaddleHeight(m_leftPaddle, requested_height);
            }

            // Right paddle
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::Up))
            {
                const float next_height = m_rightPaddle->transform()->position.y - 200.0f * dt;
                m_rightPaddle->transform()->position.y = newPaddleHeight(m_rightPaddle, next_height);
            }
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::Down))
            {
                const float requested_height = m_rightPaddle->transform()->position.y + 200.0f * dt;
                m_rightPaddle->transform()->position.y = newPaddleHeight(m_rightPaddle, requested_height);
            }

            // Update paddle speed from score
            if (const float current_score = score(); m_lastScore != current_score)
            {
                m_hitSound.play();
                m_ball->velocity += (current_score - m_lastScore) * 5;
                m_lastScore = current_score;
            }
        }

        void onDestroyed() override
        {
            m_ball->onLoose.disconnect(m_looseSlotKey);
            spark::core::Input::keyPressedEvents[spark::base::KeyCodes::O].disconnect(m_loadGameSlotId);
            spark::core::Input::keyPressedEvents[spark::base::KeyCodes::P].disconnect(m_saveSlotKey);
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
        [[nodiscard]] static float newPaddleHeight(const GameObject* paddle, const float next_height)
        {
            const auto paddle_rect = paddle->component<spark::core::components::Rectangle>();
            const auto screen_height = spark::core::Application::Instance()->window().size().castTo<float>().y;

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
        [[nodiscard]] float score()
        {
            const auto scores = FindByName(root(), "Background")->componentsInChildren<ui::Score>();
            return std::accumulate(scores.begin(),
                                   scores.end(),
                                   0.0f,
                                   [](const float acc, const class ui::Score* score)
                                   {
                                       return acc + static_cast<float>(score->value());
                                   });
        }

    private:
        std::size_t m_looseSlotKey = 0, m_loadGameSlotId = 0, m_saveSlotKey = 0;
        float m_lastScore = 0;
        GameObject *m_leftPaddle = nullptr, *m_rightPaddle = nullptr;
        spark::audio::Sound m_hitSound, m_looseSound, m_menuSound;
        Ball* m_ball = nullptr;
    };
}

IMPLEMENT_SPARK_RTTI(pong::GameManager)

SPARK_SERIALIZE_RTTI_CLASS(pong::GameManager, m_lastScore)
