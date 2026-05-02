#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Core/Assert.hpp"

#include <glad/glad.h>

namespace Engine
{
    Framebuffer::Framebuffer(const FramebufferConfig& config)
        : m_config(config)
    {
        recreate();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_rendererId);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }

    void Framebuffer::recreate()
    {
        if (m_rendererId)
        {
            glDeleteFramebuffers(1, &m_rendererId);
            glDeleteTextures(1, &m_colorAttachment);
            glDeleteTextures(1, &m_depthAttachment);
        }

        glGenFramebuffers(1, &m_rendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

        // ---- COLOR ATTACHMENT ----
        glGenTextures(1, &m_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachment);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            m_config.width,
            m_config.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_colorAttachment,
            0
        );

        // ---- DEPTH/STENCIL ATTACHMENT ----
        glGenTextures(1, &m_depthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachment);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH24_STENCIL8,
            m_config.width,
            m_config.height,
            0,
            GL_DEPTH_STENCIL,
            GL_UNSIGNED_INT_24_8,
            nullptr
        );

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D,
            m_depthAttachment,
            0
        );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_config.width, m_config.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

        glGenTextures(1, &m_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_config.width, m_config.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                      "Framebuffer is incomplete");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
        glViewport(0, 0, m_config.width, m_config.height);
    }

    void Framebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::resize(unsigned int width, unsigned int height)
    {
        if (width == 0 || height == 0)
            return;

        m_config.width = width;
        m_config.height = height;

        recreate();
    }

    unsigned int Framebuffer::colorAttachment() const
    {
        return m_colorAttachment;
    }
}