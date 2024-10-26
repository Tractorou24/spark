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

    void Cell::reset()
    {
        setWeights(0, 0);
        if (m_status == Status::Path)
            setStatus(Status::None);
    }

    spark::math::Vector2<std::size_t>& Cell::position()
    {
        return m_position;
    }

    void Cell::setStatus(const Status status)
    {
        m_status = status;
        auto* rectangle = component<spark::core::components::Rectangle>();
        switch (m_status)
        {
        case Status::Obstacle:
            rectangle->color = {0.f, 0.f, 0.f, 1.f};
            break;
        case Status::Input:
            rectangle->color = {246.f / 255.f, 250.f / 255.f, 150.f / 255.f, 1.f};
            break;
        case Status::Output:
            rectangle->color = {169.f / 255.f, 201.f / 255.f, 196.f / 255.f, 1.f};
            break;
        case Status::Path:
            rectangle->color = {0.f, 0.f, 1.f, 1.f};
            break;
        default:
            rectangle->color = {1.f, 1.f, 1.f, 1.f};
        }
    }

    Cell::Status Cell::status() const
    {
        return m_status;
    }

    std::vector<Cell*> Cell::neighbors(const std::vector<std::vector<Cell*>>& cells)
    {
        std::vector<Cell*> result;
        const auto coordinates = position();

        // Define relative positions of all 8 possible neighbors as a constexpr std::array
        constexpr std::array<std::pair<int, int>, 8> directions = {
            {
                {-1, 0}, {1, 0}, {0, -1}, {0, 1},
                {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
            }
        };

        // Iterate through each direction and add if it exists
        for (const auto& [dx, dy] : directions)
        {
            if ((dx < 0 && coordinates.x == 0) || (dy < 0 && coordinates.y == 0))
                continue;

            const std::size_t nx = coordinates.x + dx;
            const std::size_t ny = coordinates.y + dy;

            if (nx < cells.size() && ny < cells[0].size())
                result.push_back(cells[nx][ny]);
        }
        return result;
    }

    void Cell::setWeights(const std::size_t start, const std::size_t dest)
    {
        m_startWeight = start;
        m_destWeight = dest;
    }

    std::tuple<std::size_t, std::size_t, std::size_t> Cell::weights() const
    {
        return std::make_tuple(m_startWeight, m_destWeight, m_startWeight + m_destWeight);
    }
}

std::string_view to_string(const pathfinding::Cell::Status status)
{
    switch (status)
    {
    case pathfinding::Cell::Status::Obstacle:
        return "Obstacle";
    case pathfinding::Cell::Status::Input:
        return "Input";
    case pathfinding::Cell::Status::Output:
        return "Output";
    case pathfinding::Cell::Status::Path:
        return "Path";
    default:
        break;
    }
    return "None";
}
