#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Renderer/Texture2D.hpp"

namespace Engine
{
    ResourceHandle<Texture2D> AssetManager::loadTexture(const std::filesystem::path& path)
    {
        return m_textureLibrary.load(path);
    }

    ResourceHandle<Shader> AssetManager::loadShader(
        const std::string& id,
        std::string_view vertexSource,
        std::string_view fragmentSource
    )
    {
        return m_shaderLibrary.load(id, vertexSource, fragmentSource);
    }

    ResourceHandle<Shader> AssetManager::loadShaderFromFiles(
        const std::string& id,
        const std::filesystem::path& vertexPath,
        const std::filesystem::path& fragmentPath
    )
    {
        return m_shaderLibrary.loadFromFiles(id, vertexPath, fragmentPath);
    }

    ResourceHandle<AudioClip> AssetManager::loadAudio(const std::filesystem::path& path)
    {
        const std::string key = normalizePath(path);

        if (auto it = m_audioClips.find(key); it != m_audioClips.end())
        {
            return ResourceHandle<AudioClip>(it->second);
        }

        ENGINE_ASSERT(FileSystem::exists(path), "Audio file does not exist");

        auto clip = std::make_shared<AudioClip>(path);
        m_audioClips[key] = clip;

        return ResourceHandle<AudioClip>(clip);
    }

    ResourceHandle<JsonAsset> AssetManager::loadJson(const std::filesystem::path& path)
    {
        const std::string key = normalizePath(path);

        if (auto it = m_jsonAssets.find(key); it != m_jsonAssets.end())
        {
            return ResourceHandle<JsonAsset>(it->second);
        }

        auto text = FileSystem::readTextFile(path);
        ENGINE_ASSERT(text.has_value(), "Failed to load JSON file");

        auto json = std::make_shared<JsonAsset>(*text);
        m_jsonAssets[key] = json;

        return ResourceHandle<JsonAsset>(json);
    }

    TextureLibrary& AssetManager::textures()
    {
        return m_textureLibrary;
    }

    ShaderLibrary& AssetManager::shaders()
    {
        return m_shaderLibrary;
    }

    void AssetManager::clear()
    {
        m_jsonAssets.clear();
        m_audioClips.clear();

        m_shaderLibrary.clear();
        m_textureLibrary.clear();
    }

    std::string AssetManager::normalizePath(const std::filesystem::path& path)
    {
        return std::filesystem::weakly_canonical(path).string();
    }
}