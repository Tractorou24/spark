#include "spark/audio/Sound.h"

#include "spark/base/Exception.h"

#include <format>

namespace spark::audio
{
    const std::vector<std::string_view> Sound::supportedExtensions = {".wav", ".ogg"};

    Sound::Sound(const std::filesystem::path& file)
    {
        if (std::ranges::find(supportedExtensions, file.extension()) == supportedExtensions.end())
            throw base::UnsupportedFileFormatException(std::format("Unsupported file format: {}", file.extension().generic_string()));

        if (!m_buffer.loadFromFile(file.generic_string()))
            throw base::CouldNotOpenFileException(std::format("Could not open file: {}", file.generic_string()));
        m_sound.setBuffer(m_buffer);
    }

    Sound::~Sound()
    {
        m_sound.stop();
        m_sound.resetBuffer();
    }

    Sound::Sound(const Sound& other)
    {
        m_buffer = other.m_buffer;
        m_sound.setBuffer(m_buffer);
    }

    Sound& Sound::operator=(const Sound& other)
    {
        m_buffer = other.m_buffer;
        m_sound.setBuffer(m_buffer);
        return *this;
    }

    void Sound::play()
    {
        m_sound.play();
    }

    void Sound::pause()
    {
        m_sound.pause();
    }

    void Sound::stop()
    {
        m_sound.stop();
    }

    void Sound::restart()
    {
        m_sound.stop();
        m_sound.play();
    }

    bool Sound::loop() const
    {
        return m_sound.getLoop();
    }

    float Sound::volume() const
    {
        return m_sound.getVolume();
    }

    float Sound::pitch() const
    {
        return m_sound.getPitch();
    }

    bool Sound::isPlaying() const
    {
        return m_sound.getStatus() == sf::Sound::Playing;
    }

    void Sound::setLoop(const bool loop)
    {
        m_sound.setLoop(loop);
    }

    void Sound::setVolume(const float volume)
    {
        m_sound.setVolume(volume);
    }

    void Sound::setPitch(const float pitch)
    {
        m_sound.setPitch(pitch);
    }
}
