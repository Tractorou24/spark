#pragma once

#include "pong/ui/Button.h"

#include "spark/core/SceneManager.h"
#include "spark/engine/GameObject.h"
#include "spark/engine/components/Image.h"
#include "spark/path/Paths.h"

namespace pong::ui
{
    /**
     * \brief The pong main menu shown at the start of the game.
     */
    class Menu final : public spark::engine::GameObject
    {
        DECLARE_SPARK_RTTI(Menu, spark::engine::GameObject)

    public:
        explicit Menu(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent)
        {
            const auto& window_size = spark::core::Application::Instance()->window().size().castTo<float>();

            addComponent<spark::engine::components::Image>(spark::path::assets_path() / "menu_background.jpg");

            auto* text = Instantiate("Title", this);
            text->addComponent<spark::engine::components::Text>("THE PONG GAME", spark::math::Vector2<float>(0, 0), spark::path::assets_path() / "font.ttf");
            text->transform()->position = {window_size.x / 2 - 250, 50};

            m_playButton = Instantiate<Button>("Play", this, "Play", spark::math::Vector2<float>(150, 75));
            m_playButton->transform()->position = {window_size.x / 3 - 100, window_size.y / 2 - 50};

            m_quitButton = Instantiate<Button>("Quit", this, "Quit", spark::math::Vector2<float>(150, 75));
            m_quitButton->transform()->position = {window_size.x / 1.5f, window_size.y / 2 - 50};
        }

        void onSpawn() override
        {
            m_playButtonSlotId = m_playButton->onClicked.connect([]
            {
                spark::core::SceneManager::LoadScene("Game");
            });
            m_quitButtonSlotId = m_quitButton->onClicked.connect([]
            {
                spark::core::Application::Instance()->close();
            });
        }

        void onDestroyed() override
        {
            m_playButton->onClicked.disconnect(m_playButtonSlotId);
            m_quitButton->onClicked.disconnect(m_quitButtonSlotId);
        }

    private:
        Button *m_playButton, *m_quitButton;
        std::size_t m_playButtonSlotId = 0, m_quitButtonSlotId = 0;
    };
}

IMPLEMENT_SPARK_RTTI(pong::ui::Menu)
