#pragma once

#include "spark/core/Export.h"

#include "spark/events/Event.h"
#include "spark/log/Logger.h"
#include "spark/math/Vector2.h"

#include <functional>

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
            std::function<void(events::Event&)> eventCallback;
        };

    public:
        /**
         * \brief Creates a window with the given \p settings.
         * \param settings The \ref Settings used to create and manage the window.
         */
        explicit Window(const Settings& settings);
        ~Window();

        Window(const Window& other) = delete;
        Window(Window&& other) noexcept = default;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = default;

        /**
         * \brief Closes the window.
         */
        void close();

        /**
         * \brief Method called every frame to update the window.
         */
        void onUpdate();

        /**
         * \brief Method called every frame to render the window.
         */
        void onRender();

        /**
         * \brief Gets the size of the window.
         * \return A 2D vector representing the width and height of the window.
         */
        [[nodiscard]] math::Vector2<unsigned int> size() const;

        /**
         * \brief Gets a handle to the native platform window.
         * \return A void* pointer to the native window which can be casted to the platform's window type.
         */
        [[nodiscard]] void* nativeWindow() const;

    private:
        Settings m_settings;
        void* m_window;
    };
}
