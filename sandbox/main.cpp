#include "Engine/Core/Application.hpp"
#include "Engine/Core/Config.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Core/Log.hpp"

int main()
{
    Engine::EngineConfig config;
    config.applicationName = "Sandbox";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.enableVSync = true;

    ENGINE_INFO("Starting Sandbox");

    Engine::FileSystem::writeTextFile("engine_test.txt", "Game engine file utilities working.");

    auto contents = Engine::FileSystem::readTextFile("engine_test.txt");

    if (contents)
    {
        ENGINE_INFO(*contents);
    }

    Engine::Application app(config);
    app.run();

    return 0;
}