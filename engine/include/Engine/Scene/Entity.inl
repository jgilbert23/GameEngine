#pragma once

#include <utility>

namespace Engine
{
    template <typename T, typename... Args>
    T& Entity::addComponent(Args&&... args)
    {
        return m_scene->m_registry.emplace<T>(
            m_handle,
            std::forward<Args>(args)...
        );
    }

    template <typename T>
    T& Entity::getComponent()
    {
        return m_scene->m_registry.get<T>(m_handle);
    }

    template <typename T>
    const T& Entity::getComponent() const
    {
        return m_scene->m_registry.get<T>(m_handle);
    }

    template <typename T>
    bool Entity::hasComponent() const
    {
        return m_scene->m_registry.all_of<T>(m_handle);
    }

    template <typename T>
    void Entity::removeComponent()
    {
        m_scene->m_registry.remove<T>(m_handle);
    }
}