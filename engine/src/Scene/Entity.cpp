#include "Engine/Scene/Entity.hpp"
#include "Engine/Scene/Scene.hpp"

namespace Engine
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_handle(handle), m_scene(scene)
    {
    }

    bool Entity::valid() const
    {
        return m_scene && m_scene->m_registry.valid(m_handle);
    }

    entt::entity Entity::handle() const
    {
        return m_handle;
    }

    Entity::operator bool() const
    {
        return valid();
    }
}