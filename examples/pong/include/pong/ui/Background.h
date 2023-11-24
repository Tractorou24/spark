#pragma once

#include "spark/core/Application.h"
#include "spark/core/Renderer2D.h"
#include "spark/engine/GameObject.h"
#include "spark/math/Vector2.h"
#include "spark/path/Paths.h"
#include "spark/patterns/Signal.h"

namespace pong::ui
{
    /**
     * \brief A component that draws a vertical dashed line on the screen.
     */
    class DashedLine final : public spark::engine::Component
    {
        DECLARE_SPARK_RTTI(DashedLine, Component)

    public:
        constexpr static std::size_t lineLength = 20, verticalOffset = 20;

    public:
        explicit DashedLine(spark::engine::GameObject* parent, const std::size_t x_offset)
            : Component(parent), m_xOffset(x_offset) {}

        void render() const override
        {
            const auto window_size = spark::core::Application::Instance()->window().size().castTo<std::size_t>();

            for (std::size_t i = 0; i < window_size.y / (lineLength + verticalOffset); i++)
                spark::core::Renderer2D::DrawQuad({static_cast<float>(m_xOffset), static_cast<float>(i * lineLength + i * verticalOffset)}, {5, lineLength}, {255, 255, 255});
        }

    private:
        std::size_t m_xOffset = 0;
    };

    /**
     * \brief A component that draws the score on the screen.
     */
    class Score final : public spark::engine::Component
    {
        DECLARE_SPARK_RTTI(Score, Component)

    public:
        spark::patterns::Signal<std::size_t> incrementScore;

    public:
        explicit Score(spark::engine::GameObject* parent, const spark::math::Vector2<float> position)
            : Component(parent), m_position(position)
        {
            incrementScore.connect([this](const std::size_t score) { m_currentScore += score; });
        }

        void render() const override
        {
            const auto score = std::to_string(m_currentScore);
            spark::core::Renderer2D::DrawText(score, m_position, 100, spark::path::assets_path() / "font.ttf");
        }

        /**
         * \brief Gets the current score.
         * \return A std::size_t representing the current score.
         */
        [[nodiscard]] std::size_t getScore() const noexcept { return m_currentScore; }

        /**
         * \brief Resets the score to 0.
         */
        void reset() { m_currentScore = 0; }

    private:
        std::size_t m_currentScore = 0;
        spark::math::Vector2<float> m_position;
    };

    /**
     * \brief The background of the game with the dashed line and the score.
     */
    class Background final : public spark::engine::GameObject
    {
        DECLARE_SPARK_RTTI(Background, GameObject)

    public:
        explicit Background(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent)
        {
            const auto window_size = spark::core::Application::Instance()->window().size().castTo<std::size_t>();
            addComponent<DashedLine>(window_size.x / 2);

            m_leftScore = Instantiate("Left Score", this);
            m_leftScore->addComponent<Score>(spark::math::Vector2<float>(window_size.castTo<float>().x / 2 - 150, 35));

            m_rightScore = Instantiate("Right Score", this);
            m_rightScore->addComponent<Score>(spark::math::Vector2<float>(window_size.castTo<float>().x / 2 + 75, 35));
        }

    private:
        GameObject *m_leftScore = nullptr, *m_rightScore = nullptr;
    };
}

IMPLEMENT_SPARK_RTTI(pong::ui::DashedLine)

IMPLEMENT_SPARK_RTTI(pong::ui::Score)

IMPLEMENT_SPARK_RTTI(pong::ui::Background)
