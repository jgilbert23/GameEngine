#pragma once

#include <filesystem>

namespace Engine
{
    class AudioEngine
    {
    public:
        static void init();
        static void shutdown();
        static void update();

        static void playOneShot(const std::filesystem::path& path, float volume = 1.0f);

        static void setMasterVolume(float volume);
        static float masterVolume();

        static bool initialized();
    };
}