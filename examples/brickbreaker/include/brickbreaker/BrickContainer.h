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
            : spark::core::GameObject(name, parent)
        {
            try
            {
                m_winSound = spark::audio::Sound(spark::path::assets_path() / "win.ogg");
            }
            catch (const spark::base::CouldNotOpenFileException& ex)
            {
                spark::log::error(ex.what());
            }
        }

        void onUpdate(float /*dt*/) override
        {
            if (children().empty())
            {
                if (!m_playedWinSound)
                {
                    m_playedWinSound = true;
                    m_winSound.play();
                    return;
                }
                if (!m_winSound.isPlaying())
                    spark::core::Application::Instance()->close();
            }
        }

    private:
        spark::audio::Sound m_winSound;
        bool m_playedWinSound = false;
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
