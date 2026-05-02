#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/Renderer/Texture2D.hpp"
#include "Engine/Core/Assert.hpp"

#include <glad/glad.h>

namespace Engine
{
    Texture2D::Texture2D(const std::filesystem::path& path)
    {
        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

        ENGINE_ASSERT(data != nullptr, "Failed to load texture");

        m_width = static_cast<unsigned int>(width);
        m_height = static_cast<unsigned int>(height);

        if (channels == 4)
        {
            m_internalFormat = GL_RGBA8;
            m_dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_internalFormat = GL_RGB8;
            m_dataFormat = GL_RGB;
        }
        else
        {
            ENGINE_ASSERT(false, "Unsupported texture format");
        }

        glGenTextures(1, &m_rendererId);
        glBindTexture(GL_TEXTURE_2D, m_rendererId);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            m_internalFormat,
            m_width,
            m_height,
            0,
            m_dataFormat,
            GL_UNSIGNED_BYTE,
            data
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    }

    Texture2D::Texture2D(unsigned int width, unsigned int height)
        : m_width(width), m_height(height), m_internalFormat(GL_RGBA8), m_dataFormat(GL_RGBA)
    {
        glGenTextures(1, &m_rendererId);
        glBindTexture(GL_TEXTURE_2D, m_rendererId);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            m_internalFormat,
            m_width,
            m_height,
            0,
            m_dataFormat,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_rendererId);
    }

    void Texture2D::bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_rendererId);
    }

    unsigned int Texture2D::rendererId() const
    {
        return m_rendererId;
    }

    unsigned int Texture2D::width() const
    {
        return m_width;
    }

    unsigned int Texture2D::height() const
    {
        return m_height;
    }
}