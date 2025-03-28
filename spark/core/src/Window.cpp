#include "spark/core/Window.h"
#include "spark/core/Application.h"

#include "spark/base/Exception.h"
#include "spark/base/KeyCodes.h"
#include "spark/events/KeyEvents.h"
#include "spark/events/MouseEvents.h"
#include "spark/events/WindowEvents.h"
#include "spark/imgui/ImGui.h"
#include "spark/log/Logger.h"
#include "spark/math/Vector2.h"
#include "spark/render/vk/VulkanBackend.h"

#include "imgui.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace
{
    /**
     * \brief Callback used by GLFW to log errors
     * \param error Error code
     * \param description Description of the error
     */
    void error_callback(int error, const char* description)
    {
        spark::log::error("GLFW Error ({0}): {1}", error, description);
    }

    /**
     * \brief Converts a GLFW key code (int) to a Spark \ref spark::base::KeyCode
     * \param glfw_keycode GLFW key code (int)
     * \return A Spark \ref spark::base::KeyCode
     *
     * \note This function is not complete, it only converts the key codes used in the engine.
     *
     * Ranges of unhandled key codes:
     * - `GLFW_KEY_WORLD_1` to `GLFW_KEY_WORLD_2`
     * - `GLFW_KEY_KP_DECIMAL` to `GLFW_KEY_KP_EQUAL`
     * - `GLFW_KEY_F16` to `GLFW_KEY_F25`
     * - `GLFW_KEY_CAPS_LOCK` to `GLFW_KEY_PRINT_SCREEN`
     */
    spark::base::KeyCode glfw_keycode_to_spark_keycode(const int glfw_keycode)
    {
        switch (glfw_keycode)
        {
        case GLFW_KEY_SPACE:
            return spark::base::KeyCodes::Space;
        case GLFW_KEY_APOSTROPHE:
            return spark::base::KeyCodes::Quote;
        case GLFW_KEY_COMMA:
            return spark::base::KeyCodes::Comma;
        case GLFW_KEY_MINUS:
            return spark::base::KeyCodes::Subtract;
        case GLFW_KEY_PERIOD:
            return spark::base::KeyCodes::Period;
        case GLFW_KEY_SLASH:
            return spark::base::KeyCodes::Slash;
        case GLFW_KEY_0:
            return spark::base::KeyCodes::Num0;
        case GLFW_KEY_1:
            return spark::base::KeyCodes::Num1;
        case GLFW_KEY_2:
            return spark::base::KeyCodes::Num2;
        case GLFW_KEY_3:
            return spark::base::KeyCodes::Num3;
        case GLFW_KEY_4:
            return spark::base::KeyCodes::Num4;
        case GLFW_KEY_5:
            return spark::base::KeyCodes::Num5;
        case GLFW_KEY_6:
            return spark::base::KeyCodes::Num6;
        case GLFW_KEY_7:
            return spark::base::KeyCodes::Num7;
        case GLFW_KEY_8:
            return spark::base::KeyCodes::Num8;
        case GLFW_KEY_9:
            return spark::base::KeyCodes::Num9;
        case GLFW_KEY_SEMICOLON:
            return spark::base::KeyCodes::Semicolon;
        case GLFW_KEY_EQUAL:
            return spark::base::KeyCodes::Equal;
        case GLFW_KEY_A:
            return spark::base::KeyCodes::A;
        case GLFW_KEY_B:
            return spark::base::KeyCodes::B;
        case GLFW_KEY_C:
            return spark::base::KeyCodes::C;
        case GLFW_KEY_D:
            return spark::base::KeyCodes::D;
        case GLFW_KEY_E:
            return spark::base::KeyCodes::E;
        case GLFW_KEY_F:
            return spark::base::KeyCodes::F;
        case GLFW_KEY_G:
            return spark::base::KeyCodes::G;
        case GLFW_KEY_H:
            return spark::base::KeyCodes::H;
        case GLFW_KEY_I:
            return spark::base::KeyCodes::I;
        case GLFW_KEY_J:
            return spark::base::KeyCodes::J;
        case GLFW_KEY_K:
            return spark::base::KeyCodes::K;
        case GLFW_KEY_L:
            return spark::base::KeyCodes::L;
        case GLFW_KEY_M:
            return spark::base::KeyCodes::M;
        case GLFW_KEY_N:
            return spark::base::KeyCodes::N;
        case GLFW_KEY_O:
            return spark::base::KeyCodes::O;
        case GLFW_KEY_P:
            return spark::base::KeyCodes::P;
        case GLFW_KEY_Q:
            return spark::base::KeyCodes::Q;
        case GLFW_KEY_R:
            return spark::base::KeyCodes::R;
        case GLFW_KEY_S:
            return spark::base::KeyCodes::S;
        case GLFW_KEY_T:
            return spark::base::KeyCodes::T;
        case GLFW_KEY_U:
            return spark::base::KeyCodes::U;
        case GLFW_KEY_V:
            return spark::base::KeyCodes::V;
        case GLFW_KEY_W:
            return spark::base::KeyCodes::W;
        case GLFW_KEY_X:
            return spark::base::KeyCodes::X;
        case GLFW_KEY_Y:
            return spark::base::KeyCodes::Y;
        case GLFW_KEY_Z:
            return spark::base::KeyCodes::Z;
        case GLFW_KEY_LEFT_BRACKET:
            return spark::base::KeyCodes::LBracket;
        case GLFW_KEY_BACKSLASH:
            return spark::base::KeyCodes::Backslash;
        case GLFW_KEY_RIGHT_BRACKET:
            return spark::base::KeyCodes::RBracket;
        case GLFW_KEY_GRAVE_ACCENT:
            return spark::base::KeyCodes::Tilde;
        case GLFW_KEY_ESCAPE:
            return spark::base::KeyCodes::Escape;
        case GLFW_KEY_ENTER:
            return spark::base::KeyCodes::Enter;
        case GLFW_KEY_TAB:
            return spark::base::KeyCodes::Tab;
        case GLFW_KEY_BACKSPACE:
            return spark::base::KeyCodes::Backspace;
        case GLFW_KEY_INSERT:
            return spark::base::KeyCodes::Insert;
        case GLFW_KEY_DELETE:
            return spark::base::KeyCodes::Delete;
        case GLFW_KEY_RIGHT:
            return spark::base::KeyCodes::Right;
        case GLFW_KEY_LEFT:
            return spark::base::KeyCodes::Left;
        case GLFW_KEY_DOWN:
            return spark::base::KeyCodes::Down;
        case GLFW_KEY_UP:
            return spark::base::KeyCodes::Up;
        case GLFW_KEY_PAGE_UP:
            return spark::base::KeyCodes::PageUp;
        case GLFW_KEY_PAGE_DOWN:
            return spark::base::KeyCodes::PageDown;
        case GLFW_KEY_HOME:
            return spark::base::KeyCodes::Home;
        case GLFW_KEY_END:
            return spark::base::KeyCodes::End;
        case GLFW_KEY_PAUSE:
            return spark::base::KeyCodes::Pause;
        case GLFW_KEY_F1:
            return spark::base::KeyCodes::F1;
        case GLFW_KEY_F2:
            return spark::base::KeyCodes::F2;
        case GLFW_KEY_F3:
            return spark::base::KeyCodes::F3;
        case GLFW_KEY_F4:
            return spark::base::KeyCodes::F4;
        case GLFW_KEY_F5:
            return spark::base::KeyCodes::F5;
        case GLFW_KEY_F6:
            return spark::base::KeyCodes::F6;
        case GLFW_KEY_F7:
            return spark::base::KeyCodes::F7;
        case GLFW_KEY_F8:
            return spark::base::KeyCodes::F8;
        case GLFW_KEY_F9:
            return spark::base::KeyCodes::F9;
        case GLFW_KEY_F10:
            return spark::base::KeyCodes::F10;
        case GLFW_KEY_F11:
            return spark::base::KeyCodes::F11;
        case GLFW_KEY_F12:
            return spark::base::KeyCodes::F12;
        case GLFW_KEY_F13:
            return spark::base::KeyCodes::F13;
        case GLFW_KEY_F14:
            return spark::base::KeyCodes::F14;
        case GLFW_KEY_F15:
            return spark::base::KeyCodes::F15;
        case GLFW_KEY_KP_0:
            return spark::base::KeyCodes::Numpad0;
        case GLFW_KEY_KP_1:
            return spark::base::KeyCodes::Numpad1;
        case GLFW_KEY_KP_2:
            return spark::base::KeyCodes::Numpad2;
        case GLFW_KEY_KP_3:
            return spark::base::KeyCodes::Numpad3;
        case GLFW_KEY_KP_4:
            return spark::base::KeyCodes::Numpad4;
        case GLFW_KEY_KP_5:
            return spark::base::KeyCodes::Numpad5;
        case GLFW_KEY_KP_6:
            return spark::base::KeyCodes::Numpad6;
        case GLFW_KEY_KP_7:
            return spark::base::KeyCodes::Numpad7;
        case GLFW_KEY_KP_8:
            return spark::base::KeyCodes::Numpad8;
        case GLFW_KEY_KP_9:
            return spark::base::KeyCodes::Numpad9;
        case GLFW_KEY_LEFT_SHIFT:
            return spark::base::KeyCodes::LShift;
        case GLFW_KEY_LEFT_CONTROL:
            return spark::base::KeyCodes::LControl;
        case GLFW_KEY_LEFT_ALT:
            return spark::base::KeyCodes::LAlt;
        case GLFW_KEY_LEFT_SUPER:
            return spark::base::KeyCodes::LSystem;
        case GLFW_KEY_RIGHT_SHIFT:
            return spark::base::KeyCodes::RShift;
        case GLFW_KEY_RIGHT_CONTROL:
            return spark::base::KeyCodes::RControl;
        case GLFW_KEY_RIGHT_ALT:
            return spark::base::KeyCodes::RAlt;
        case GLFW_KEY_RIGHT_SUPER:
            return spark::base::KeyCodes::RSystem;
        case GLFW_KEY_MENU:
            return spark::base::KeyCodes::Menu;
        default:
            spark::log::warning("Unknown GLFW key code: {0}", glfw_keycode);
            return spark::base::KeyCodes::Unknown;
        }
    }

    /**
     * \brief Converts a GLFW mouse code (int) to a Spark \ref spark::base::MouseCode
     * \param glfw_code GLFW mouse code (int)
     * \return A Spark \ref spark::base::MouseCode
     *
     * \note This function is not complete, it only converts the mouse codes used in the engine.
     *
     * Ranges of unhandled mouse codes:
     * - `GLFW_MOUSE_BUTTON_4` to `GLFW_MOUSE_BUTTON_8`
     */
    spark::base::MouseCode glfw_mousecode_to_spark_mousecode(const int glfw_code)
    {
        switch (glfw_code)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            return spark::base::MouseCodes::Left;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return spark::base::MouseCodes::Right;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return spark::base::MouseCodes::Middle;
        default:
            spark::log::warning("Unknown GLFW mouse code: {0}", glfw_code);
            return spark::base::MouseCodes::Unknown;
        }
    }
}

