#pragma once

#include <string_view>

namespace Engine
{
    struct EngineConfig
    {
        std::string_view applicationName = "GameEngine Sandbox";
        int windowWidth = 1280;
        int windowHeight = 720;
        bool enableVSync = true;
    };
}