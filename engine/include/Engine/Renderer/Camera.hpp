#pragma once

#include <array>

namespace Engine
{
    class Camera
    {
    public:
        virtual ~Camera() = default;

        virtual const std::array<float, 16>& viewProjectionMatrix() const = 0;
    };
}