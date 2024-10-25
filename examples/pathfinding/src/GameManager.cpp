#include "pathfinding/GameManager.h"
#include "pathfinding/Grid.h"

#include "spark/core/GameObject.h"

#include "imgui.h"

#include <string>
#include <utility>

namespace pathfinding
{
    GameManager::GameManager(std::string name, spark::core::GameObject* parent, Grid* grid)
        : spark::core::GameObject(std::move(name), parent), m_grid(grid)
    {
        m_grid->resize(gridSize, cellSize, cellBorderSize);
    }

    void GameManager::onUpdate(const float dt)
    {
        GameObject::onUpdate(dt);

        ImGui::Begin("Path Finding Data");

        bool should_resize = false;

        // Grid settings
        ImGui::SeparatorText("Grid");
        should_resize |= ImGui::SliderInt2("Size", reinterpret_cast<int*>(&gridSize.x), 1, 50);
        should_resize |= ImGui::SliderInt("Cell Size", reinterpret_cast<int*>(&cellSize), 1, 50);
        should_resize |= ImGui::SliderInt("Cell Offset:", reinterpret_cast<int*>(&cellBorderSize), 1, 10);
        ImGui::Text("Cell Count: %u", gridSize.x * gridSize.y);

        ImGui::End();

        if (should_resize)
            m_grid->resize(gridSize, cellSize, cellBorderSize);
    }
}
