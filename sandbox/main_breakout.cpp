#include "Engine/Core/Application.hpp"
#include "Engine/Core/Config.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Core/Layer.hpp"
#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Renderer2D.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Brick
{
    Vec2 position;
    Vec2 size;
    bool alive = true;
};

class BreakoutLayer final : public Engine::Layer
{
public:
    BreakoutLayer()
        : Engine::Layer("BreakoutLayer"),
          m_camera(0.0f, WorldWidth, 0.0f, WorldHeight)
    {
        reset();
    }

    void onUpdate(float dt) override
    {
        updatePaddle(dt);
        updateBall(dt);
    }

private:
    static constexpr float WorldWidth = 16.0f;
    static constexpr float WorldHeight = 9.0f;

    Vec2 m_paddlePosition{8.0f, 0.7f};
    Vec2 m_paddleSize{2.4f, 0.35f};
    float m_paddleSpeed = 9.0f;

    Vec2 m_ballPosition{8.0f, 2.0f};
    Vec2 m_ballVelocity{4.2f, 5.5f};
    Vec2 m_ballSize{0.28f, 0.28f};

    std::vector<Brick> m_bricks;
    Engine::OrthographicCamera m_camera;

private:
    void reset()
    {
        m_paddlePosition = {8.0f, 0.7f};
        m_ballPosition = {8.0f, 2.0f};
        m_ballVelocity = {4.2f, 5.5f};

        m_bricks.clear();

        constexpr int rows = 5;
        constexpr int cols = 10;

        const float brickWidth = 1.35f;
        const float brickHeight = 0.45f;
        const float spacing = 0.12f;

        const float startX = 1.0f;
        const float startY = 7.8f;

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                Brick brick;
                brick.size = {brickWidth, brickHeight};
                brick.position = {
                    startX + col * (brickWidth + spacing),
                    startY - row * (brickHeight + spacing)
                };

                m_bricks.push_back(brick);
            }
        }
    }

    void updatePaddle(float dt)
    {
        if (Engine::Input::isKeyPressed(GLFW_KEY_A) ||
            Engine::Input::isKeyPressed(GLFW_KEY_LEFT))
        {
            m_paddlePosition.x -= m_paddleSpeed * dt;
        }

        if (Engine::Input::isKeyPressed(GLFW_KEY_D) ||
            Engine::Input::isKeyPressed(GLFW_KEY_RIGHT))
        {
            m_paddlePosition.x += m_paddleSpeed * dt;
        }

        m_paddlePosition.x = std::clamp(
            m_paddlePosition.x,
            m_paddleSize.x * 0.5f,
            WorldWidth - m_paddleSize.x * 0.5f
        );

        if (Engine::Input::isKeyPressed(GLFW_KEY_R))
        {
            reset();
        }
    }

    void updateBall(float dt)
    {
        m_ballPosition.x += m_ballVelocity.x * dt;
        m_ballPosition.y += m_ballVelocity.y * dt;

        if (m_ballPosition.x - m_ballSize.x * 0.5f <= 0.0f)
        {
            m_ballPosition.x = m_ballSize.x * 0.5f;
            m_ballVelocity.x *= -1.0f;
        }

        if (m_ballPosition.x + m_ballSize.x * 0.5f >= WorldWidth)
        {
            m_ballPosition.x = WorldWidth - m_ballSize.x * 0.5f;
            m_ballVelocity.x *= -1.0f;
        }

        if (m_ballPosition.y + m_ballSize.y * 0.5f >= WorldHeight)
        {
            m_ballPosition.y = WorldHeight - m_ballSize.y * 0.5f;
            m_ballVelocity.y *= -1.0f;
        }

        if (m_ballPosition.y < 0.0f)
        {
            reset();
            return;
        }

        handlePaddleCollision();
        handleBrickCollisions();
    }

    void handlePaddleCollision()
    {
        if (!intersects(m_ballPosition, m_ballSize, m_paddlePosition, m_paddleSize))
        {
            return;
        }

        m_ballPosition.y = m_paddlePosition.y + m_paddleSize.y * 0.5f + m_ballSize.y * 0.5f;
        m_ballVelocity.y = std::abs(m_ballVelocity.y);

        const float offset =
            (m_ballPosition.x - m_paddlePosition.x) / (m_paddleSize.x * 0.5f);

        m_ballVelocity.x = offset * 5.5f;
    }

    void handleBrickCollisions()
    {
        for (auto& brick : m_bricks)
        {
            if (!brick.alive)
            {
                continue;
            }

            if (intersects(m_ballPosition, m_ballSize, brick.position, brick.size))
            {
                brick.alive = false;
                m_ballVelocity.y *= -1.0f;
                break;
            }
        }

        const bool allDestroyed = std::all_of(
            m_bricks.begin(),
            m_bricks.end(),
            [](const Brick& brick)
            {
                return !brick.alive;
            }
        );

        if (allDestroyed)
        {
            reset();
        }
    }

    static bool intersects(
        const Vec2& aPosition,
        const Vec2& aSize,
        const Vec2& bPosition,
        const Vec2& bSize
    )
    {
        const bool overlapX =
            std::abs(aPosition.x - bPosition.x) <=
            (aSize.x * 0.5f + bSize.x * 0.5f);

        const bool overlapY =
            std::abs(aPosition.y - bPosition.y) <=
            (aSize.y * 0.5f + bSize.y * 0.5f);

        return overlapX && overlapY;
    }

    void onRender() override
    {
        render();
    }

    void render()
    {
        Engine::Renderer2D::beginScene(m_camera);

        Engine::Renderer2D::drawQuad(
            m_paddlePosition.x,
            m_paddlePosition.y,
            m_paddleSize.x,
            m_paddleSize.y,
            0.2f,
            0.7f,
            1.0f,
            1.0f
        );

        Engine::Renderer2D::drawQuad(
            m_ballPosition.x,
            m_ballPosition.y,
            m_ballSize.x,
            m_ballSize.y,
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        for (const auto& brick : m_bricks)
        {
            if (!brick.alive)
            {
                continue;
            }

            Engine::Renderer2D::drawQuad(
                brick.position.x,
                brick.position.y,
                brick.size.x,
                brick.size.y,
                1.0f,
                0.35f,
                0.25f,
                1.0f
            );
        }

        Engine::Renderer2D::endScene();
    }
};

int main()
{
    Engine::EngineConfig config;
    config.applicationName = "Breakout";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.enableVSync = true;

    Engine::Application app(config);
    app.pushLayer(std::make_unique<BreakoutLayer>());
    app.run();

    return 0;
}