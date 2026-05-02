#include "Engine/Renderer/VertexBuffer.hpp"

#include <glad/glad.h>

namespace Engine
{
    VertexBuffer::VertexBuffer(const void* data, std::size_t size)
    {
        glGenBuffers(1, &m_rendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(std::size_t size)
    {
        glGenBuffers(1, &m_rendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_rendererId);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void* data, std::size_t size, std::size_t offset)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
        glBufferSubData(
            GL_ARRAY_BUFFER,
            static_cast<GLintptr>(offset),
            static_cast<GLsizeiptr>(size),
            data
        );
    }

    unsigned int VertexBuffer::rendererId() const
    {
        return m_rendererId;
    }
}