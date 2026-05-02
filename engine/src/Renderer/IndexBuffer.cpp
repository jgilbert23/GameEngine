#include "Engine/Renderer/IndexBuffer.hpp"

#include <glad/glad.h>

namespace Engine
{
    IndexBuffer::IndexBuffer(const unsigned int* indices, std::size_t count)
        : m_count(count)
    {
        glGenBuffers(1, &m_rendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(count * sizeof(unsigned int)),
            indices,
            GL_STATIC_DRAW
        );
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererId);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    std::size_t IndexBuffer::count() const
    {
        return m_count;
    }

    unsigned int IndexBuffer::rendererId() const
    {
        return m_rendererId;
    }
}