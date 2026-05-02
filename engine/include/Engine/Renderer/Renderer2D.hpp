#pragma once

#include "Engine/Renderer/OrthographicCamera.hpp"
#include "Engine/Renderer/Texture2D.hpp"

#include <array>
#include <memory>

namespace Engine
{
    class Renderer2D
    {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const OrthographicCamera& camera);
        static void endScene();

        static void drawQuad(
            float x,
            float y,
            float width,
            float height,
            float r,
            float g,
            float b,
            float a
        );

        static void drawTexturedQuad(
            float x,
            float y,
            float width,
            float height,
            const Texture2D& texture
        );

    private:
        static void flush();
        static void startBatch();
        static void nextBatch();
    };
}