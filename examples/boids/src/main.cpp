#include "boids/BoidsManager.h"

#include "spark/core/Application.h"
#include "spark/core/ApplicationBuilder.h"
#include "spark/core/EntryPoint.h"
#include "spark/core/GameObject.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

std::unique_ptr<spark::core::Application> spark_main(const std::vector<std::string>& /*args*/)
{
    auto app = spark::core::ApplicationBuilder().setName("Boids").setSize(1280, 720).build();
    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(spark::imgui::context()));

    // Create the game scene
    {
        auto* root = spark::core::GameObject::Instantiate("Root", nullptr);
        spark::core::GameObject::Instantiate<boids::BoidsManager>("BoidsManager", root);

        // Set the scene
        auto scene = std::make_unique<spark::core::Scene>(root);
        spark::core::SceneManager::RegisterScene("Main", std::move(scene));
    }

    spark::core::SceneManager::LoadScene("Main");
    return app;
}
