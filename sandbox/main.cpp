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
#include <random>
#include <string>
#include <vector>

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Bullet
{
    Vec2 position;
    Vec2 velocity;
    Vec2 size;
    bool alive = true;
};

struct Enemy
{
    Vec2 position;
    Vec2 velocity;
    Vec2 size;
    bool alive = true;
};

class TopDownShooterLayer final : public Engine::Layer
{
public:
    TopDownShooterLayer()
        : Engine::Layer("TopDownShooterLayer"),
          m_camera(0.0f, WorldWidth, 0.0f, WorldHeight),
          m_rng(std::random_device{}())
    {
        reset();
    }

    void onUpdate(float dt) override
    {
        if (m_gameOver)
        {
            if (Engine::Input::isKeyPressed(GLFW_KEY_R))
                reset();

            return;
        }

        updatePlayer(dt);
        updateShooting(dt);
        updateBullets(dt);
        updateEnemies(dt);
        updateSpawning(dt);
        handleCollisions();
        cleanup();
    }

    void onRender() override
    {
        Engine::Renderer2D::beginScene(m_camera);

        drawBackground();
        drawPlayer();
        drawBullets();
        drawEnemies();
        drawHudBars();

        Engine::Renderer2D::endScene();
    }

private:
    static constexpr float WorldWidth = 16.0f;
    static constexpr float WorldHeight = 9.0f;

    Vec2 m_playerPosition{8.0f, 1.0f};
    Vec2 m_playerSize{0.55f, 0.55f};
    float m_playerSpeed = 7.0f;

    std::vector<Bullet> m_bullets;
    std::vector<Enemy> m_enemies;

    float m_shootCooldown = 0.0f;
    float m_shootInterval = 0.16f;

    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 0.75f;

    int m_score = 0;
    int m_lives = 3;
    bool m_gameOver = false;

    Engine::OrthographicCamera m_camera;
    std::mt19937 m_rng;

private:
    void reset()
    {
        m_playerPosition = {WorldWidth * 0.5f, 1.0f};
        m_bullets.clear();
        m_enemies.clear();

        m_shootCooldown = 0.0f;
        m_spawnTimer = 0.0f;

        m_score = 0;
        m_lives = 3;
        m_gameOver = false;
    }

    void updatePlayer(float dt)
    {
        Vec2 movement{};

        if (Engine::Input::isKeyPressed(GLFW_KEY_A) ||
            Engine::Input::isKeyPressed(GLFW_KEY_LEFT))
            movement.x -= 1.0f;

        if (Engine::Input::isKeyPressed(GLFW_KEY_D) ||
            Engine::Input::isKeyPressed(GLFW_KEY_RIGHT))
            movement.x += 1.0f;

        if (Engine::Input::isKeyPressed(GLFW_KEY_W) ||
            Engine::Input::isKeyPressed(GLFW_KEY_UP))
            movement.y += 1.0f;

        if (Engine::Input::isKeyPressed(GLFW_KEY_S) ||
            Engine::Input::isKeyPressed(GLFW_KEY_DOWN))
            movement.y -= 1.0f;

        normalizeIfNeeded(movement);

        m_playerPosition.x += movement.x * m_playerSpeed * dt;
        m_playerPosition.y += movement.y * m_playerSpeed * dt;

        m_playerPosition.x = std::clamp(
            m_playerPosition.x,
            m_playerSize.x * 0.5f,
            WorldWidth - m_playerSize.x * 0.5f
        );

        m_playerPosition.y = std::clamp(
            m_playerPosition.y,
            m_playerSize.y * 0.5f,
            WorldHeight - m_playerSize.y * 0.5f
        );
    }

    void updateShooting(float dt)
    {
        m_shootCooldown -= dt;

        const bool shoot =
            Engine::Input::isKeyPressed(GLFW_KEY_SPACE) ||
            Engine::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

        if (!shoot || m_shootCooldown > 0.0f)
            return;

        Bullet bullet;
        bullet.position = {m_playerPosition.x, m_playerPosition.y + 0.45f};
        bullet.velocity = {0.0f, 12.0f};
        bullet.size = {0.12f, 0.35f};

        m_bullets.push_back(bullet);
        m_shootCooldown = m_shootInterval;
    }

    void updateBullets(float dt)
    {
        for (auto& bullet : m_bullets)
        {
            bullet.position.x += bullet.velocity.x * dt;
            bullet.position.y += bullet.velocity.y * dt;

            if (bullet.position.y > WorldHeight + 1.0f)
                bullet.alive = false;
        }
    }

    void updateEnemies(float dt)
    {
        for (auto& enemy : m_enemies)
        {
            enemy.position.x += enemy.velocity.x * dt;
            enemy.position.y += enemy.velocity.y * dt;

            if (enemy.position.y < -1.0f)
            {
                enemy.alive = false;
                loseLife();
            }
        }
    }

    void updateSpawning(float dt)
    {
        m_spawnTimer -= dt;

        if (m_spawnTimer > 0.0f)
            return;

        spawnEnemy();

        const float difficulty = std::min(static_cast<float>(m_score) * 0.015f, 0.45f);
        m_spawnTimer = std::max(0.25f, m_spawnInterval - difficulty);
    }

