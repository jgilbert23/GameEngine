#pragma once

#include <cstddef>

namespace Engine
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const unsigned int* indices, std::size_t count);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        void bind() const;
        void unbind() const;

        std::size_t count() const;
        unsigned int rendererId() const;

    private:
        unsigned int m_rendererId = 0;
        std::size_t m_count = 0;
    };
}