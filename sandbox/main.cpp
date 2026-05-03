#include "Engine/Core/Application.hpp"
#include "Engine/Core/Config.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Core/Layer.hpp"

#include "Engine/Scene/Components.hpp"
#include "Engine/Scene/Entity.hpp"
#include "Engine/Scene/Scene.hpp"

#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Renderer2D.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <random>

struct BulletComponent
{
    bool alive = true;
};

struct EnemyComponent
{
    bool alive = true;
};

struct PlayerComponent
{
    bool active = true;
};

struct LifetimeComponent
{
    float remaining = 5.0f;
};

class ECSShooterLayer final : public Engine::Layer
{
public:
    ECSShooterLayer()
        : Engine::Layer("ECSShooterLayer"),
          m_rng(std::random_device{}())
    {
    }

    void onAttach() override
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

        updatePlayerInput();
        updateShooting(dt);
        updateSpawning(dt);

        m_scene.onUpdate(dt);

        updateLifetimes(dt);
        handleCollisions();
        cleanupDeadEntities();
    }

    void onRender() override
    {
        m_scene.onRender();

        Engine::OrthographicCamera camera(0.0f, 16.0f, 0.0f, 9.0f);

        Engine::Renderer2D::beginScene(camera);

        // Lives: green bars
        for (int i = 0; i < m_lives; ++i)
        {
            Engine::Renderer2D::drawQuad(
                0.45f + static_cast<float>(i) * 0.55f,
                8.65f,
                0.45f,
                0.18f,
                0.2f, 1.0f, 0.3f, 1.0f
            );
        }

        // Score: yellow bars
        const int scoreBars = std::min(m_score, 20);

        for (int i = 0; i < scoreBars; ++i)
        {
            Engine::Renderer2D::drawQuad(
                15.5f - static_cast<float>(i) * 0.18f,
                8.65f,
                0.12f,
                0.18f,
                1.0f, 0.8f, 0.2f, 1.0f
            );
        }

        Engine::Renderer2D::endScene();
    }

private:
    static constexpr float WorldWidth = 16.0f;
    static constexpr float WorldHeight = 9.0f;

    Engine::Scene m_scene;
    Engine::Entity m_player;

    std::mt19937 m_rng;

    float m_shootCooldown = 0.0f;
    float m_shootInterval = 0.16f;

    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 0.75f;

    int m_score = 0;
    int m_lives = 3;
    bool m_gameOver = false;