    void spawnEnemy()
    {
        std::uniform_real_distribution<float> xDist(0.8f, WorldWidth - 0.8f);
        std::uniform_real_distribution<float> speedDist(1.5f, 3.6f);
        std::uniform_real_distribution<float> driftDist(-0.6f, 0.6f);
        std::uniform_real_distribution<float> sizeDist(0.45f, 0.85f);

        const float size = sizeDist(m_rng);

        Enemy enemy;
        enemy.position = {xDist(m_rng), WorldHeight + 0.6f};
        enemy.velocity = {driftDist(m_rng), -speedDist(m_rng)};
        enemy.size = {size, size};
        enemy.alive = true;

        m_enemies.push_back(enemy);
    }

    void handleCollisions()
    {
        for (auto& bullet : m_bullets)
        {
            if (!bullet.alive)
                continue;

            for (auto& enemy : m_enemies)
            {
                if (!enemy.alive)
                    continue;

                if (intersects(bullet.position, bullet.size, enemy.position, enemy.size))
                {
                    bullet.alive = false;
                    enemy.alive = false;
                    m_score++;
                    break;
                }
            }
        }

        for (auto& enemy : m_enemies)
        {
            if (!enemy.alive)
                continue;

            if (intersects(m_playerPosition, m_playerSize, enemy.position, enemy.size))
            {
                enemy.alive = false;
                loseLife();
            }
        }
    }

    void cleanup()
    {
        m_bullets.erase(
            std::remove_if(
                m_bullets.begin(),
                m_bullets.end(),
                [](const Bullet& bullet)
                {
                    return !bullet.alive;
                }
            ),
            m_bullets.end()
        );

        m_enemies.erase(
            std::remove_if(
                m_enemies.begin(),
                m_enemies.end(),
                [](const Enemy& enemy)
                {
                    return !enemy.alive;
                }
            ),
            m_enemies.end()
        );
    }

    void loseLife()
    {
        m_lives--;

        if (m_lives <= 0)
            m_gameOver = true;
    }

    static void normalizeIfNeeded(Vec2& v)
    {
        const float length = std::sqrt(v.x * v.x + v.y * v.y);

        if (length > 0.0f)
        {
            v.x /= length;
            v.y /= length;
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

    void drawBackground()
    {
        Engine::Renderer2D::drawQuad(
            WorldWidth * 0.5f,
            WorldHeight * 0.5f,
            WorldWidth,
            WorldHeight,
            0.02f,
            0.02f,
            0.035f,
            1.0f
        );
    }

    void drawPlayer()
    {
        const float r = m_gameOver ? 0.4f : 0.2f;
        const float g = m_gameOver ? 0.4f : 0.8f;
        const float b = m_gameOver ? 0.4f : 1.0f;

        Engine::Renderer2D::drawQuad(
            m_playerPosition.x,
            m_playerPosition.y,
            m_playerSize.x,
            m_playerSize.y,
            r,
            g,
            b,
            1.0f
        );
    }

    void drawBullets()
    {
        for (const auto& bullet : m_bullets)
        {
            Engine::Renderer2D::drawQuad(
                bullet.position.x,
                bullet.position.y,
                bullet.size.x,
                bullet.size.y,
                1.0f,
                0.95f,
                0.25f,
                1.0f
            );
        }
    }

    void drawEnemies()
    {
        for (const auto& enemy : m_enemies)
        {
            Engine::Renderer2D::drawQuad(
                enemy.position.x,
                enemy.position.y,
                enemy.size.x,
                enemy.size.y,
                1.0f,
                0.2f,
                0.25f,
                1.0f
            );
        }
    }

    void drawHudBars()
    {
        const float lifeWidth = 0.45f;
        const float lifeHeight = 0.18f;

        for (int i = 0; i < m_lives; ++i)
        {
            Engine::Renderer2D::drawQuad(
                0.45f + static_cast<float>(i) * 0.55f,
                8.65f,
                lifeWidth,
                lifeHeight,
                0.2f,
                1.0f,
                0.3f,
                1.0f
            );
        }

        const int scoreBars = std::min(m_score, 20);

        for (int i = 0; i < scoreBars; ++i)
        {
            Engine::Renderer2D::drawQuad(
                15.5f - static_cast<float>(i) * 0.18f,
                8.65f,
                0.12f,
                0.18f,
                1.0f,
                0.8f,
                0.2f,
                1.0f
            );
        }

        if (m_gameOver)
        {
            Engine::Renderer2D::drawQuad(
                WorldWidth * 0.5f,
                WorldHeight * 0.5f,
                5.0f,
                1.2f,
                0.7f,
                0.1f,
                0.1f,
                1.0f
            );
        }
    }
};

int main()
{
    Engine::EngineConfig config;
    config.applicationName = "Top-Down Shooter";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.enableVSync = true;

    Engine::Application app(config);
    app.pushLayer(std::make_unique<TopDownShooterLayer>());
    app.run();

    return 0;
}