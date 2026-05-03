#pragma once

#include <entt/entt.hpp>

#include "Engine/Scene/Entity.hpp"


namespace Engine
{
    class OrthographicCamera;

    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        Entity createEntity(const char* name = "Entity");
        void destroyEntity(Entity entity);

        void onUpdate(float dt);
        void onRender();

        void clear();
        
        entt::registry& registry();
        const entt::registry& registry() const;

    private:
        entt::registry m_registry;

        friend class Entity;
    };
}

#include "Engine/Scene/Entity.inl"