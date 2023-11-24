#pragma once

#include "spark/engine/Component.h"

#include <filesystem>

namespace spark::engine::components
{
    /**
     * \brief A simple component to render an image
     */
    class Image final : public Component
    {
        DECLARE_SPARK_RTTI(Image, Component)

    public:
        explicit Image(GameObject* parent, std::filesystem::path path)
            : Component(parent), m_path(std::move(path)) { }

        void render() const override
        {
            core::Renderer2D::DrawImage(m_path, gameObject()->transform()->position, core::Application::Instance()->window().size().castTo<float>());
        }

    private:
        std::filesystem::path m_path;
        std::optional<math::Vector2<float>> m_size;
    };
}

IMPLEMENT_SPARK_RTTI(spark::engine::components::Image)
