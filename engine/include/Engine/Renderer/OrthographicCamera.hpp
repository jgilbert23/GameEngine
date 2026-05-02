#pragma once

#include "Engine/Renderer/Camera.hpp"
#include <array>

namespace Engine
{
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void setPosition(float x, float y, float z);
        void setRotation(float rotationDegrees);

        const std::array<float, 16>& viewProjectionMatrix() const override;

    private:
        std::array<float, 16> m_projection{};
        std::array<float, 16> m_view{};
        std::array<float, 16> m_viewProjection{};

        float m_positionX = 0.0f;
        float m_positionY = 0.0f;
        float m_positionZ = 0.0f;
        float m_rotation = 0.0f;

        void recalculateViewMatrix();
    };
}