#pragma once

#include <cstddef>

namespace Engine
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, std::size_t size);
        explicit VertexBuffer(std::size_t size);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        void bind() const;
        void unbind() const;
        void setData(const void* data, std::size_t size, std::size_t offset = 0);

        unsigned int rendererId() const;

    private:
        unsigned int m_rendererId = 0;
    };
}