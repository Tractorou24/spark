#include "spark/core/Renderer2D.h"
#include "spark/core/Application.h"
#include "spark/core/platforms/SfmlWindow.h"

#include "spark/base/Platforms.h"

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

namespace spark::core
{
    sf::Font Renderer2D::s_font = Renderer2D::loadDefaultFont();
    std::unordered_map<std::string, sf::Text> Renderer2D::s_texts;

    void Renderer2D::DrawQuad(const math::Vector2<float> position, const math::Vector2<float> size, const math::Vector3<unsigned char> color)
    {
        auto* raw_window = core::Application::Instance()->m_window.get();
        const auto* window = dynamic_cast<core::SfmlWindow*>(raw_window);

        auto object = sf::RectangleShape(sf::Vector2f(size.x, size.y));
        object.setPosition(position.x, position.y);
        object.setFillColor(sf::Color(color.x, color.y, color.z));
        window->m_window->draw(object);
    }

    void Renderer2D::DrawRotatedQuad(math::Vector2<float> position, math::Vector2<float> size, math::Vector3<unsigned char> color, float rotation)
    {
        auto* raw_window = core::Application::Instance()->m_window.get();
        const auto* window = dynamic_cast<core::SfmlWindow*>(raw_window);

        auto object = sf::RectangleShape(sf::Vector2f(size.x, size.y));
        object.setPosition(position.x, position.y);
        object.setFillColor(sf::Color(color.x, color.y, color.z));
        object.rotate(rotation);
        window->m_window->draw(object);
    }

    void Renderer2D::DrawCircle(const math::Vector2<float> position, const math::Vector3<unsigned char> color, const float radius)
    {
        auto* raw_window = core::Application::Instance()->m_window.get();
        const auto* window = dynamic_cast<core::SfmlWindow*>(raw_window);

        auto object = sf::CircleShape(radius);
        object.setPosition(position.x, position.y);
        object.setFillColor(sf::Color(color.x, color.y, color.z));
        window->m_window->draw(object);
    }

    void Renderer2D::DrawText(const std::string& text, const math::Vector2<float> position, const std::size_t character_size, const std::filesystem::path& font_path)
    {
        auto* raw_window = core::Application::Instance()->m_window.get();
        const auto* window = dynamic_cast<core::SfmlWindow*>(raw_window);

        // TODO: This is a temporary solution, we should not be creating a new text & font object every frame, but storing them with a specific lifetime
        sf::Font font = s_font;
        if (!font_path.empty())
            font.loadFromFile(font_path.string());

        sf::Text object(text, font);
        object.setPosition(position.x, position.y);
        object.setCharacterSize(static_cast<unsigned>(character_size));
        window->m_window->draw(object);
    }

    void Renderer2D::DrawImage(const std::filesystem::path& path, math::Vector2<float> position, const std::optional<math::Vector2<float>>& size)
    {
        auto* raw_window = core::Application::Instance()->m_window.get();
        const auto* window = dynamic_cast<core::SfmlWindow*>(raw_window);

        // TODO: This is temporary, use real resource management when switching the renderer, we should not reload the image every frame !
        sf::Texture texture;
        if(!texture.loadFromFile(path.generic_string()))
            SPARK_CORE_ERROR("Failed to load texture from file: {}", path.generic_string());

        sf::Sprite sprite(texture);
        sprite.setPosition({position.x, position.y});
        if (size.has_value())
        {
            sprite.setScale(size->x / static_cast<float>(texture.getSize().x),
                            size->y / static_cast<float>(texture.getSize().y));
        }

        window->m_window->draw(sprite);
    }

    sf::Font Renderer2D::loadFont(const std::string_view path)
    {
        sf::Font font;
        font.loadFromFile(path.data());
        return font;
    }

    sf::Font Renderer2D::loadDefaultFont()
    {
#ifdef SPARK_OS_WINDOWS
        return loadFont(R"(C:\Windows\Fonts\arial.ttf)");
#else
        return loadFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
#endif
    }
}
