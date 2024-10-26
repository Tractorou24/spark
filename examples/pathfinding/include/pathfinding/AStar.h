#pragma once

#include "pathfinding/Cell.h"

#include <vector>

namespace pathfinding
{
    /**
     * \brief Implements the A* algorithm to find the shortest path between two points in a grid.
     */
    class AStar
    {
    public:
        /**
         * \brief Constructs the A* algorithm.
         * \param cells The list of all cells arranged in a 2D array.
         *
         * \note Constructing the algorithm does not compute anything, use the \ref compute() method for this.
         */
        explicit AStar(std::vector<std::vector<Cell*>> cells);

        /**
         * \brief Computes the algorithm, finding a path from \p start to \p end.
         * \param start The start cell.
         * \param end The end cell.
         */
        void compute(Cell& start, Cell& end);

        /**
         * \brief Calculates the weights of \p cell depending on the start and end cells.
         * \param start The start cell.
         * \param end The end cell.
         * \param cell The cell to calculate the weights for.
         * \return The weights of the cell.
         */
        auto calculateWeights(Cell& start, Cell& end, Cell& cell) const -> decltype(cell.weights());

    private:
        std::vector<std::vector<Cell*>> m_cells;
        std::vector<Cell*> m_computed;
        std::vector<Cell*> m_visited;
    };
}
