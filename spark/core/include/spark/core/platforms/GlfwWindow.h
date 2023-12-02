#pragma once

#include "spark/core/Export.h"
#include "spark/core/Renderer2D.h"
#include "spark/core/Window.h"

namespace spark::core
{
    /**
     * \brief A class representing a window created and managed by GLFW.
     */
    class SPARK_CORE_EXPORT GlfwWindow final : public Window
    {
        friend class Renderer2D;

    public:
        explicit GlfwWindow(const Window::Settings& settings);
        ~GlfwWindow() override;

        void close() override;

        void onUpdate() override;
        void onRender() override;

        [[nodiscard]] math::Vector2<unsigned int> size() const override;

        [[nodiscard]] void* nativeWindow() const override;

    private:
        void* m_window;
        Settings m_settings;
    };
}
