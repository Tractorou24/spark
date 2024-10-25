#pragma once

#include "spark/core/GameObject.h"
#include "spark/math/Vector2.h"

#include <string>

namespace pathfinding
{
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
        Grid* m_grid = nullptr;
        class Cell* m_selectedCell = nullptr;
    };
}

IMPLEMENT_SPARK_RTTI(pathfinding::GameManager)
