#pragma once

#include "Engine/Assets/ResourceHandle.hpp"
#include "Engine/Renderer/Texture2D.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine
{
    class TextureLibrary
    {
    public:
        ResourceHandle<Texture2D> load(const std::filesystem::path& path);
        ResourceHandle<Texture2D> get(const std::filesystem::path& path) const;

        bool contains(const std::filesystem::path& path) const;
        void clear();

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;

        static std::string normalizePath(const std::filesystem::path& path);
    };
}