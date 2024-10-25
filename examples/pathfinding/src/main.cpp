#include "pathfinding/GameManager.h"
#include "pathfinding/Grid.h"

#include "spark/core/Application.h"
#include "spark/core/ApplicationBuilder.h"
#include "spark/core/EntryPoint.h"
#include "spark/core/GameObject.h"
#include "spark/core/SceneManager.h"
#include "spark/imgui/ImGui.h"

#include "imgui.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

std::unique_ptr<spark::core::Application> spark_main(const std::vector<std::string>& /*args*/)
{
    auto app = spark::core::ApplicationBuilder().setName("PathFinding").setSize(1280, 720).build();
    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(spark::imgui::context()));

    // Create the main scene
    {
        auto* root = spark::core::GameObject::Instantiate("Scene Root", nullptr);
        auto* grid = spark::core::GameObject::Instantiate<pathfinding::Grid>("Grid", root);
        spark::core::GameObject::Instantiate<pathfinding::GameManager>("Manager", root, grid);

        auto scene = std::make_unique<spark::core::Scene>(root);
        spark::core::SceneManager::RegisterScene("Game", std::move(scene));
    }

    spark::core::SceneManager::LoadScene("Game");
    return app;
}
