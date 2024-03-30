#pragma once

#include "spark/core/Component.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Collider.h"
#include "spark/rtti/HasRtti.h"

namespace brickbreaker
{
    /**
     * \brief A game object used to add collision to a screen border. (left, right, top, bottom)
     */
    class ScreenBorder final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(ScreenBorder, GameObject)

    public:
        explicit ScreenBorder(std::string name, spark::core::GameObject* parent)
            : ScreenBorder(std::move(name), parent, {0, 0}, {0, 0}) {}

        /**
         * \brief Creates a new collision border at the bottom of the screen.
         * \param name The name of the game object.
         * \param parent The parent of the game object.
         * \return The created screen border.
         */
        static ScreenBorder* Top(std::string name, spark::core::GameObject* parent)
        {
            const auto position = spark::math::Vector2(0.0f, -1.f);
            const auto size = spark::math::Vector2(static_cast<float>(spark::core::Application::Instance()->window().size().x), 1.f);
            return new ScreenBorder(std::move(name), parent, position, size);
        }

        /**
         * \brief Creates a new collision border at the bottom of the screen.
         * \param name The name of the game object.
         * \param parent The parent of the game object.
         * \return The created screen border.
         */
        static ScreenBorder* Left(std::string name, spark::core::GameObject* parent)
        {
            const auto position = spark::math::Vector2(-1.f, 0.f);
            const auto size = spark::math::Vector2(1.f, static_cast<float>(spark::core::Application::Instance()->window().size().y));
            return new ScreenBorder(std::move(name), parent, position, size);
        }

        /**
         * \brief Creates a new collision border at the bottom of the screen.
         * \param name The name of the game object.
         * \param parent The parent of the game object.
         * \return The created screen border.
         */
        static ScreenBorder* Right(std::string name, spark::core::GameObject* parent)
        {
            const auto position = spark::math::Vector2(static_cast<float>(spark::core::Application::Instance()->window().size().x), 0.f);
            const auto size = spark::math::Vector2(1.f, static_cast<float>(spark::core::Application::Instance()->window().size().y));
            return new ScreenBorder(std::move(name), parent, position, size);
        }

        /**
         * \brief Creates a new collision border at the bottom of the screen.
         * \param name The name of the game object.
         * \param parent The parent of the game object.
         * \return The created screen border.
         */
        static ScreenBorder* Bottom(std::string name, spark::core::GameObject* parent)
        {
            const auto position = spark::math::Vector2(0.0f, static_cast<float>(spark::core::Application::Instance()->window().size().y));
            const auto size = spark::math::Vector2(static_cast<float>(spark::core::Application::Instance()->window().size().x), 1.f);
            return new ScreenBorder(std::move(name), parent, position, size);
        }

    private:
        ScreenBorder(std::string name, spark::core::GameObject* parent, const spark::math::Vector2<float>& position, const spark::math::Vector2<float>& size)
            : GameObject(std::move(name), parent)
        {
            transform()->position = position;
            addComponent<spark::core::components::StaticCollider>(spark::math::Rectangle({0, 0}, size));
        }
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::ScreenBorder)

SPARK_SERIALIZE_RTTI_CLASS(brickbreaker::ScreenBorder)
