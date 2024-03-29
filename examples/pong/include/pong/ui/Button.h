#pragma once

#include "spark/core/Input.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Text.h"
#include "spark/core/components/Transform.h"
#include "spark/path/Paths.h"
#include "spark/patterns/Signal.h"
#include "spark/rtti/HasRtti.h"

namespace pong::ui
{
    /**
     * \brief A button that can be clicked with text on it.
     */
    class Button final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Button, GameObject)

    public:
        explicit Button(std::string name, GameObject* parent, const std::string& text, const spark::math::Vector2<float> size)
            : GameObject(std::move(name), parent), m_size(size)
        {
            spark::core::Input::mousePressedEvents[spark::base::MouseCodes::Left].connect([this]
            {
                const auto mouse_pos = spark::core::Input::MousePosition();
                const auto position = transform()->position;
                if (mouse_pos.x >= position.x && mouse_pos.x <= position.x + m_size.x && mouse_pos.y >= position.y && mouse_pos.y <= position.y + m_size.y)
                    onClicked.emit();
            });
            addComponent<spark::core::components::Text>(text, spark::math::Vector2<float>(0, 0), spark::path::assets_path() / "font.ttf");
        }

        spark::patterns::Signal<> onClicked;

    private:
        spark::math::Vector2<float> m_size;
    };
}

IMPLEMENT_SPARK_RTTI(pong::ui::Button)
