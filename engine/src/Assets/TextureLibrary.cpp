#include "Engine/Assets/TextureLibrary.hpp"
#include "Engine/Core/Assert.hpp"

namespace Engine
{
    ResourceHandle<Texture2D> TextureLibrary::load(const std::filesystem::path& path)
    {
        const std::string key = normalizePath(path);

        if (auto it = m_textures.find(key); it != m_textures.end())
        {
            return ResourceHandle<Texture2D>(it->second);
        }

        auto texture = std::make_shared<Texture2D>(path);
        m_textures[key] = texture;

        return ResourceHandle<Texture2D>(texture);
    }

    ResourceHandle<Texture2D> TextureLibrary::get(const std::filesystem::path& path) const
    {
        const std::string key = normalizePath(path);

        auto it = m_textures.find(key);
        ENGINE_ASSERT(it != m_textures.end(), "Texture not found in TextureLibrary");

        return ResourceHandle<Texture2D>(it->second);
    }

    bool TextureLibrary::contains(const std::filesystem::path& path) const
    {
        return m_textures.contains(normalizePath(path));
    }

    void TextureLibrary::clear()
    {
        m_textures.clear();
    }

    std::string TextureLibrary::normalizePath(const std::filesystem::path& path)
    {
        return std::filesystem::weakly_canonical(path).string();
    }
}