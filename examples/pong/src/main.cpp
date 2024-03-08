#include "pong/Ball.h"
#include "pong/GameManager.h"
#include "pong/Paddle.h"
#include "pong/ui/Background.h"
#include "pong/ui/Menu.h"

#include "spark/core/Application.h"
#include "spark/core/ApplicationBuilder.h"
#include "spark/core/EntryPoint.h"
#include "spark/core/SceneManager.h"
#include "spark/core/GameObject.h"
#include "spark/core/Scene.h"

std::unique_ptr<spark::core::Application> spark_main(const std::vector<std::string>& /*args*/)
{
    auto app = spark::core::ApplicationBuilder().setName("Pong").setSize(1280, 720).build();

    // Create the main menu scene
    {
        auto* root = spark::core::GameObject::Instantiate("Scene Root", nullptr);
        spark::core::GameObject::Instantiate<pong::ui::Menu>("Menu", root);

        auto scene = std::make_unique<spark::core::Scene>(root);
        spark::core::SceneManager::RegisterScene("MainMenu", std::move(scene));
    }

    // Create the game scene
    {
        auto* root = spark::core::GameObject::Instantiate("Scene Root", nullptr);
        spark::core::GameObject::Instantiate<pong::GameManager>("Game Manager", root);
        spark::core::GameObject::Instantiate<pong::ui::Background>("Background", root);
        spark::core::GameObject::Instantiate<pong::Paddle>("Left Paddle", root);
        spark::core::GameObject::Instantiate<pong::Paddle>("Right Paddle", root);
        spark::core::GameObject::Instantiate<pong::Ball>("Ball", root, 15.0f);

        // Set the scene
        auto scene = std::make_unique<spark::core::Scene>(root);
        spark::core::SceneManager::RegisterScene("Game", std::move(scene));
    }

    spark::core::SceneManager::LoadScene("MainMenu");
    return app;
}
