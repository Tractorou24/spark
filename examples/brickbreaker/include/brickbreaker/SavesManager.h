#pragma once

#include "experimental/ser/FileSerializer.h"
#include "spark/base/KeyCodes.h"
#include "spark/core/GameObject.h"
#include "spark/core/Input.h"
#include "spark/core/SceneManager.h"
#include "spark/core/details/SerializationSchemes.h"
#include "spark/lib/UuidGenerator.h"

namespace brickbreaker
{
    class SavesManager final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(SavesManager, GameObject)

    public:
        explicit SavesManager(std::string name, GameObject* parent)
            : GameObject(std::move(name), parent) {}

        void onSpawn() override
        {
            m_loadGameSlotId = spark::core::Input::keyPressedEvents[spark::base::KeyCodes::O].connect([]
            {
                // Get last game file
                const auto saves_path = spark::path::assets_path() / "saves";

                if (!std::filesystem::exists(saves_path))
                {
                    spark::log::info("Saves directory not found, creating it.");
                    std::filesystem::create_directory(saves_path);
                }

                std::set<std::filesystem::path, decltype([](const std::filesystem::path& lhs, const std::filesystem::path& rhs)
                {
                    return std::filesystem::last_write_time(lhs) > std::filesystem::last_write_time(rhs);
                })> files;
                for (const auto& entry : std::filesystem::directory_iterator(saves_path))
                    if (entry.is_regular_file())
                        files.insert(entry.path());

                if (files.empty())
                {
                    spark::log::error("No save found in {}", saves_path.generic_string());
                    return;
                }

                spark::log::info("Loading save {}", files.begin()->filename().string());

                // Load the game scene
                spark::core::SceneManager::LoadScene("Game");

                // Deserialize it into the loaded scene (so it does not call reset())
                try
                {
                    experimental::ser::FileSerializer deserializer(*files.begin(), true);
                    deserializer >> *spark::core::SceneManager::Scene("Game").get();
                }
                catch (std::exception& ex)
                {
                    spark::log::error("Failed to load game: {}", ex.what());
                }
            });

            m_saveSlotKey = spark::core::Input::keyPressedEvents[spark::base::KeyCodes::S].connect([]
            {
                // Generate the save file
                const auto saves_path = spark::path::assets_path() / "saves";
                if (!std::filesystem::exists(saves_path))
                {
                    spark::log::info("Saves directory not found, creating it.");
                    std::filesystem::create_directory(saves_path);
                }

                const auto file_path = saves_path / spark::lib::UuidGenerator().generate().str();
                spark::log::info("Saving game to {}", file_path.filename().string());

                // Write it
                try
                {
                    experimental::ser::FileSerializer serializer(file_path, false);
                    serializer << *spark::core::SceneManager::Scene("Game").get();
                } catch (std::exception& ex)
                {
                    spark::log::error("Failed to save game: {}", ex.what());
                }
            });
        }

        void onDestroyed() override
        {
            spark::core::Input::keyPressedEvents[spark::base::KeyCodes::O].disconnect(m_loadGameSlotId);
            spark::core::Input::keyPressedEvents[spark::base::KeyCodes::S].disconnect(m_saveSlotKey);
        }

    private:
        std::size_t m_loadGameSlotId = 0, m_saveSlotKey = 0;
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::SavesManager)

SPARK_SERIALIZE_RTTI_CLASS(brickbreaker::SavesManager)
