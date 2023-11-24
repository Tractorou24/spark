#pragma once

#include "spark/audio/Export.h"

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include <filesystem>

namespace spark::audio
{
    /**
     * \brief A class representing a sound that can be played.
     */
    class SPARK_AUDIO_EXPORT Sound final
    {
    public:
        static const std::vector<std::string_view> supportedExtensions;

    public:
        /**
         * \brief Instantiates a new playable sound.
         * \param file The path to the sound file.
         */
        explicit Sound(const std::filesystem::path& file);
        ~Sound();

        Sound(const Sound& other);
        Sound(Sound&& other) noexcept = default;
        Sound& operator=(const Sound& other);
        Sound& operator=(Sound&& other) noexcept = default;

        /**
         * \brief Starts playing the sound.
         */
        void play();

        /**
         * \brief Pauses the sound.
         */
        void pause();

        /**
         * \brief Stops the sound and resets it to the beginning.
         */
        void stop();

        /**
         * \brief Restarts the sound from the beginning.
         */
        void restart();

        /**
         * \brief Gets whether the sound is currently playing.
         * \return A boolean indicating whether the sound is currently playing.
         */
        [[nodiscard]] bool loop() const;

        /**
         * \brief Gets the volume of the sound.
         * \return A float between 0 and 100 indicating the volume of the sound.
         */
        [[nodiscard]] float volume() const;

        /**
         * \brief Gets the pitch of the sound.
         * \return A float indicating the pitch of the sound.
         */
        [[nodiscard]] float pitch() const;

        /**
         * \brief Sets the sound to loop or not.
         * \param loop A boolean indicating whether the sound should loop or not.
         */
        void setLoop(bool loop);

        /**
         * \brief Sets the volume of the sound.
         * \param volume A float between 0 and 100 indicating the volume of the sound.
         */
        void setVolume(float volume);

        /**
         * \brief Sets the pitch of the sound.
         * \param pitch A float indicating the pitch of the sound.
         */
        void setPitch(float pitch);

    private:
        sf::SoundBuffer m_buffer;
        sf::Sound m_sound;
    };
}
