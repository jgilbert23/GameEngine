#include "Engine/Scene/Scene.hpp"

#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Renderer2D.hpp"
#include "Engine/Scene/Components.hpp"
#include "Engine/Core/Log.hpp"

namespace Engine
{
    Entity Scene::createEntity(const char* name)
    {
        Entity entity{m_registry.create(), this};

        entity.addComponent<NameComponent>(NameComponent{name});
        entity.addComponent<TransformComponent>();

        return entity;
    }

    void Scene::destroyEntity(Entity entity)
    {
        if (entity.valid())
        {
            m_registry.destroy(entity.handle());
        }
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
        m_registry.clear();
    }
}