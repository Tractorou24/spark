#include "spark/core/Input.h"
#include "spark/core/Application.h"

#include "boost/preprocessor/seq/for_each.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

/**
 * \brief Inserts a key code into the key event map.
 * \param r BoostPP internal macro parameter.
 * \param data The namespace of the key code.
 * \param elem The key code to be defined.
 */
#define SPARK_LIB_INSERT_INTO_EVENT_MAP(r, data, elem) map.insert({data::elem, spark::patterns::Signal<>()});

namespace spark::core
{
    namespace globals
    {
        auto generate_key_map()
        {
            std::unordered_map<base::KeyCode, patterns::Signal<>> map;
            BOOST_PP_SEQ_FOR_EACH(SPARK_LIB_INSERT_INTO_EVENT_MAP, base::KeyCodes, SPARK_BASE_KEYCODES_VALUES)
            return map;
        }

        auto generate_mouse_map()
        {
            std::unordered_map<base::MouseCode, patterns::Signal<>> map;
            BOOST_PP_SEQ_FOR_EACH(SPARK_LIB_INSERT_INTO_EVENT_MAP, base::MouseCodes, SPARK_BASE_MOUSECODES_VALUES)
            return map;
        }
    }

    std::unordered_map<base::KeyCode, patterns::Signal<>> Input::keyPressedEvents = globals::generate_key_map();
    std::unordered_map<base::KeyCode, patterns::Signal<>> Input::keyReleasedEvents = globals::generate_key_map();
    std::unordered_map<base::MouseCode, patterns::Signal<>> Input::mousePressedEvents = globals::generate_mouse_map();
    std::unordered_map<base::MouseCode, patterns::Signal<>> Input::mouseReleasedEvents = globals::generate_mouse_map();
    patterns::Signal<> Input::mouseMovedEvent;

    bool Input::IsKeyPressed(base::KeyCode key)
    {
        const auto sfml_key = static_cast<sf::Keyboard::Key>(key);
        return sf::Keyboard::isKeyPressed(sfml_key);
    }

    bool Input::IsMousePressed(base::MouseCode button)
    {
        const auto sfml_button = static_cast<sf::Mouse::Button>(button);
        return sf::Mouse::isButtonPressed(sfml_button);
    }

    math::Vector2<float> Input::GetMousePosition()
    {
        const auto position = sf::Mouse::getPosition(*static_cast<sf::Window*>(Application::Instance()->getWindow().getNativeWindow()));
        return {static_cast<float>(position.x), static_cast<float>(position.y)};
    }
}

#undef SPARK_CORE_DEFINE_KEYS_EVENT_MAP
