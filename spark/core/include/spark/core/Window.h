#pragma once

#include "spark/core/Export.h"

#include "spark/events/Event.h"
#include "spark/log/Logger.h"
#include "spark/math/Vector2.h"

#include <functional>
#include <memory>

namespace spark::core
{
    /**
     * \brief An interface representing any window of any platform.
     */
    class SPARK_CORE_EXPORT Window
    {
    public:
        /**
         * \brief A struct containing the settings used to create a window.
         */
        struct Settings
        {
            std::string title;
            spark::math::Vector2<unsigned int> size;
        };

        /**
         * \brief Creates a window based on the platform.
         * \param settings The window build settings. 
         * \return A std::unique_ptr to the window.
         */
        static std::unique_ptr<Window> Create(const Settings& settings);

    public:
        Window() = default;
        virtual ~Window() = default;

        Window(const Window& other) = delete;
        Window(Window&& other) noexcept = default;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = default;

        /**
         * \brief Closes the window.
         */
        virtual void close() = 0;

        /**
         * \brief Method called every frame to update the window.
         */
        virtual void onUpdate() = 0;

        /**
         * \brief Method called every frame to render the window.
         */
        virtual void onRender() = 0;

        /**
         * \brief Gets the size of the window.
         * \return A 2D vector representing the width and height of the window.
         */
        [[nodiscard]] virtual math::Vector2<unsigned int> size() const = 0;

        /**
         * \brief Gets a handle to the native platform window.
         * \return A void* pointer to the native window which can be casted to the platform's window type.
         */
        [[nodiscard]] virtual void* nativeWindow() const = 0;

        /**
         * \brief Sets the event callback for the window.
         * \param callback A std::function taking an event as parameter.
         */
        virtual void setEventCallback(const std::function<void(events::Event&)>& callback) = 0;
    };
}
