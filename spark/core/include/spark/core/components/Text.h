#pragma once

#include "spark/core/Component.h"

#include "spark/math/Vector2.h"
#include "spark/rtti/HasRtti.h"

namespace spark::core::components
{
    class Text final : public Component
    {
        DECLARE_SPARK_RTTI(Text, Component)
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
        explicit Text(GameObject* parent)
            : Component(parent) {}

        explicit Text(GameObject* parent, std::string content, const math::Vector2<float> offset, std::filesystem::path font_path = "")
            : Component(parent), m_content(std::move(content)), m_offset(offset), m_fontPath(std::move(font_path)) {}

        void render() const override
        {
            //core::Renderer2D::DrawText(m_content, Transform::LocalToWorld(gameObject()->transform()) + m_offset, 72, m_fontPath);
        }

    private:
        std::string m_content;
        math::Vector2<float> m_offset;
        std::filesystem::path m_fontPath;
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Text)
