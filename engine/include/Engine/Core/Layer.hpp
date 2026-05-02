#pragma once

#include "Engine/Core/Event.hpp"

#include <string>
#include <utility>

namespace Engine
{
    class Layer
    {
    public:
        explicit Layer(std::string name = "Layer")
            : m_name(std::move(name))
        {
        }

        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual void onDetach() {}

        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
        virtual void onFixedUpdate(float fixedDeltaTime) {}
        virtual void onEvent(Event& event) {}

        const std::string& name() const
        {
            return m_name;
        }

    private:
        std::string m_name;
    };
}