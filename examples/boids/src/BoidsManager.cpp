#include "boids/BoidsManager.h"
#include "boids/Bird.h"

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/lib/Random.h"
#include "spark/math/Vector2.h"

#include "imgui.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace boids
{
    BoidsManager::BoidsManager(std::string name, spark::core::GameObject* parent)
        : spark::core::GameObject(std::move(name), parent)
        , m_birdContainer(spark::core::GameObject::Instantiate("Birds", this))
    {
        adjustBirdCount();
    }

    void BoidsManager::adjustBirdCount()
    {
        const std::int64_t diff = boidsCount - static_cast<unsigned>(m_birdContainer->children().size());
        if (diff > 0)
        {
            const auto screen_size = spark::core::Application::Instance()->window().size().castTo<float>();
            for (std::int64_t i = 0; i < diff; ++i)
            {
                auto* bird = spark::core::GameObject::Instantiate<Bird>("Bird",
                                                                        m_birdContainer,
                                                                        spark::math::Vector2<float> {
                                                                            spark::lib::Random::Number(0.f, screen_size.x),
                                                                            spark::lib::Random::Number(0.f, screen_size.y)
                                                                        });
                m_birds[bird->cell()].push_back(bird);

                // When a cell is changed, remote from the old one and add to the new one.
                bird->onCellChanged.connect([&](Bird* bird, const std::size_t old_cell)
                {
                    const auto new_cell = bird->cell();

                    SPARK_ASSERT(old_cell != new_cell && "Bird cell should have changed if 'onCellChanged' is emitted, but it didn't.");
                    SPARK_ASSERT(std::ranges::find(m_birds[old_cell], bird) != m_birds[old_cell].end()
                                 && "Bird should be in the old cell when 'onCellChanged' is emitted, but it isn't.");

                    m_birds[new_cell].push_back(bird);
                });
            }
        } else
        {
            for (std::int64_t i = 0; i < -diff; ++i)
            {
                auto& bird = static_cast<Bird&>(*m_birdContainer->children().back());
                m_birds[bird.cell()].remove(&bird);
                spark::core::GameObject::Destroy(&bird);
            }
        }
    }

    void BoidsManager::onUpdate(const float /*dt*/)
    {
        ImGui::Begin("Data");

        if (ImGui::SliderInt("Birds Count", reinterpret_cast<int*>(&boidsCount), 1, 100))
            adjustBirdCount();

        ImGui::End();
    }
}
