#include "Engine/Core/Application.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/Log.hpp"

namespace Engine
{
    Application::Application(EngineConfig config)
        : m_config(config)
    {
        ENGINE_ASSERT(m_config.windowWidth > 0, "Window width must be greater than zero");
        ENGINE_ASSERT(m_config.windowHeight > 0, "Window height must be greater than zero");

        ENGINE_INFO("Application created");
    }

    void Application::run()
    {
        ENGINE_INFO("Application started");

        int frameCount = 0;

        while (m_running)
        {
            update();
            render();

            frameCount++;

            if (frameCount >= 3)
            {
                m_running = false;
            }
        }

        ENGINE_INFO("Application shutdown");
    }

    void Application::update()
    {
        ENGINE_TRACE("Updating frame");
    }

    void Application::render()
    {
        ENGINE_TRACE("Rendering frame");
    }
}