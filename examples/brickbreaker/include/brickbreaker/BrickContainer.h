#pragma once

#include "experimental/ser/FileSerializer.h"
#include "spark/core/GameObject.h"
#include "spark/core/details/SerializationSchemes.h"

namespace brickbreaker
{
    class BrickContainer final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(BrickContainer, GameObject)

    public:
        BrickContainer(const std::string& name, spark::core::GameObject* parent)
            : spark::core::GameObject(name, parent) {}
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::BrickContainer)

template <typename SerializerType>
struct experimental::ser::SerializerScheme<SerializerType, brickbreaker::BrickContainer>
{
    static void serialize(SerializerType& serializer, const brickbreaker::BrickContainer& object)
    {
        experimental::ser::SerializerScheme<SerializerType, spark::core::GameObject>::serialize(serializer, object);
    }

    static void deserialize(SerializerType& serializer, brickbreaker::BrickContainer& object)
    {
        auto children = object.children();
        for (auto* child : children)
            spark::core::GameObject::Destroy(child, true);
        experimental::ser::SerializerScheme<SerializerType, spark::core::GameObject>::deserialize(serializer, object);
    }
};
