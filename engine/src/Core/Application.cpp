#include "Engine/Core/Application.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Renderer/RenderCommand.hpp"
#include "Engine/Renderer/Renderer2D.hpp"

#include <algorithm>

namespace Engine
{
    Application::Application(EngineConfig config)
        : m_config(config)
    {
        ENGINE_ASSERT(m_config.windowWidth > 0, "Window width must be greater than zero");
        ENGINE_ASSERT(m_config.windowHeight > 0, "Window height must be greater than zero");

        WindowConfig windowConfig;
        windowConfig.title = std::string(m_config.applicationName);
        windowConfig.width = m_config.windowWidth;
        windowConfig.height = m_config.windowHeight;
        windowConfig.vSync = m_config.enableVSync;

        m_window = std::make_unique<Window>(windowConfig);

        RenderCommand::init();
        Renderer2D::init();

        RenderCommand::setViewport(0, 0, m_config.windowWidth, m_config.windowHeight);

        m_window->setEventCallback([this](Event& event)
        {
            onEvent(event);
        });

        ENGINE_INFO("Application created");
    }

    Application::~Application()
    {
        for (auto& layer : m_layers)
        {
            layer->onDetach();
        }

        m_layers.clear();
        Renderer2D::shutdown();

        ENGINE_INFO("Application destroyed");
    }

    void Application::run()
    {
        ENGINE_INFO("Application started");

        while (m_running && !m_window->shouldClose())
        {
            const float deltaTime = m_timer.tick();

            m_window->pollEvents();

            m_fixedTimeAccumulator += deltaTime;

            while (m_fixedTimeAccumulator >= FixedDeltaTime)
            {
                fixedUpdate(FixedDeltaTime);
                m_fixedTimeAccumulator -= FixedDeltaTime;
            }

            update(deltaTime);
            render();

            m_window->swapBuffers();
        }

        ENGINE_INFO("Application shutdown");
    }

    void Application::pushLayer(std::unique_ptr<Layer> layer)
    {
        layer->onAttach();
        m_layers.emplace_back(std::move(layer));
    }

    void Application::onEvent(Event& event)
    {
        if (event.type == EventType::WindowClose)
        {
            m_running = false;
            event.handled = true;
        }

        if (event.type == EventType::WindowResize)
        {
            ENGINE_INFO("Window resized");
        }

        for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
        {
            if (event.handled)
            {
                break;
            }

            (*it)->onEvent(event);
        }
    }

    void Application::update(float deltaTime)
    {
        for (auto& layer : m_layers)
        {
            layer->onUpdate(deltaTime);
        }
    }

    void Application::fixedUpdate(float fixedDeltaTime)
    {
        for (auto& layer : m_layers)
        {
            layer->onFixedUpdate(fixedDeltaTime);
        }
    }

    void Application::render()
    {
        RenderCommand::setClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        RenderCommand::clear();
    }
}