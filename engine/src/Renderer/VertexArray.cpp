#include "Engine/Renderer/VertexArray.hpp"

#include <glad/glad.h>

namespace Engine
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_rendererId);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_rendererId);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_rendererId);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(
        std::shared_ptr<VertexBuffer> vertexBuffer,
        const VertexAttribute* attributes,
        std::size_t attributeCount,
        std::size_t stride
    )
    {
        bind();
        vertexBuffer->bind();

        std::size_t offset = 0;

        for (std::size_t i = 0; i < attributeCount; ++i)
        {
            const auto& attribute = attributes[i];

            glEnableVertexAttribArray(attribute.location);
            glVertexAttribPointer(
                attribute.location,
                static_cast<int>(componentCount(attribute.type)),
                GL_FLOAT,
                attribute.normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(stride),
                reinterpret_cast<const void*>(offset)
            );

            offset += componentCount(attribute.type) * sizeof(float);
        }
    }

    void VertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
    {
        bind();
        indexBuffer->bind();
        m_indexBuffer = std::move(indexBuffer);
    }

    const std::shared_ptr<IndexBuffer>& VertexArray::indexBuffer() const
    {
        return m_indexBuffer;
    }

    unsigned int VertexArray::componentCount(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
        }

        return 0;
    }
}