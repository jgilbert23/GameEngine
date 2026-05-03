#include "Engine/Assets/ShaderLibrary.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/FileSystem.hpp"

namespace Engine
{
    ResourceHandle<Shader> ShaderLibrary::load(
        const std::string& id,
        std::string_view vertexSource,
        std::string_view fragmentSource
    )
    {
        if (auto it = m_shaders.find(id); it != m_shaders.end())
        {
            return ResourceHandle<Shader>(it->second);
        }

        auto shader = std::make_shared<Shader>(vertexSource, fragmentSource);
        m_shaders[id] = shader;

        return ResourceHandle<Shader>(shader);
    }

    ResourceHandle<Shader> ShaderLibrary::loadFromFiles(
        const std::string& id,
        const std::filesystem::path& vertexPath,
        const std::filesystem::path& fragmentPath
    )
    {
        if (auto it = m_shaders.find(id); it != m_shaders.end())
        {
            return ResourceHandle<Shader>(it->second);
        }

        auto vertexSource = FileSystem::readTextFile(vertexPath);
        auto fragmentSource = FileSystem::readTextFile(fragmentPath);

        ENGINE_ASSERT(vertexSource.has_value(), "Failed to load vertex shader");
        ENGINE_ASSERT(fragmentSource.has_value(), "Failed to load fragment shader");

        return load(id, *vertexSource, *fragmentSource);
    }

    ResourceHandle<Shader> ShaderLibrary::get(const std::string& id) const
    {
        auto it = m_shaders.find(id);
        ENGINE_ASSERT(it != m_shaders.end(), "Shader not found in ShaderLibrary");

        return ResourceHandle<Shader>(it->second);
    }

    bool ShaderLibrary::contains(const std::string& id) const
    {
        return m_shaders.contains(id);
    }

    void ShaderLibrary::clear()
    {
        m_shaders.clear();
    }
}