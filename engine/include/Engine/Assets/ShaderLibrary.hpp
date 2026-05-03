#pragma once

#include "Engine/Assets/ResourceHandle.hpp"
#include "Engine/Renderer/Shader.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace Engine
{
    class ShaderLibrary
    {
    public:
        ResourceHandle<Shader> load(
            const std::string& id,
            std::string_view vertexSource,
            std::string_view fragmentSource
        );

        ResourceHandle<Shader> loadFromFiles(
            const std::string& id,
            const std::filesystem::path& vertexPath,
            const std::filesystem::path& fragmentPath
        );

        ResourceHandle<Shader> get(const std::string& id) const;

        bool contains(const std::string& id) const;
        void clear();

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
    };
}