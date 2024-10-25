#include "pathfinding/GameManager.h"
#include "pathfinding/Cell.h"
#include "pathfinding/Grid.h"

#include "spark/base/KeyCodes.h"
#include "spark/core/GameObject.h"
#include "spark/core/Input.h"

#include "imgui.h"

#include <string>
#include <utility>

namespace pathfinding
{
    GameManager::GameManager(std::string name, spark::core::GameObject* parent, Grid* grid)
        : spark::core::GameObject(std::move(name), parent), m_grid(grid)
    {
        m_grid->resize(gridSize, cellSize, cellBorderSize);
        m_grid->onCellClicked.connect([this](Cell& cell)
        {
            if (!spark::core::Input::IsKeyPressed(spark::base::KeyCodes::LControl)) // Obstacle mode (Simple click)
            {
                if (cell.status() == Cell::Status::None)
                    cell.setStatus(Cell::Status::Obstacle);
                else if (cell.status() == Cell::Status::Obstacle)
                    cell.setStatus(Cell::Status::None);
            } else // Selection mode (LCtrl + Click)
                m_selectedCell = &cell;
        });
    }

    void GameManager::onUpdate(const float dt)
    {
        GameObject::onUpdate(dt);

        ImGui::Begin("Path Finding Data");

        bool should_resize = false;

        // Doc
        ImGui::Text("Click on a cell to set it as an obstacle");
        ImGui::Text("LCtrl+Click on a cell to see its data");

        // Grid settings
        ImGui::SeparatorText("Grid");
        should_resize |= ImGui::SliderInt2("Size", reinterpret_cast<int*>(&gridSize.x), 1, 50);
        should_resize |= ImGui::SliderInt("Cell Size", reinterpret_cast<int*>(&cellSize), 1, 50);
        should_resize |= ImGui::SliderInt("Cell Offset:", reinterpret_cast<int*>(&cellBorderSize), 1, 10);
        ImGui::Text("Cell Count: %u", gridSize.x * gridSize.y);

        // Cell data
        ImGui::SeparatorText("Current cell data");
        if (m_selectedCell != nullptr)
        {
            ImGui::Text("Position: %zu, %zu", m_selectedCell->position().x, m_selectedCell->position().y);
            ImGui::Text("Type: %s", to_string(m_selectedCell->status()).data());
        } else
            ImGui::Text("No cell selected");

        ImGui::End();

        if (should_resize)
        {
            m_selectedCell = nullptr; // Reset the selection, since it can be deleted (dangling ptr)
            m_grid->resize(gridSize, cellSize, cellBorderSize);
        }
    }
}
