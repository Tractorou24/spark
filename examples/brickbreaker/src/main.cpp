#include "brickbreaker/Ball.h"
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
    auto app = spark::core::ApplicationBuilder().setName("BrickBreaker").setSize(1280, 720).build();

    {
        auto* root = spark::core::GameObject::Instantiate("Scene Root", nullptr);

        // Add the collisions around the screen
        brickbreaker::ScreenBorder::Top("Top Border", root);
        brickbreaker::ScreenBorder::Left("Left Border", root);
        brickbreaker::ScreenBorder::Right("Right Border", root);

        // Add the game objects
        spark::core::GameObject::Instantiate<brickbreaker::Paddle>("Paddle", root, spark::math::Vector2<float> {150, 25});
        spark::core::GameObject::Instantiate<brickbreaker::Ball>("Ball", root, 15.0f);

        // Register the main game scene
        auto scene = std::make_unique<spark::core::Scene>(root);
        spark::core::SceneManager::RegisterScene("Game", std::move(scene));
    }

    spark::core::SceneManager::LoadScene("Game");
    return app;
}
