#pragma once

#include <filesystem>
#include <string>

namespace Engine
{
    class Texture2D
    {
    public:
        explicit Texture2D(const std::filesystem::path& path);
        Texture2D(unsigned int width, unsigned int height);
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        void bind(unsigned int slot = 0) const;

        unsigned int rendererId() const;
        unsigned int width() const;
        unsigned int height() const;

    private:
        unsigned int m_rendererId = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;
        unsigned int m_internalFormat = 0;
        unsigned int m_dataFormat = 0;
    };
}