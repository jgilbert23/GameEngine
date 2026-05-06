#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "Engine/Audio/AudioEngine.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/Log.hpp"

#include <memory>
#include <vector>

namespace Engine
{
    struct ActiveSound
    {
        ma_sound sound{};
        bool initialized = false;
    };

    static ma_engine s_engine;
    static bool s_initialized = false;
    static float s_masterVolume = 1.0f;

    static std::vector<std::unique_ptr<ActiveSound>> s_activeSounds;

    ma_engine* internalAudioEngine()
    {
        return &s_engine;
    }

    void AudioEngine::init()
    {
        if (s_initialized)
            return;

        const ma_result result = ma_engine_init(nullptr, &s_engine);
        ENGINE_ASSERT(result == MA_SUCCESS, "Failed to initialize audio engine");

        ma_engine_set_volume(&s_engine, s_masterVolume);
        s_initialized = true;
    }

    void AudioEngine::shutdown()
    {
        if (!s_initialized)
            return;

        for (auto& activeSound : s_activeSounds)
        {
            if (activeSound && activeSound->initialized)
            {
                ma_sound_uninit(&activeSound->sound);
                activeSound->initialized = false;
            }
        }

        s_activeSounds.clear();

        ma_engine_uninit(&s_engine);
        s_initialized = false;
    }

    void AudioEngine::update()
    {
        if (!s_initialized)
            return;

        for (auto it = s_activeSounds.begin(); it != s_activeSounds.end();)
        {
            ActiveSound* activeSound = it->get();

            if (!activeSound || !activeSound->initialized)
            {
                it = s_activeSounds.erase(it);
                continue;
            }

            if (!ma_sound_is_playing(&activeSound->sound))
            {
                ma_sound_uninit(&activeSound->sound);
                activeSound->initialized = false;
                it = s_activeSounds.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void AudioEngine::playOneShot(const std::filesystem::path& path, float volume)
    {
        ENGINE_ASSERT(s_initialized, "AudioEngine is not initialized");

        auto activeSound = std::make_unique<ActiveSound>();

        const ma_result result = ma_sound_init_from_file(
            &s_engine,
            path.string().c_str(),
            0,
            nullptr,
            nullptr,
            &activeSound->sound
        );

        if (result != MA_SUCCESS)
        {
            ENGINE_ERROR("Failed to load one-shot sound");
            return;
        }

        activeSound->initialized = true;

        ma_sound_set_volume(&activeSound->sound, volume);
        ma_sound_start(&activeSound->sound);

        s_activeSounds.emplace_back(std::move(activeSound));
    }

    void AudioEngine::setMasterVolume(float volume)
    {
        s_masterVolume = volume;

        if (s_initialized)
            ma_engine_set_volume(&s_engine, s_masterVolume);
    }

    float AudioEngine::masterVolume()
    {
        return s_masterVolume;
    }

    bool AudioEngine::initialized()
    {
        return s_initialized;
    }
}