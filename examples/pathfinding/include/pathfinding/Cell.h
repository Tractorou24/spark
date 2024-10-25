#pragma once

#include "spark/core/GameObject.h"
#include "spark/math/Vector2.h"
#include "spark/patterns/Signal.h"
#include "spark/patterns/Traverser.h"

namespace pathfinding
{
    /**
     * \brief A single cell inside a \ref Grid.
     */
    class Cell final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Cell, GameObject)

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
         * \brief Gets the coordinates of the cell in its associated grid.
         * \return The `[X, Y]` position of the cell.
         */
        [[nodiscard]] spark::math::Vector2<std::size_t>& position();

    private:
        std::size_t m_mousePressedHandle = 0;
        spark::math::Vector2<std::size_t> m_position;
    };
}

IMPLEMENT_SPARK_RTTI(pathfinding::Cell)
