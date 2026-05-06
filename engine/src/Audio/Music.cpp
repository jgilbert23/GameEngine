#include "Engine/Audio/Music.hpp"
#include "Engine/Audio/AudioEngine.hpp"
#include "Engine/Core/Assert.hpp"

#include <miniaudio.h>

namespace Engine
{
    ma_engine* internalAudioEngine();

    Music::Music(std::filesystem::path path)
        : m_path(std::move(path))
    {
    }

    Music::~Music()
    {
        if (m_sound)
        {
            ma_sound_uninit(m_sound);
            delete m_sound;
            m_sound = nullptr;
        }
    }

    void Music::loadIfNeeded()
    {
        if (m_loaded)
            return;

        ENGINE_ASSERT(AudioEngine::initialized(), "AudioEngine is not initialized");

        m_sound = new ma_sound{};

        ma_result result = ma_sound_init_from_file(
            internalAudioEngine(),
            m_path.string().c_str(),
            MA_SOUND_FLAG_STREAM,
            nullptr,
            nullptr,
            m_sound
        );

        ENGINE_ASSERT(result == MA_SUCCESS, "Failed to load music");

        m_loaded = true;
    }

    void Music::play(bool loop)
    {
        loadIfNeeded();

        ma_sound_set_looping(m_sound, loop ? MA_TRUE : MA_FALSE);
        ma_sound_start(m_sound);
    }

    void Music::stop()
    {
        if (!m_sound)
            return;

        ma_sound_stop(m_sound);
        ma_sound_seek_to_pcm_frame(m_sound, 0);
    }

    void Music::pause()
    {
        if (!m_sound)
            return;

        ma_sound_stop(m_sound);
    }

    void Music::setVolume(float volume)
    {
        loadIfNeeded();
        ma_sound_set_volume(m_sound, volume);
    }

    bool Music::isPlaying() const
    {
        return m_sound && ma_sound_is_playing(m_sound);
    }
}