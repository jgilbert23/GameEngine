#pragma once

#include "Engine/Core/Config.hpp"
#include "Engine/Core/Event.hpp"
#include "Engine/Core/Layer.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Window.hpp"

#include <memory>
#include <vector>

namespace Engine
{
    class Application
    {
    public:
        explicit Application(EngineConfig config = {});
        ~Application();

        void run();
        void pushLayer(std::unique_ptr<Layer> layer);

    private:
        EngineConfig m_config;
        std::vector<std::unique_ptr<Layer>> m_layers;
        Timer m_timer;
        std::unique_ptr<Window> m_window;

        bool m_running = true;

        static constexpr float FixedDeltaTime = 1.0f / 60.0f;
        float m_fixedTimeAccumulator = 0.0f;

        void onEvent(Event& event);
        void update(float deltaTime);
        void fixedUpdate(float fixedDeltaTime);
        void render();
    };
}