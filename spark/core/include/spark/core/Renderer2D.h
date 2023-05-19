#pragma once

#include <filesystem>

#include "spark/core/Export.h"

#include "spark/math/Vector2.h"
#include "spark/math/Vector3.h"

#include "SFML/Graphics/Font.hpp"

#include <optional>
#include <unordered_map>

namespace spark::core
{
    class SPARK_CORE_EXPORT Renderer2D
    {
    public:
        static void DrawQuad(math::Vector2<float> position, math::Vector2<float> size, math::Vector3<unsigned char> color);
        static void DrawRotatedQuad(math::Vector2<float> position, math::Vector2<float> size, math::Vector3<unsigned char> color, float rotation);
        static void DrawCircle(math::Vector2<float> position, math::Vector3<unsigned char> color, float radius);
        static void DrawText(const std::string& text, math::Vector2<float> position, std::size_t character_size, const std::filesystem::path& font_path = "");
        static void DrawImage(const std::filesystem::path& path, math::Vector2<float> position, const std::optional<math::Vector2<float>>& size);

    private:
        static sf::Font loadFont(std::string_view path);
        static sf::Font loadDefaultFont();

    private:
        static sf::Font s_font;
        static std::unordered_map<std::string, sf::Text> s_texts;
    };

    template <typename T>
    struct Render2D;
}
