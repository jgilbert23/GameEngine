#pragma once

#include <entt/entt.hpp>

#include "Engine/Scene/Entity.hpp"

#include <memory>

class b2World;
namespace Engine
{
    class OrthographicCamera;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity createEntity(const char* name = "Entity");
        void destroyEntity(Entity entity);

        void clear();

        void onUpdate(float dt);
        void onRender();

        void onPhysicsStart();
        void onPhysicsStop();
        void onPhysicsStep(float dt);

        entt::registry& registry();
        const entt::registry& registry() const;

    private:
        entt::registry m_registry;
        std::unique_ptr<b2World> m_physicsWorld;

        void createPhysicsBody(Entity entity);

        friend class Entity;
    };
}

#include "Engine/Scene/Entity.inl"