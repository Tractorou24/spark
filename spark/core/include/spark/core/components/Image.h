#pragma once

#include "spark/core/Component.h"

#include <filesystem>

namespace spark::core::components
{
    /**
     * \brief A simple component to render an image
     */
    class Image final : public Component
    {
        DECLARE_SPARK_RTTI(Image, Component)
        SPARK_ALLOW_PRIVATE_SERIALIZATION

    public:
        explicit Image(GameObject* parent)
            : Component(parent) {}

        explicit Image(GameObject* parent, std::filesystem::path path)
            : Component(parent), m_path(std::move(path)) {}

        void render() const override
        {
            //core::Renderer2D::DrawImage(m_path, gameObject()->transform()->position, core::Application::Instance()->window().size().castTo<float>());
        }

    private:
        std::filesystem::path m_path;
        std::optional<math::Vector2<float>> m_size;
    };
}

IMPLEMENT_SPARK_RTTI(spark::core::components::Image)
