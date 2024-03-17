#pragma once

#include "spark/core/Application.h"
#include "spark/core/Component.h"
#include "spark/core/GameObject.h"
#include "spark/core/Scene.h"
#include "spark/core/components/Circle.h"
#include "spark/core/components/Collider.h"
#include "spark/core/components/Image.h"
#include "spark/core/components/Transform.h"

#include "spark/math/Vector2.h"

template <typename SerializerType>
struct experimental::ser::SerializerScheme<SerializerType, std::filesystem::path>
{
    static void serialize(SerializerType& serializer, const std::filesystem::path& obj)
    {
        serializer << obj.generic_string();
    }

    static void deserialize(SerializerType& deserializer, std::filesystem::path& obj)
    {
        std::string str;
        deserializer >> str;
        obj = str;
    }
};

template <typename SerializerType, typename T>
struct experimental::ser::SerializerScheme<SerializerType, std::optional<T>>
{
    static void serialize(SerializerType& serializer, const std::optional<T>& obj)
    {
        serializer << obj.has_value();
        if (obj.has_value())
            serializer << obj.value();
    }

    static void deserialize(SerializerType& deserializer, std::optional<T>& obj)
    {
        bool has_value = false;
        deserializer >> has_value;
        if (has_value)
        {
            T value;
            deserializer >> value;
            obj = value;
        }
        else
            obj = std::nullopt;
    }
};

template <typename SerializerType, typename T>
struct experimental::ser::SerializerScheme<SerializerType, spark::math::Vector2<T>>
{
    static void serialize(SerializerType& serializer, const spark::math::Vector2<T>& obj)
    {
        serializer << obj.x;
        serializer << obj.y;
    }

    static void deserialize(SerializerType& deserializer, spark::math::Vector2<T>& obj)
    {
        deserializer >> obj.x;
        deserializer >> obj.y;
    }
};

template <typename SerializerType, typename T>
struct experimental::ser::SerializerScheme<SerializerType, spark::math::Rectangle<T>>
{
    static void serialize(SerializerType& serializer, const spark::math::Rectangle<T>& obj)
    {
        serializer << obj.position;
        serializer << obj.extent;
    }

    static void deserialize(SerializerType& deserializer, spark::math::Rectangle<T>& obj)
    {
        deserializer >> obj.position;
        deserializer >> obj.extent;
    }
};

SPARK_DEFINE_EMPTY_SERIALIZER_SCHEME(spark::core::Component)

SPARK_SERIALIZE_RTTI_CLASS(spark::core::components::Circle, radius)
SPARK_SERIALIZE_RTTI_CLASS(spark::core::components::Collider, m_rectangle)
SPARK_SERIALIZE_RTTI_CLASS(spark::core::components::Image, m_path, m_size)

template <typename SerializerType>
struct experimental::ser::SerializerScheme<SerializerType, spark::core::GameObject>
{
    static void serialize(SerializerType& serializer, const spark::core::GameObject& obj)
    {
        serializer << obj.isShown;

        serializer << obj.components().size();
        for (const auto* component : obj.components())
        {
            serializer << component->rttiInstance().className();
            spark::core::Application::Instance()->registries().component.serializer(component->rttiInstance().className())(serializer, *component);
        }

        serializer << obj.children().size();
        for (const auto* child : obj.children())
        {
            serializer << child->rttiInstance().className();
            serializer << child->name();
            spark::core::Application::Instance()->registries().gameObject.serializer(child->rttiInstance().className())(serializer, *child);
        }
    }

    static void deserialize(SerializerType& deserializer, spark::core::GameObject& obj)
    {
        deserializer >> obj.isShown;

        std::size_t components_count;
        deserializer >> components_count;
        for (std::size_t i = 0; i < components_count; ++i)
        {
            std::string type;
            deserializer >> type;

            // If the class already haves the component, don't recreate it. Only deserialize in place.
            spark::core::Component* component = nullptr;
            if (auto it = obj.m_components.find(spark::rtti::RttiDatabase::Get(type)); it != obj.m_components.end())
                component = it->second.first;
            else
            {
                component = spark::core::Application::Instance()->registries().component.create(type, &obj).release();
                obj.addComponent(component, true);
            }

            spark::core::Application::Instance()->registries().component.deserializer(type)(deserializer, *component);
        }

        std::size_t children_count;
        deserializer >> children_count;

        for (std::size_t i = 0; i < children_count; ++i)
        {
            std::string type, name;
            deserializer >> type;
            deserializer >> name;

            spark::core::GameObject* game_object = nullptr;

            auto children = obj.children();
            if (auto it = std::ranges::find_if(children,
                                               [&](const spark::core::GameObject* go)
                                               {
                                                   return &go->rttiInstance() == spark::rtti::RttiDatabase::Get(type) && go->name() == name;
                                               }); it != children.end())
                game_object = *it;
            else
                game_object = spark::core::Application::Instance()->registries().gameObject.create(type, std::move(name), &obj).release();

            spark::core::Application::Instance()->registries().gameObject.deserializer(type)(deserializer, *game_object);
        }
    }
};

template <typename SerializerType>
struct experimental::ser::SerializerScheme<SerializerType, spark::core::Scene>
{
    static void serialize(SerializerType& serializer, const spark::core::Scene& obj)
    {
        serializer << *obj.m_root;
        serializer << obj.m_isLoaded;
    }

    static void deserialize(SerializerType& deserializer, spark::core::Scene& obj)
    {
        SPARK_ASSERT(obj.m_root != nullptr && "Root GameObject must be set before deserialization")

        deserializer >> *obj.m_root;
        deserializer >> obj.m_isLoaded;
    }
};
