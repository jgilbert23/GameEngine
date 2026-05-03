#pragma once

#include "Engine/Renderer/Texture2D.hpp"

#include <memory>
#include <string>

namespace Engine
{
    struct TransformComponent
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        float rotation = 0.0f;

        float scaleX = 1.0f;
        float scaleY = 1.0f;
    };

    struct SpriteComponent
    {
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;

        std::shared_ptr<Texture2D> texture = nullptr;
    };

    struct CameraComponent
    {
        bool primary = false;

        float left = 0.0f;
        float right = 16.0f;
        float bottom = 0.0f;
        float top = 9.0f;
    };

    struct RigidBodyComponent
    {
        float velocityX = 0.0f;
        float velocityY = 0.0f;

        float accelerationX = 0.0f;
        float accelerationY = 0.0f;

        bool fixedRotation = true;
    };

    struct NameComponent
    {
        std::string name = "Entity";
    };
}