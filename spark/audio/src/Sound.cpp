#include "spark/audio/Sound.h"

#include "spark/base/Exception.h"

namespace spark::audio
{
    Sound::Sound(const std::filesystem::path& file)
    {
        if (std::ranges::find(supportedExtensions, file.extension()) != supportedExtensions.end())
            SPARK_BASE_THROW_EXCEPTION(base::UnsupportedFileFormatException, fmt::format("Unsupported file format: {}", file.extension().generic_string()).c_str());

        if (!m_buffer.loadFromFile(file.generic_string()))
            SPARK_BASE_THROW_EXCEPTION(base::CouldNotOpenFileException, fmt::format("Could not open file: {}", file.generic_string()).c_str());
        m_sound.setBuffer(m_buffer);
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

    bool Sound::getLoop() const
    {
        return m_sound.getLoop();
    }

    float Sound::getVolume() const
    {
        return m_sound.getVolume();
    }

    float Sound::getPitch() const
    {
        return m_sound.getPitch();
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
