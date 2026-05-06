#include "Engine/Audio/Sound.hpp"
#include "Engine/Audio/AudioEngine.hpp"

namespace Engine
{
    Sound::Sound(std::filesystem::path path)
        : m_path(std::move(path))
    {
    }

    void Sound::play(float volume)
    {
        AudioEngine::playOneShot(m_path, volume);
    }

    const std::filesystem::path& Sound::path() const
    {
        return m_path;
    }
}