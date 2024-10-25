#pragma once

#include "pathfinding/Cell.h"

#include "spark/core/GameObject.h"
#include "spark/math/Vector2.h"

namespace pathfinding
{
    /**
     * \brief A resizable rectangle grid holding \link Cell cells \endlink.
     */
    class Grid final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Grid, GameObject)

    public:
        /**
         * \brief Constructs a new grid with the specified \p size and \p cell_size.
         * \param name The name of the grid.
         * \param parent The parent object of the grid.
         */
        explicit Grid(std::string name, spark::core::GameObject* parent);

        /**
         * \brief Resizes the grid to the specified \p size and \p cell_size.
         * \param size The new size of the grid in cells.
         * \param cell_size The new size in pixels of each square cell.
         * \param cell_offset The new offset between each cell.
         */
        void resize(const spark::math::Vector2<unsigned>& size, unsigned cell_size, unsigned cell_offset);
    };
}

IMPLEMENT_SPARK_RTTI(pathfinding::Grid)