private:
    void reset()
    {
        m_scene.clear();

        m_score = 0;
        m_lives = 3;
        m_gameOver = false;
        m_shootCooldown = 0.0f;
        m_spawnTimer = 0.0f;

        createCamera();
        // createBackground();
        createPlayer();
    }

    void createCamera()
    {
        auto camera = m_scene.createEntity("Main Camera");

        camera.addComponent<Engine::CameraComponent>(
            Engine::CameraComponent{
                .primary = true,
                .left = 0.0f,
                .right = WorldWidth,
                .bottom = 0.0f,
                .top = WorldHeight
            }
        );
    }

    void createBackground()
    {
        auto background = m_scene.createEntity("Background");

        auto& transform = background.getComponent<Engine::TransformComponent>();
        transform.x = WorldWidth * 0.5f;
        transform.y = WorldHeight * 0.5f;
        transform.scaleX = WorldWidth;
        transform.scaleY = WorldHeight;

        background.addComponent<Engine::SpriteComponent>(
            Engine::SpriteComponent{
                .r = 0.02f,
                .g = 0.02f,
                .b = 0.035f,
                .a = 1.0f
            }
        );
    }

    void createPlayer()
    {
        m_player = m_scene.createEntity("Player");

        auto& transform = m_player.getComponent<Engine::TransformComponent>();
        transform.x = WorldWidth * 0.5f;
        transform.y = 1.0f;
        transform.scaleX = 0.55f;
        transform.scaleY = 0.55f;

        m_player.addComponent<PlayerComponent>();

        m_player.addComponent<Engine::SpriteComponent>(
            Engine::SpriteComponent{
                .r = 0.2f,
                .g = 0.8f,
                .b = 1.0f,
                .a = 1.0f
            }
        );

        m_player.addComponent<Engine::RigidBodyComponent>();
    }

    void updatePlayerInput()
    {
        auto& transform = m_player.getComponent<Engine::TransformComponent>();
        auto& rb = m_player.getComponent<Engine::RigidBodyComponent>();

        rb.velocityX = 0.0f;
        rb.velocityY = 0.0f;

        constexpr float speed = 7.0f;

        if (Engine::Input::isKeyPressed(GLFW_KEY_A) ||
            Engine::Input::isKeyPressed(GLFW_KEY_LEFT))
            rb.velocityX -= speed;

        if (Engine::Input::isKeyPressed(GLFW_KEY_D) ||
            Engine::Input::isKeyPressed(GLFW_KEY_RIGHT))
            rb.velocityX += speed;

        if (Engine::Input::isKeyPressed(GLFW_KEY_W) ||
            Engine::Input::isKeyPressed(GLFW_KEY_UP))
            rb.velocityY += speed;

        if (Engine::Input::isKeyPressed(GLFW_KEY_S) ||
            Engine::Input::isKeyPressed(GLFW_KEY_DOWN))
            rb.velocityY -= speed;

        normalizeVelocity(rb);

        transform.x = std::clamp(
            transform.x,
            transform.scaleX * 0.5f,
            WorldWidth - transform.scaleX * 0.5f
        );

        transform.y = std::clamp(
            transform.y,
            transform.scaleY * 0.5f,
            WorldHeight - transform.scaleY * 0.5f
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

        const auto& playerTransform =
            m_player.getComponent<Engine::TransformComponent>();

        auto bullet = m_scene.createEntity("Bullet");

        auto& transform = bullet.getComponent<Engine::TransformComponent>();
        transform.x = playerTransform.x;
        transform.y = playerTransform.y + 0.45f;
        transform.scaleX = 0.12f;
        transform.scaleY = 0.35f;

        bullet.addComponent<BulletComponent>();
        bullet.addComponent<LifetimeComponent>(LifetimeComponent{2.0f});

        bullet.addComponent<Engine::SpriteComponent>(
            Engine::SpriteComponent{
                .r = 1.0f,
                .g = 0.95f,
                .b = 0.25f,
                .a = 1.0f
            }
        );

        bullet.addComponent<Engine::RigidBodyComponent>(
            Engine::RigidBodyComponent{
                .velocityX = 0.0f,
                .velocityY = 12.0f
            }
        );

        m_shootCooldown = m_shootInterval;
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

        auto enemy = m_scene.createEntity("Enemy");

        auto& transform = enemy.getComponent<Engine::TransformComponent>();
        transform.x = xDist(m_rng);
        transform.y = WorldHeight + 0.6f;
        transform.scaleX = size;
        transform.scaleY = size;

        enemy.addComponent<EnemyComponent>();
        enemy.addComponent<LifetimeComponent>(LifetimeComponent{8.0f});

        enemy.addComponent<Engine::SpriteComponent>(
            Engine::SpriteComponent{
                .r = 1.0f,
                .g = 0.2f,
                .b = 0.25f,
                .a = 1.0f
            }
        );

        enemy.addComponent<Engine::RigidBodyComponent>(
            Engine::RigidBodyComponent{
                .velocityX = driftDist(m_rng),
                .velocityY = -speedDist(m_rng)
            }
        );
    }

    void updateLifetimes(float dt)
    {
        auto view = m_scene.registry().view<
            Engine::TransformComponent,
            LifetimeComponent
        >();

        for (auto entityHandle : view)
        {
            auto& transform = view.get<Engine::TransformComponent>(entityHandle);
            auto& lifetime = view.get<LifetimeComponent>(entityHandle);

            lifetime.remaining -= dt;

            if (transform.y > WorldHeight + 2.0f ||
                transform.y < -2.0f ||
                lifetime.remaining <= 0.0f)
            {
                markDead(entityHandle);

                if (m_scene.registry().all_of<EnemyComponent>(entityHandle) &&
                    transform.y < -1.0f)
                {
                    loseLife();
                }
            }
        }
    }

    void handleCollisions()
    {
        auto bulletView = m_scene.registry().view<
            Engine::TransformComponent,
            BulletComponent
        >();

        auto enemyView = m_scene.registry().view<
            Engine::TransformComponent,
            EnemyComponent
        >();

        for (auto bulletHandle : bulletView)
        {
            auto& bullet = bulletView.get<BulletComponent>(bulletHandle);

            if (!bullet.alive)
                continue;

            const auto& bulletTransform =
                bulletView.get<Engine::TransformComponent>(bulletHandle);

            for (auto enemyHandle : enemyView)
            {
                auto& enemy = enemyView.get<EnemyComponent>(enemyHandle);

                if (!enemy.alive)
                    continue;

                const auto& enemyTransform =
                    enemyView.get<Engine::TransformComponent>(enemyHandle);

                if (intersects(bulletTransform, enemyTransform))
                {
                    bullet.alive = false;
                    enemy.alive = false;
                    m_score++;
                    break;
                }
            }
        }

        auto& playerTransform =
            m_player.getComponent<Engine::TransformComponent>();

        for (auto enemyHandle : enemyView)
        {
            auto& enemy = enemyView.get<EnemyComponent>(enemyHandle);

            if (!enemy.alive)
                continue;

            const auto& enemyTransform =
                enemyView.get<Engine::TransformComponent>(enemyHandle);

            if (intersects(playerTransform, enemyTransform))
            {
                enemy.alive = false;
                loseLife();
            }
        }
    }

    void cleanupDeadEntities()
    {
        std::vector<entt::entity> toDestroy;

        auto bulletView = m_scene.registry().view<BulletComponent>();

        for (auto entity : bulletView)
        {
            if (!bulletView.get<BulletComponent>(entity).alive)
                toDestroy.push_back(entity);
        }

        auto enemyView = m_scene.registry().view<EnemyComponent>();

        for (auto entity : enemyView)
        {
            if (!enemyView.get<EnemyComponent>(entity).alive)
                toDestroy.push_back(entity);
        }

        for (auto entity : toDestroy)
        {
            m_scene.destroyEntity(Engine::Entity{entity, &m_scene});
        }
    }

    void loseLife()
    {
        m_lives--;

        if (m_lives <= 0)
        {
            m_gameOver = true;

            auto& sprite = m_player.getComponent<Engine::SpriteComponent>();
            sprite.r = 0.4f;
            sprite.g = 0.4f;
            sprite.b = 0.4f;
        }
    }

    static void normalizeVelocity(Engine::RigidBodyComponent& rb)
    {
        const float length = std::sqrt(
            rb.velocityX * rb.velocityX +
            rb.velocityY * rb.velocityY
        );

        if (length > 0.0f)
        {
            constexpr float speed = 7.0f;

            rb.velocityX = (rb.velocityX / length) * speed;
            rb.velocityY = (rb.velocityY / length) * speed;
        }
    }

    static bool intersects(
        const Engine::TransformComponent& a,
        const Engine::TransformComponent& b
    )
    {
        const bool overlapX =
            std::abs(a.x - b.x) <=
            (a.scaleX * 0.5f + b.scaleX * 0.5f);

        const bool overlapY =
            std::abs(a.y - b.y) <=
            (a.scaleY * 0.5f + b.scaleY * 0.5f);

        return overlapX && overlapY;
    }

    void markDead(entt::entity entityHandle)
    {
        if (m_scene.registry().all_of<BulletComponent>(entityHandle))
            m_scene.registry().get<BulletComponent>(entityHandle).alive = false;

        if (m_scene.registry().all_of<EnemyComponent>(entityHandle))
            m_scene.registry().get<EnemyComponent>(entityHandle).alive = false;
    }
};

int main()
{
    Engine::EngineConfig config;
    config.applicationName = "ECS Top-Down Shooter";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.enableVSync = true;

    Engine::Application app(config);
    app.pushLayer(std::make_unique<ECSShooterLayer>());
    app.run();

    return 0;
}