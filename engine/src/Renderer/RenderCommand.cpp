#include "Engine/Renderer/RenderCommand.hpp"

#include <glad/glad.h>

namespace Engine
{
    void RenderCommand::init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void RenderCommand::setViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }

    void RenderCommand::setClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void RenderCommand::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::enableDepthTest(bool enabled)
    {
        if (enabled)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void RenderCommand::enableBlending(bool enabled)
    {
        if (enabled)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }
}