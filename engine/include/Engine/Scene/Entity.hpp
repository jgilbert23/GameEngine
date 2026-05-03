#pragma once

#include <entt/entt.hpp>


namespace Engine
{
    class Scene;

    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);

        template <typename T, typename... Args>
        T& addComponent(Args&&... args);

        template <typename T>
        T& getComponent();

        template <typename T>
        const T& getComponent() const;

        template <typename T>
        bool hasComponent() const;

        template <typename T>
        void removeComponent();

        bool valid() const;
        entt::entity handle() const;

        explicit operator bool() const;

    private:
        entt::entity m_handle = entt::null;
        Scene* m_scene = nullptr;
    };
}