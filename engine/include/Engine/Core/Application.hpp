#pragma once

#include "Engine/Core/Config.hpp"

namespace Engine
{
    class Application
    {
    public:
        explicit Application(EngineConfig config = {});
        void run();

    private:
        EngineConfig m_config;
        bool m_running = true;

        void update();
        void render();
    };
}