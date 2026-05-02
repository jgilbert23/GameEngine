#include "Engine/Renderer/SpriteRenderer.hpp"
#include "Engine/Renderer/Renderer2D.hpp"

namespace Engine
{
    void SpriteRenderer::beginScene(const OrthographicCamera& camera)
    {
        Renderer2D::beginScene(camera);
    }

    void SpriteRenderer::endScene()
    {
        Renderer2D::endScene();
    }

    void SpriteRenderer::drawSprite(
        float x,
        float y,
        float width,
        float height,
        float r,
        float g,
        float b,
        float a
    )
    {
        Renderer2D::drawQuad(x, y, width, height, r, g, b, a);
    }

    void SpriteRenderer::drawSprite(
        float x,
        float y,
        float width,
        float height,
        const Texture2D& texture
    )
    {
        Renderer2D::drawTexturedQuad(x, y, width, height, texture);
    }
}