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
            if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::LShift)) // Input mode (LShift + Click)
                setIo(cell, true);
            else if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::LAlt)) // Output mode (LCtrl + Click)
                setIo(cell, false);
            else if (spark::core::Input::IsKeyPressed(spark::base::KeyCodes::LControl)) // Selection mode (LCtrl + Click)
                m_selectedCell = &cell;
            else // Obstacle mode (Simple click)
            {
                if (cell.status() != Cell::Status::Obstacle)
                    cell.setStatus(Cell::Status::Obstacle);
                else
                    cell.setStatus(Cell::Status::None);
            }
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
        ImGui::Text("LShift+Click on a cell to set it as an input");
        ImGui::Text("LAlt+Click on a cell to set it as an output");

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
            // Reset all, since it can be deleted (dangling ptr)
            m_selectedCell = nullptr;
            m_inputCell = nullptr;
            m_outputCell = nullptr;

            // Resize the grid
            m_grid->resize(gridSize, cellSize, cellBorderSize);
        }
    }

    void GameManager::setIo(Cell& cell, const bool is_input)
    {
        Cell** local_cell = is_input ? &m_inputCell : &m_outputCell;
        Cell** other_cell = is_input ? &m_outputCell : &m_inputCell;

        // Opposite IO is already on the same cell
        if (*other_cell == &cell)
        {
            (*other_cell)->setStatus(Cell::Status::None);
            *other_cell = nullptr;
        }

        // The right IO type is already placed on the grid
        if (*local_cell != nullptr)
            (*local_cell)->setStatus(Cell::Status::None);

        cell.setStatus(is_input ? Cell::Status::Input : Cell::Status::Output);
        *local_cell = &cell;
    }
}
