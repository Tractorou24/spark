#include "pathfinding/Cell.h"
#include "pathfinding/Grid.h"

#include "spark/base/Exception.h"
#include "spark/base/MouseCodes.h"
#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/components/Rectangle.h"
#include "spark/core/components/Transform.h"
#include "spark/math/Vector2.h"

#include <cstddef>
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

    void Cell::onSpawn()
    {
        // Allow clicking
        m_mousePressedHandle = spark::core::Input::mousePressedEvents[spark::base::MouseCodes::Left].connect([this]
        {
            const auto mouse_position = spark::core::Input::MousePosition();
            const auto* rectangle = component<spark::core::components::Rectangle>();

            const bool is_contained = mouse_position.x >= transform()->position.x && mouse_position.x <= transform()->position.x + rectangle->size.x &&
                    mouse_position.y >= transform()->position.y && mouse_position.y <= transform()->position.y + rectangle->size.y;

            if (is_contained)
                onClicked.emit(*this);
        });
    }

    void Cell::onDestroyed()
    {
        spark::core::Input::mousePressedEvents[spark::base::MouseCodes::Left].disconnect(m_mousePressedHandle);
    }

    spark::math::Vector2<std::size_t>& Cell::position()
    {
        return m_position;
    }
}
