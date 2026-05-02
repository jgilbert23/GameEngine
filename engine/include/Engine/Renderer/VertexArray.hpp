#pragma once

#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"

#include <memory>

namespace Engine
{
    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4
    };

    struct VertexAttribute
    {
        unsigned int location = 0;
        ShaderDataType type = ShaderDataType::Float;
        bool normalized = false;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void bind() const;
        void unbind() const;

        void addVertexBuffer(
            std::shared_ptr<VertexBuffer> vertexBuffer,
            const VertexAttribute* attributes,
            std::size_t attributeCount,
            std::size_t stride
        );

        void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

        const std::shared_ptr<IndexBuffer>& indexBuffer() const;

    private:
        unsigned int m_rendererId = 0;
        std::shared_ptr<IndexBuffer> m_indexBuffer;

        static unsigned int componentCount(ShaderDataType type);
    };
}