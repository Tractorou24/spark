#include "pathfinding/Cell.h"
#include "pathfinding/Grid.h"

#include "spark/base/Exception.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Rectangle.h"

#include <format>
#include <string>
#include <utility>

namespace pathfinding
{
    Cell::Cell(std::string name, spark::core::GameObject* parent, const unsigned size)
        : spark::core::GameObject(std::move(name), parent)
    {
        // Check the parent type (must be a grid)
        if (&parent->rttiInstance() != &Grid::classRtti())
            throw spark::base::TypeMismatchException(std::format("Cannot create cell {} with parent {} of type {}",
                                                                 GameObject::name(),
                                                                 parent->name(),
                                                                 parent->rttiInstance().className()));

        // Configure the render component
        addComponent<spark::core::components::Rectangle>();

        auto* rectangle = component<spark::core::components::Rectangle>();
        rectangle->size = {static_cast<float>(size), static_cast<float>(size)};
    }

    spark::math::Vector2<std::size_t>& Cell::position()
    {
        return m_position;
    }
}
