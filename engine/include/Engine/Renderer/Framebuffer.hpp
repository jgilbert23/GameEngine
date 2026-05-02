#pragma once

namespace Engine
{
    struct FramebufferConfig
    {
        unsigned int width = 1280;
        unsigned int height = 720;
    };

    class Framebuffer
    {
    public:
        explicit Framebuffer(const FramebufferConfig& config);
        ~Framebuffer();

        void bind();
        void unbind();

        void resize(unsigned int width, unsigned int height);

        unsigned int colorAttachment() const;

    private:
        unsigned int m_rendererId = 0;
        unsigned int m_colorAttachment = 0;
        unsigned int m_depthAttachment = 0;

        FramebufferConfig m_config;

        void recreate();
    };
}