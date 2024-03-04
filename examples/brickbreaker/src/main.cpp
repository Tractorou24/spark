#include "brickbreaker/Ball.h"
#include "brickbreaker/Brick.h"
#include "brickbreaker/Paddle.h"
#include "brickbreaker/ScreenBorder.h"

#include "spark/core/Application.h"
#include "spark/core/ApplicationBuilder.h"
#include "spark/core/EntryPoint.h"
#include "spark/core/GameObject.h"
#include "spark/core/Scene.h"
#include "spark/core/SceneManager.h"

std::unique_ptr<spark::core::Application> spark_main(const std::vector<std::string>& /*args*/)
{
    static constexpr spark::math::Vector2 window_size = {1280u, 720u};
    static constexpr spark::math::Vector2 brick_count = {7u, 10u};
    static constexpr spark::math::Vector2 brick_size = {100.f, 40.f};

    auto app = spark::core::ApplicationBuilder().setName("BrickBreaker").setSize(window_size.x, window_size.y).build();

    {
        auto* root = spark::core::GameObject::Instantiate("Scene Root", nullptr);

        // Add the collisions around the screen
        brickbreaker::ScreenBorder::Top("Top Border", root);
        brickbreaker::ScreenBorder::Left("Left Border", root);
        brickbreaker::ScreenBorder::Right("Right Border", root);

        // Add the game objects
        spark::core::GameObject::Instantiate<brickbreaker::Paddle>("Paddle", root, spark::math::Vector2<float> {150, 25});
        spark::core::GameObject::Instantiate<brickbreaker::Ball>("Ball", root, 15.0f);

        auto* brick_container = spark::core::GameObject::Instantiate<spark::core::GameObject>("Brick Container", root);
        for (unsigned row = 0; row < brick_count.x; row++)
        {
            for (unsigned column = 0; column < brick_count.y; column++)
            {
                const auto* brick = spark::core::GameObject::Instantiate<brickbreaker::Brick>(std::format("Brick{}{}", row, column), brick_container, brick_size);
                brick->transform()->position = spark::math::Vector2(window_size.x / 8 + column * brick_size.x + column * 2, window_size.y / 8 + row * brick_size.y + row * 2);
            }
        }

        // Register the main game scene
        auto scene = std::make_unique<spark::core::Scene>(root);
        spark::core::SceneManager::RegisterScene("Game", std::move(scene));
    }

    spark::core::SceneManager::LoadScene("Game");
    return app;
}
