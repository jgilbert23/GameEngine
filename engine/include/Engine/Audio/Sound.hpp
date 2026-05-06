#pragma once

#include <filesystem>

namespace Engine
{
    class Sound
    {
    public:
        explicit Sound(std::filesystem::path path);

        void play(float volume = 1.0f);

        const std::filesystem::path& path() const;

    private:
        std::filesystem::path m_path;
    };
}