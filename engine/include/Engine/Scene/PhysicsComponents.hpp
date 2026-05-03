#pragma once

#include <box2d/box2d.h>

namespace Engine
{
    enum class BodyType2D
    {
        Static,
        Dynamic,
        Kinematic
    };

    struct RigidBody2DComponent
    {
        BodyType2D type = BodyType2D::Dynamic;
        bool fixedRotation = true;

        b2Body* runtimeBody = nullptr;
    };

    struct BoxCollider2DComponent
    {
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        float sizeX = 0.5f;
        float sizeY = 0.5f;

        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.5f;
    };

    struct CircleCollider2DComponent
    {
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        float radius = 0.5f;

        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.5f;
    };
}