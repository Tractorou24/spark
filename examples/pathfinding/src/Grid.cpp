#include "pathfinding/Grid.h"
#include "pathfinding/Cell.h"

#include "spark/core/GameObject.h"
#include "spark/core/components/Transform.h"
#include "spark/math/Vector2.h"

#include <cstddef>
#include <string>
#include <utility>

namespace pathfinding
{
    Grid::Grid(std::string name, spark::core::GameObject* parent)
        : spark::core::GameObject(std::move(name), parent) {}

    void Grid::resize(const spark::math::Vector2<unsigned>& size, const unsigned cell_size, const unsigned cell_offset)
    {
        // Delete all cells
        for (auto* cell : GameObject::children())
            Destroy(cell);

        // Recreate them
        for (std::size_t i = 0; i < size.x; i++)
        {
            for (std::size_t j = 0; j < size.y; j++)
            {
                auto* cell = Instantiate<Cell>(std::format("Cell {}x{}", j, i), this, cell_size);
                cell->position() = {j, i};
                cell->transform()->position = {static_cast<float>(j * cell_size + j * cell_offset), static_cast<float>(i * cell_size + i * cell_offset)};
                cell->onClicked.connect([this](Cell& c) { onCellClicked.emit(c); });
            }
        }
        m_gridSize = size;
    }

    std::vector<std::vector<Cell*>> Grid::cells()
    {
        std::vector<std::vector<Cell*>> cells(m_gridSize.x);
        for (std::size_t i = 0; i < m_gridSize.x; ++i)
            cells[i].resize(m_gridSize.y);

        for (auto* obj : GameObject::children())
        {
            const auto cell = static_cast<Cell*>(obj);
            cells[cell->position().x][cell->position().y] = cell;
        }

        return cells;
    }
}
