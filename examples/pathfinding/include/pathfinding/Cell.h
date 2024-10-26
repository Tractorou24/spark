#pragma once

#include "spark/core/GameObject.h"
#include "spark/math/Vector2.h"
#include "spark/patterns/Signal.h"
#include "spark/patterns/Traverser.h"

#include <string_view>

namespace pathfinding
{
    /**
     * \brief A single cell inside a \ref Grid.
     */
    class Cell final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Cell, GameObject)

    public:
        /// \brief The status of the cell (defining shown color)
        enum class Status : std::uint8_t
        {
            None,
            Obstacle,
            Input,
            Output,
            Path
        };

    public:
        /// \brief The signal emitted when the cell is clicked.
        spark::patterns::Signal<Cell&> onClicked;

    public:
        /**
         * \brief Creates a new square cell with the specified \p size.
         * \param name The name of the cell.
         * \param parent The parent of the cell (usually a \ref Grid).
         * \param size The size in pixels (XY) of the cell.
         */
        explicit Cell(std::string name, spark::core::GameObject* parent, unsigned size);

        void onSpawn() override;
        void onDestroyed() override;

        /**
         * \brief Resets a cell to a valid status before a computation. (weights & status)
         */
        void reset();

        /**
         * \brief Gets the coordinates of the cell in its associated grid.
         * \return The `[X, Y]` position of the cell.
         */
        [[nodiscard]] spark::math::Vector2<std::size_t>& position();

        /**
         * \brief Sets the new status of the cell.
         * \param status The new status of the cell.
         */
        void setStatus(Status status);

        /**
         * \brief Gets the status of the cell.
         * \return The cell status.
         */
        [[nodiscard]] Status status() const;

        /**
         * \brief Get all neighbors of this cell (left, right, top, bottom + diagonals).
         * \param cells The list of all cells arranged in a 2D array (where `cells[position().x][position().y] == this`).
         * \return A \ref std::vector with all neighbors.
         */
        [[nodiscard]] std::vector<Cell*> neighbors(const std::vector<std::vector<Cell*>>& cells);

        /**
         * \brief Sets the weights of the cell.
         * \param start The weight from the start node.
         * \param dest The weight from the destination node.
         */
        void setWeights(std::size_t start, std::size_t dest);

        /**
         * \brief Gets all the weights of the current cell.
         * \return The start, destination and total weights of the cell.
         */
        [[nodiscard]] std::tuple<std::size_t, std::size_t, std::size_t> weights() const;

    private:
        std::size_t m_mousePressedHandle = 0;
        spark::math::Vector2<std::size_t> m_position;
        Status m_status = Status::None;
        std::size_t m_startWeight = 0, m_destWeight = 0;
    };
}

/// \brief Converts the given cell \p status to a \ref std::string_view.
std::string_view to_string(pathfinding::Cell::Status status);

IMPLEMENT_SPARK_RTTI(pathfinding::Cell)
