#pragma once

#include "spark/core/GameObject.h"
#include "spark/math/Vector2.h"

#include <optional>
#include <string>

namespace pathfinding
{
    class Cell;

    /**
     * \brief The manager responsible for holding global settings 
     */
    class GameManager final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(GameManager, GameObject)

    public:
        /// \brief The size of the grid in cells
        spark::math::Vector2<unsigned> gridSize = {10, 10};

        /// \brief The size of each cell in the grid in px.
        unsigned cellSize = 25;

        /// \brief The offset between each cell.
        unsigned cellBorderSize = 5;

    public:
        /**
         * \brief Creates a new grid manager with underlying grid.
         * \param name The name of the object.
         * \param parent The parent object in the scene.
         * \param grid The grid used to display the cells.
         */
        explicit GameManager(std::string name, spark::core::GameObject* parent, class Grid* grid);
        ~GameManager() override = default;

        /**
         * \brief Updates the grid and shows settings.
         * \param dt The delta time since the last frame.
         */
        void onUpdate(float dt) override;

    private:
        /**
         * \brief Sets the input or output cell depending on \p is_input to \p cell. 
         * \param cell The cell to set as IO.
         * \param is_input `true` if the cell should be an input, `false` if it should be an output.
         */
        void setIo(Cell& cell, bool is_input);

        /**
         * \brief Checks if the grid is in a state that can be computed.
         * \return `std::nullopt` if the grid is valid, otherwise a vector of strings with the errors (ready to be shown).
         *
         * To have a valid grid, it must have an input and an output cell.
         */
        [[nodiscard]] std::optional<std::vector<std::string>> canCompute() const;

        /**
         * \brief Computes the path from the input to the output cell using the A* algorithm.
         */
        void compute();

    private:
        Grid* m_grid = nullptr;
        Cell* m_selectedCell = nullptr;
        Cell *m_inputCell = nullptr, *m_outputCell = nullptr;
    };
}

IMPLEMENT_SPARK_RTTI(pathfinding::GameManager)
