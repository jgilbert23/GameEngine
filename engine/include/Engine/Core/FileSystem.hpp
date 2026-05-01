#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace Engine
{
    class FileSystem
    {
    public:
        static bool exists(const std::filesystem::path& path);
        static std::optional<std::string> readTextFile(const std::filesystem::path& path);
        static bool writeTextFile(const std::filesystem::path& path, std::string_view content);
    };
}