#include "boids/BoidsManager.h"
#include "boids/Bird.h"
#include "boids/SimulationData.h"

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/lib/Random.h"
#include "spark/math/Vector2.h"

#include "imgui.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace boids
{
    BoidsManager::BoidsManager(std::string name, spark::core::GameObject* parent)
        : spark::core::GameObject(std::move(name), parent)
    {
        adjustBirdCount();
    }

    void BoidsManager::adjustBirdCount()
    {
        const std::int64_t diff = static_cast<std::int32_t>(boidsCount) - static_cast<std::int32_t>(children().size());
        if (diff > 0)
        {
            const auto screen_size = spark::core::Application::Instance()->window().size().castTo<float>();
            for (std::int64_t i = 0; i < diff; ++i)
            {
                static std::size_t counter = 0;
                auto* bird = spark::core::GameObject::Instantiate<Bird>(std::format("Bird {}", counter),
                                                                        this,
                                                                        spark::math::Vector2<float> {
                                                                            spark::lib::Random::Number(0.f, screen_size.x),
                                                                            spark::lib::Random::Number(0.f, screen_size.y)
                                                                        },
                                                                        &simulationData,
                                                                        [&](const std::size_t cell) -> std::list<Bird*>
                                                                        {
                                                                            return m_birds.at(cell);
                                                                        });
                m_birds[bird->cell()].push_back(bird);

                // When a cell is changed, remove from the old one and add to the new one.
                bird->onCellChanged.connect([&](Bird* bird, const std::size_t old_cell)
                {
                    const auto new_cell = bird->cell();

                    SPARK_ASSERT(old_cell != new_cell && "Bird cell should have changed if 'onCellChanged' is emitted, but it didn't.");
                    SPARK_ASSERT(std::ranges::find(m_birds[old_cell], bird) != m_birds[old_cell].end()
                                 && "Bird should be in the old cell when 'onCellChanged' is emitted, but it isn't.");

                    [[maybe_unused]] const auto removed = m_birds[old_cell].remove(bird);
                    SPARK_ASSERT(removed == 1 && "The bird should have been removed from the cell, but it wasn't.");

                    m_birds[new_cell].push_back(bird);
                });
            }
        } else
        {
            for (std::int64_t i = 0; i < -diff; ++i)
            {
                for (auto& birds : m_birds | std::views::values)
                {
                    if (!birds.empty())
                    {
                        auto* bird = birds.front();
                        [[maybe_unused]] const auto removed = m_birds[bird->cell()].remove(bird);
                        SPARK_ASSERT(removed == 1 && "The bird should have been removed from the cell, but it wasn't.");

                        spark::core::GameObject::Destroy(bird);
                        break;
                    }
                }
            }
        }
    }

    void BoidsManager::adjustCellCount()
    {
        for (const auto& birds : m_birds | std::views::values)
            for (auto* bird : birds)
                bird->setCellCount(cellCount);
    }

    void BoidsManager::onUpdate(const float /*dt*/)
    {
        ImGui::Begin("Data");

        if (ImGui::SliderInt("Birds Count", reinterpret_cast<int*>(&boidsCount), 1, 100))
            adjustBirdCount();
        if (ImGui::SliderInt("Cell Count", reinterpret_cast<int*>(&cellCount), 5, 500))
            adjustCellCount();

        ImGui::SliderFloat("Separation Weight", &simulationData.separationWeight, 0.f, 10.f);
        ImGui::SliderFloat("Alignment Weight", &simulationData.alignmentWeight, 0.f, 10.f);
        ImGui::SliderFloat("Cohesion Weight", &simulationData.cohesionWeight, 0.f, 10.f);
        ImGui::SliderFloat("Max Speed", &simulationData.maxSpeed, 0.f, 500.f);
        ImGui::SliderFloat("Max Distance", &simulationData.maxDistance, 0.f, 500.f);
        ImGui::SliderFloat("Field Of View", &simulationData.fieldOfView, 0.f, 360.f);

        ImGui::End();
    }
}
