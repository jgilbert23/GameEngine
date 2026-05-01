#include "Engine/Core/FileSystem.hpp"
#include "Engine/Core/Log.hpp"

#include <fstream>
#include <sstream>

namespace Engine
{
    bool FileSystem::exists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }

    std::optional<std::string> FileSystem::readTextFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);

        if (!file)
        {
            ENGINE_ERROR("Failed to open file for reading");
            return std::nullopt;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }

    bool FileSystem::writeTextFile(const std::filesystem::path& path, std::string_view content)
    {
        std::ofstream file(path);

        if (!file)
        {
            ENGINE_ERROR("Failed to open file for writing");
            return false;
        }

        file << content;
        return true;
    }
}