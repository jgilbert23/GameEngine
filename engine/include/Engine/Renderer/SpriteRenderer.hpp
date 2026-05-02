#pragma once

#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Texture2D.hpp"

namespace Engine
{
    class SpriteRenderer
    {
    public:
        static void beginScene(const OrthographicCamera& camera);
        static void endScene();

        static void drawSprite(
            float x,
            float y,
            float width,
            float height,
            float r,
            float g,
            float b,
            float a
        );

        static void drawSprite(
            float x,
            float y,
            float width,
            float height,
            const Texture2D& texture
        );
    };
}