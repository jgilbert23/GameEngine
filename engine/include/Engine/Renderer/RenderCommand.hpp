#pragma once

namespace Engine
{
    class RenderCommand
    {
    public:
        static void init();

        static void setViewport(int x, int y, int width, int height);
        static void setClearColor(float r, float g, float b, float a);
        static void clear();

        static void enableDepthTest(bool enabled);
        static void enableBlending(bool enabled);
    };
}