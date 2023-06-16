#pragma once

#include "spark/core/Export.h"

#include "spark/base/KeyCodes.h"
#include "spark/base/MouseCodes.h"
#include "spark/math/Vector2.h"
#include "spark/patterns/Signal.h"

#include <unordered_map>

namespace spark::core
{
    /**
     * \brief A static class giving access to real-time input.
     */
    class SPARK_CORE_EXPORT Input
    {
    public:
        /**
         * \brief Gets the state of a key.
         * \param key The key to check.
         * \return A boolean indicating whether the key is pressed.
         */
        static bool IsKeyPressed(base::KeyCode key);

        /**
         * \brief Gets the state of a mouse button.
         * \param button The button to check, defaults to left mouse button.
         * \return A boolean indicating whether the button is pressed.
         */
        static bool IsMousePressed(base::MouseCode button = base::MouseCodes::Left);

        /**
         * \brief Gets the position of the mouse relative to the window.
         * \return A vector containing the x and y coordinates of the mouse.
         */
        static math::Vector2<float> GetMousePosition();

        /**
         * \brief A map of signals that are triggered when a key is pressed.
         */
        static std::unordered_map<base::KeyCode, patterns::Signal<>> keyPressedEvents;

        /**
         * \brief A map of signals that are triggered when a key is released.
         */
        static std::unordered_map<base::KeyCode, patterns::Signal<>> keyReleasedEvents;

        /**
         * \brief A map of signals that are triggered when a mouse button is pressed.
         */
        static std::unordered_map<base::MouseCode, patterns::Signal<>> mousePressedEvents;

        /**
         * \brief A map of signals that are triggered when a mouse button is released.
         */
        static std::unordered_map<base::MouseCode, patterns::Signal<>> mouseReleasedEvents;

        /**
         * \brief A signal that is triggered when the mouse is moved.
         */
        static patterns::Signal<> mouseMovedEvent;
    };
}
