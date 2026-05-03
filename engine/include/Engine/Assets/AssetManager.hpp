#pragma once

#include "Engine/Assets/ResourceHandle.hpp"
#include "Engine/Assets/ShaderLibrary.hpp"
#include "Engine/Assets/TextureLibrary.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

namespace Engine
{
    class AudioClip
    {
    public:
        explicit AudioClip(std::filesystem::path path)
            : m_path(std::move(path))
        {
        }

        const std::filesystem::path& path() const
        {
            return m_path;
        }

    private:
        std::filesystem::path m_path;
    };

    class JsonAsset
    {
    public:
        explicit JsonAsset(std::string text)
            : m_text(std::move(text))
        {
        }

        const std::string& text() const
        {
            return m_text;
        }

    private:
        std::string m_text;
    };

    class AssetManager
    {
    public:
        ResourceHandle<Texture2D> loadTexture(const std::filesystem::path& path);

        ResourceHandle<Shader> loadShader(
            const std::string& id,
            std::string_view vertexSource,
            std::string_view fragmentSource
        );

        ResourceHandle<Shader> loadShaderFromFiles(
            const std::string& id,
            const std::filesystem::path& vertexPath,
            const std::filesystem::path& fragmentPath
        );

        ResourceHandle<AudioClip> loadAudio(const std::filesystem::path& path);
        ResourceHandle<JsonAsset> loadJson(const std::filesystem::path& path);

        TextureLibrary& textures();
        ShaderLibrary& shaders();

        void clear();

    private:
        TextureLibrary m_textureLibrary;
        ShaderLibrary m_shaderLibrary;

        std::unordered_map<std::string, std::shared_ptr<AudioClip>> m_audioClips;
        std::unordered_map<std::string, std::shared_ptr<JsonAsset>> m_jsonAssets;

        static std::string normalizePath(const std::filesystem::path& path);
    };
}