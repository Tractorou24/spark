#pragma once

#include "spark/core/Export.h"
#include "spark/core/Renderer2D.h"
#include "spark/core/Window.h"

#include "SFML/Graphics/RenderWindow.hpp"

namespace spark::core
{
    /**
     * \brief A class representing a window created and managed by SFML.
     */
    class SPARK_CORE_EXPORT SfmlWindow final : public Window
    {
        friend class Renderer2D;

    public:
        explicit SfmlWindow(const WindowSpecification& settings);
        ~SfmlWindow() override = default;

        void close() override;

        void onUpdate() override;
        void onRender() override;

        [[nodiscard]] uint32_t getWidth() const override;
        [[nodiscard]] uint32_t getHeight() const override;
        [[nodiscard]] math::Vector2<uint32_t> getSize() const override;

        [[nodiscard]] void* getNativeWindow() const override;
        void setEventCallback(const std::function<void(events::Event&)>& callback) override;

    private:
        std::unique_ptr<sf::RenderWindow> m_window;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;

            std::function<void(events::Event&)> eventCallback;
        } m_data;
    };
}