/**
 * \brief Reinterpret cast a void* to a GLFWwindow*
 * \param x The void* to cast
 */
#define PRIVATE_TO_WINDOW(x) reinterpret_cast<GLFWwindow*>(x)

namespace spark::core
{
    Window::Window(const Window::Settings& settings)
        : m_settings(settings)
    {
        // Initialize GLFW
        if (const auto res = glfwInit(); res == GLFW_FALSE)
        {
            glfwTerminate();
            throw spark::base::BadWindowInitializationException("Failed to initialize GLFW!");
        }

        glfwSetErrorCallback(error_callback);
        spark::log::info("Initialized GLFW {0}", glfwGetVersionString());

        // Create window
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_window = glfwCreateWindow(static_cast<int>(settings.size.x), static_cast<int>(settings.size.y), settings.title.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw spark::base::BadWindowInitializationException("Failed to create GLFW window!");
        }

        glfwSetWindowUserPointer(PRIVATE_TO_WINDOW(m_window), this);
        spark::log::info("Created GLFW window '{0}' ({1}x{2})", settings.title, settings.size.x, m_settings.size.y);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(PRIVATE_TO_WINDOW(m_window),
                                  [](GLFWwindow* window, const int width, const int height)
                                  {
                                      Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));
                                      data.m_settings.size = {static_cast<unsigned int>(width), static_cast<unsigned int>(height)};

                                      events::WindowResizeEvent event(width, height);
                                      data.m_settings.eventCallback(event);
                                      if (!data.isMinimized())
                                          data.m_renderer->recreateSwapChain(data.m_settings.size);
                                  });

        glfwSetWindowCloseCallback(PRIVATE_TO_WINDOW(m_window),
                                   [](GLFWwindow* window)
                                   {
                                       const Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));

                                       events::WindowCloseEvent event;
                                       data.m_settings.eventCallback(event);
                                   });

        glfwSetKeyCallback(PRIVATE_TO_WINDOW(m_window),
                           [](GLFWwindow* window, const int key, int /*scancode*/, int action, int /*mods*/)
                           {
                               const Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));

                               switch (action)
                               {
                               case GLFW_PRESS:
                                   {
                                       events::KeyPressedEvent event(glfw_keycode_to_spark_keycode(key), false);
                                       data.m_settings.eventCallback(event);
                                       break;
                                   }
                               case GLFW_RELEASE:
                                   {
                                       events::KeyReleasedEvent event(glfw_keycode_to_spark_keycode(key));
                                       data.m_settings.eventCallback(event);
                                       break;
                                   }
                               case GLFW_REPEAT:
                                   {
                                       events::KeyPressedEvent event(glfw_keycode_to_spark_keycode(key), true);
                                       data.m_settings.eventCallback(event);
                                       break;
                                   }
                               default:
                                   spark::log::warning("Unknown key action: {0}", action);
                                   break;
                               }
                           });

        glfwSetMouseButtonCallback(PRIVATE_TO_WINDOW(m_window),
                                   [](GLFWwindow* window, const int button, int action, int /*mods*/)
                                   {
                                       const Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));

                                       switch (action)
                                       {
                                       case GLFW_PRESS:
                                           {
                                               events::MouseButtonPressedEvent event(glfw_mousecode_to_spark_mousecode(button));
                                               data.m_settings.eventCallback(event);
                                               break;
                                           }
                                       case GLFW_RELEASE:
                                           {
                                               events::MouseButtonReleasedEvent event(glfw_mousecode_to_spark_mousecode(button));
                                               data.m_settings.eventCallback(event);
                                               break;
                                           }
                                       default:
                                           spark::log::warning("Unknown mouse button action: {0}", action);
                                           break;
                                       }
                                   });

        glfwSetScrollCallback(PRIVATE_TO_WINDOW(m_window),
                              [](GLFWwindow* window, const double /*x_offset*/, const double y_offset)
                              {
                                  const Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));

                                  events::MouseScrolledEvent event(static_cast<float>(y_offset));
                                  data.m_settings.eventCallback(event);
                              });

        glfwSetCursorPosCallback(PRIVATE_TO_WINDOW(m_window),
                                 [](GLFWwindow* window, const double x, const double y)
                                 {
                                     const Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));

                                     events::MouseMovedEvent event(static_cast<int>(x), static_cast<int>(y));
                                     data.m_settings.eventCallback(event);
                                 });
        spark::log::info("GLFW window callbacks set for '{0}'", settings.title);

        // Setup renderer with required instance extensions
        unsigned extensions = 0;
        const char** extension_names = glfwGetRequiredInstanceExtensions(&extensions);
        if (!extension_names)
        {
            glfwDestroyWindow(PRIVATE_TO_WINDOW(m_window));
            glfwTerminate();
            throw base::NullPointerException("Failed to get required instance extensions for Vulkan");
        }

        std::vector<std::string> required_extensions(extensions);
        std::ranges::generate(required_extensions, [&extension_names, i = 0]() mutable { return extension_names[i++]; });

        using renderer_type = decltype(m_renderer)::element_type;
        using backend_type = renderer_type::backend_type;
        m_renderer = std::make_unique<renderer_type>(m_settings.size,
                                                     [&](const backend_type::handle_type& instance)
                                                     {
                                                         backend_type::surface_type::handle_type vk_surface = nullptr;
                                                         glfwCreateWindowSurface(instance, PRIVATE_TO_WINDOW(m_window), nullptr, &vk_surface);
                                                         return vk_surface;
                                                     },
                                                     required_extensions);

        // Init ImGui
        imgui::init(PRIVATE_TO_WINDOW(m_window), *m_renderer->m_renderBackend, *m_renderer->m_device, m_renderer->m_device->state().renderPass("Opaque"));
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(imgui::context()));
    }

    Window::~Window()
    {
        // Shutdown ImGui
        imgui::shutdown(*m_renderer->m_device);

        // Renderer2D must be destroyed before the window
        m_renderer.reset();

        glfwDestroyWindow(PRIVATE_TO_WINDOW(m_window));
        glfwTerminate();
        spark::log::info("Terminated GLFW, window '{0}' destroyed", m_settings.title);
    }

    void Window::close()
    {
        spark::log::info("Closing GLFW window '{0}'", m_settings.title);
        glfwSetWindowShouldClose(PRIVATE_TO_WINDOW(m_window), GLFW_TRUE);
    }

    void Window::onUpdate()
    {
        glfwPollEvents();
    }

    bool Window::isMinimized() const
    {
        return m_settings.size.x <= 1 && m_settings.size.y <= 1;
    }

    math::Vector2<unsigned int> Window::size() const
    {
        return m_settings.size;
    }

    Renderer2D<render::vk::VulkanBackend>& Window::renderer() const
    {
        return *m_renderer;
    }

    void* Window::nativeWindow() const
    {
        return m_window;
    }
}
