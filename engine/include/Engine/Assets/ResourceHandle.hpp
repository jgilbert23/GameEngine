#pragma once

#include <memory>

namespace Engine
{
    template <typename T>
    class ResourceHandle
    {
    public:
        ResourceHandle() = default;

        explicit ResourceHandle(std::shared_ptr<T> resource)
            : m_resource(std::move(resource))
        {
        }

        T* operator->()
        {
            return m_resource.get();
        }

        const T* operator->() const
        {
            return m_resource.get();
        }

        T& operator*()
        {
            return *m_resource;
        }

        const T& operator*() const
        {
            return *m_resource;
        }

        bool valid() const
        {
            return static_cast<bool>(m_resource);
        }

        std::shared_ptr<T> shared() const
        {
            return m_resource;
        }

    private:
        std::shared_ptr<T> m_resource;
    };
}