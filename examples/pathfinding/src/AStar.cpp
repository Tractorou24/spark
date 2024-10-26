#include "pathfinding/AStar.h"
#include "pathfinding/Cell.h"

#include "spark/math/Vector2.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <ranges>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pathfinding
{
    AStar::AStar(std::vector<std::vector<Cell*>> cells)
        : m_cells(std::move(cells))
    {
        for (auto* cell : m_cells | std::views::join)
            cell->reset();
    }

    void AStar::compute(Cell& start, Cell& end)
    {
        SPARK_ASSERT(m_computed.empty() && "Cells are already computed before computation started")
        SPARK_ASSERT(m_visited.empty() && "Cells are already visited before computation started")

        using Coordinate = spark::math::Vector2<std::size_t>;
        std::unordered_map<Coordinate, std::optional<Coordinate>> parent_chain;

        constexpr auto insert_sorted = [&](Cell* cell, auto& container)
        {
            constexpr auto sort = [](const Cell* lhs, const Cell* rhs)
            {
                const auto [_1, _2, lhs_weight] = lhs->weights();
                const auto [_3, _4, rhs_weight] = rhs->weights();
                return lhs_weight < rhs_weight;
            };

            container.push_back(cell);
            std::sort(container.begin(), container.end(), sort);
        };

        // Initialize the start cell
        insert_sorted(&start, m_computed);
        parent_chain[start.position()] = std::nullopt;
        SPARK_ASSERT(m_computed.size() == 1 && "Start cell is not computed")

        // Start the algorithm until a path is found or there is no more nodes to compute (means no path available).
        bool is_found = false;
        while (!is_found)
        {
            if (m_computed.empty())
                break;

            auto& current = **m_computed.begin();
            m_computed.erase(m_computed.begin());
            insert_sorted(&current, m_visited);

            // If the end is found
            if (&current == &end)
            {
                is_found = true;
                break;
            }

            // Get the neighbors
            for (auto* neighbor : current.neighbors(m_cells))
            {
                // If the neighbor is not traversable or already visited, continue (e.g. is an obstacle)
                if (neighbor->status() == Cell::Status::Obstacle || std::ranges::find(m_visited, neighbor) != m_visited.end())
                    continue;

                const auto [weight_start, weight_end, total_weight] = calculateWeights(start, end, *neighbor);
                // If the path is shorter or the cell is not computed yet, compute it
                if (total_weight < std::get<2>(current.weights()) || std::ranges::find(m_computed, neighbor) == m_computed.end())
                {
                    neighbor->setWeights(weight_start, weight_end);
                    parent_chain[neighbor->position()] = current.position();
                    if (std::ranges::find(m_computed, neighbor) == m_computed.end())
                        insert_sorted(neighbor, m_computed);
                }
            }
        }

        if (is_found)
        {
            const auto get_cell = [&, this](Cell* cell) { return m_cells[parent_chain[cell->position()].value().x][parent_chain[cell->position()].value().y]; };
            for (auto* cell = &end; cell != nullptr; cell = parent_chain[cell->position()].has_value() ? get_cell(cell) : nullptr)
                if (cell->status() != Cell::Status::Input && cell->status() != Cell::Status::Output)
                    cell->setStatus(Cell::Status::Path);
        } else
            spark::log::info("No path found!");
    }

    auto AStar::calculateWeights(Cell& start, Cell& end, Cell& cell) const -> decltype(cell.weights())
    {
        /// Calculate the weight of a single cell against another
        const auto single_weight = [&cell](Cell& other)
        {
            const std::size_t dx = cell.position().x > other.position().x
                                       ? cell.position().x - other.position().x
                                       : other.position().x - cell.position().x;

            const std::size_t dy = cell.position().y > other.position().y
                                       ? cell.position().y - other.position().y
                                       : other.position().y - cell.position().y;

            if (dx > dy)
                return 14 * dy + 10 * (dx - dy);
            return 14 * dx + 10 * (dy - dx);
        };

        // Calculate the weights
        const auto start_weight = single_weight(start);
        const auto end_weight = single_weight(end);
        return std::make_tuple(start_weight, end_weight, start_weight + end_weight);
    }
}
