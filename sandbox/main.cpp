#include "Engine/Core/Application.hpp"
#include "Engine/Core/Config.hpp"
#include "Engine/Core/Layer.hpp"
#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Renderer2D.hpp"

#include <memory>

class SandboxLayer final : public Engine::Layer
{
public:
    SandboxLayer()
        : Engine::Layer("SandboxLayer"),
          m_camera(-10.0f, 10.0f, -5.625f, 5.625f)
    {
    }

    void onUpdate(float deltaTime) override
    {
        (void)deltaTime;

        Engine::Renderer2D::beginScene(m_camera);

        Engine::Renderer2D::drawQuad(
            0.0f,
            0.0f,
            2.0f,
            2.0f,
            0.2f,
            0.6f,
            1.0f,
            1.0f
        );

        Engine::Renderer2D::drawQuad(
            3.0f,
            0.0f,
            1.5f,
            1.5f,
            1.0f,
            0.3f,
            0.2f,
            1.0f
        );

        Engine::Renderer2D::endScene();
    }

private:
    Engine::OrthographicCamera m_camera;
};

int main()
{
    Engine::EngineConfig config;
    config.applicationName = "Renderer Sandbox";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.enableVSync = true;

    Engine::Application app(config);
    app.pushLayer(std::make_unique<SandboxLayer>());
    app.run();

    return 0;
}