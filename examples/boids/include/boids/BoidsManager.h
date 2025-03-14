#pragma once

#include "boids/Bird.h"

#include "spark/core/GameObject.h"
#include "spark/lib/Random.h"

#include "imgui.h"

namespace boids
{
    /**
     * \brief The manager of the boids simulation. Holds the global settings.
     */
    class BoidsManager final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(BoidsManager, GameObject)

    public:
        unsigned boidsCount = 10;

    public:
        explicit BoidsManager(std::string name, spark::core::GameObject* parent)
            : spark::core::GameObject(std::move(name), parent)
            , m_birdContainer(spark::core::GameObject::Instantiate("Birds", this))
        {
            adjustBirdCount();
        }

        /**
         * \brief Adjusts the number of birds in the scene, corresponding to the value of \ref boidsCount.
         */
        void adjustBirdCount()
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

        void onUpdate(const float /*dt*/) override
        {
            ImGui::Begin("Data");

            if (ImGui::SliderInt("Birds Count", reinterpret_cast<int*>(&boidsCount), 1, 100))
                adjustBirdCount();

            ImGui::End();
        }

    private:
        GameObject* m_birdContainer = nullptr;
        std::unordered_map<std::size_t, std::list<const Bird*>> m_birds;
    };
}

IMPLEMENT_SPARK_RTTI(boids::BoidsManager)
