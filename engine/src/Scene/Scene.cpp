#include "Engine/Scene/Scene.hpp"

#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Renderer2D.hpp"
#include "Engine/Core/Log.hpp"

#include "Engine/Scene/Components.hpp"
#include "Engine/Scene/PhysicsComponents.hpp"

#include <box2d/box2d.h>
namespace Engine
{
    Entity Scene::createEntity(const char* name)
    {
        Entity entity{m_registry.create(), this};

        entity.addComponent<NameComponent>(NameComponent{name});
        entity.addComponent<TransformComponent>();

        return entity;
    }

    Scene::Scene() = default;
    
    Scene::~Scene()
    {
        onPhysicsStop();
    }

    static b2BodyType toBox2DBodyType(Engine::BodyType2D type)
    {
        switch (type)
        {
            case Engine::BodyType2D::Static:    return b2_staticBody;
            case Engine::BodyType2D::Dynamic:   return b2_dynamicBody;
            case Engine::BodyType2D::Kinematic: return b2_kinematicBody;
        }

        return b2_staticBody;
    }

    void Scene::onPhysicsStart()
    {
        m_physicsWorld = std::make_unique<b2World>(b2Vec2{0.0f, -9.8f});

        auto view = m_registry.view<TransformComponent, RigidBody2DComponent>();

        for (auto entityHandle : view)
        {
            createPhysicsBody(Entity{entityHandle, this});
        }
    }

    void Scene::onPhysicsStop()
    {
        if (!m_physicsWorld)
            return;

        auto view = m_registry.view<RigidBody2DComponent>();

        for (auto entityHandle : view)
        {
            auto& rb = view.get<RigidBody2DComponent>(entityHandle);
            rb.runtimeBody = nullptr;
        }

        m_physicsWorld.reset();
    }

    void Scene::createPhysicsBody(Entity entity)
    {
        if (!m_physicsWorld || !entity.valid())
            return;

        auto& transform = entity.getComponent<TransformComponent>();
        auto& rb = entity.getComponent<RigidBody2DComponent>();

        b2BodyDef bodyDef;
        bodyDef.type = toBox2DBodyType(rb.type);
        bodyDef.position.Set(transform.x, transform.y);
        bodyDef.angle = transform.rotation;
        bodyDef.fixedRotation = rb.fixedRotation;

        rb.runtimeBody = m_physicsWorld->CreateBody(&bodyDef);

        if (entity.hasComponent<BoxCollider2DComponent>())
        {
            auto& collider = entity.getComponent<BoxCollider2DComponent>();

            b2PolygonShape boxShape;
            boxShape.SetAsBox(
                collider.sizeX * transform.scaleX,
                collider.sizeY * transform.scaleY,
                b2Vec2{collider.offsetX, collider.offsetY},
                0.0f
            );

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = collider.density;
            fixtureDef.friction = collider.friction;
            fixtureDef.restitution = collider.restitution;
            fixtureDef.restitutionThreshold = collider.restitutionThreshold;

            rb.runtimeBody->CreateFixture(&fixtureDef);
        }

        if (entity.hasComponent<CircleCollider2DComponent>())
        {
            auto& collider = entity.getComponent<CircleCollider2DComponent>();

            b2CircleShape circleShape;
            circleShape.m_p.Set(collider.offsetX, collider.offsetY);
            circleShape.m_radius = collider.radius * transform.scaleX;

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = collider.density;
            fixtureDef.friction = collider.friction;
            fixtureDef.restitution = collider.restitution;
            fixtureDef.restitutionThreshold = collider.restitutionThreshold;

            rb.runtimeBody->CreateFixture(&fixtureDef);
        }
    }  
    
    void Scene::onPhysicsStep(float dt)
    {
        if (!m_physicsWorld)
            return;

        constexpr int velocityIterations = 8;
        constexpr int positionIterations = 3;

        m_physicsWorld->Step(dt, velocityIterations, positionIterations);

        auto view = m_registry.view<TransformComponent, RigidBody2DComponent>();

        for (auto entityHandle : view)
        {
            auto& transform = view.get<TransformComponent>(entityHandle);
            auto& rb = view.get<RigidBody2DComponent>(entityHandle);

            if (!rb.runtimeBody)
                continue;

            const b2Vec2& position = rb.runtimeBody->GetPosition();

            transform.x = position.x;
            transform.y = position.y;
            transform.rotation = rb.runtimeBody->GetAngle();
        }
    }

    void Scene::destroyEntity(Entity entity)
    {
        if (!entity.valid())
            return;

        if (m_physicsWorld && entity.hasComponent<RigidBody2DComponent>())
        {
            auto& rb = entity.getComponent<RigidBody2DComponent>();

            if (rb.runtimeBody)
            {
                m_physicsWorld->DestroyBody(rb.runtimeBody);
                rb.runtimeBody = nullptr;
            }
        }

        m_registry.destroy(entity.handle());
    }

    void Scene::onUpdate(float dt)
    {
        auto view = m_registry.view<TransformComponent, RigidBodyComponent>();

        for (auto entity : view)
        {
            auto& transform = view.get<TransformComponent>(entity);
            auto& rb = view.get<RigidBodyComponent>(entity);

            rb.velocityX += rb.accelerationX * dt;
            rb.velocityY += rb.accelerationY * dt;

            transform.x += rb.velocityX * dt;
            transform.y += rb.velocityY * dt;
        }
    }

    void Scene::onRender()
    {
        OrthographicCamera camera(0.0f, 16.0f, 0.0f, 9.0f);

        auto cameraView = m_registry.view<TransformComponent, CameraComponent>();

        for (auto entityHandle : cameraView)
        {
            const auto& cameraComponent = cameraView.get<CameraComponent>(entityHandle);

            if (cameraComponent.primary)
            {
                camera = OrthographicCamera(
                    cameraComponent.left,
                    cameraComponent.right,
                    cameraComponent.bottom,
                    cameraComponent.top
                );

                const auto& transform = cameraView.get<TransformComponent>(entityHandle);
                // camera.setPosition(transform.x, transform.y, transform.z);
                // camera.setRotation(transform.rotation);
                break;
            }
        }

        Renderer2D::beginScene(camera);

        auto spriteView = m_registry.view<TransformComponent, SpriteComponent>();

        for (auto entityHandle : spriteView)
        {
            const auto& transform = spriteView.get<TransformComponent>(entityHandle);
            const auto& sprite = spriteView.get<SpriteComponent>(entityHandle);

            if (sprite.texture)
            {
                Renderer2D::drawTexturedQuad(
                    transform.x,
                    transform.y,
                    transform.scaleX,
                    transform.scaleY,
                    *sprite.texture
                );
            }
            else
            {
                Renderer2D::drawQuad(
                    transform.x,
                    transform.y,
                    transform.scaleX,
                    transform.scaleY,
                    sprite.r,
                    sprite.g,
                    sprite.b,
                    sprite.a
                );
            }
        }

        Renderer2D::endScene();
    }

    entt::registry& Scene::registry()
    {
        return m_registry;
    }

    const entt::registry& Scene::registry() const
    {
        return m_registry;
    }

    void Scene::clear()
    {
        onPhysicsStop();
        m_registry.clear();
    }    
}