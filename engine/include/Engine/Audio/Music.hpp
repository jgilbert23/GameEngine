#pragma once

#include <filesystem>

struct ma_sound;

namespace Engine
{
    class Music
    {
    public:
        explicit Music(std::filesystem::path path);
        ~Music();

        Music(const Music&) = delete;
        Music& operator=(const Music&) = delete;

        void play(bool loop = true);
        void stop();
        void pause();

        void setVolume(float volume);
        bool isPlaying() const;

    private:
        std::filesystem::path m_path;
        ma_sound* m_sound = nullptr;
        bool m_loaded = false;

        void loadIfNeeded();
    };
}