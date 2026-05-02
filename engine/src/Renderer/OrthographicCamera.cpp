#include "Engine/Renderer/OrthographicCamera.hpp"

#include <cmath>

namespace Engine
{
    static std::array<float, 16> identity()
    {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    static std::array<float, 16> multiply(
        const std::array<float, 16>& a,
        const std::array<float, 16>& b
    )
    {
        std::array<float, 16> result{};

        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                result[col * 4 + row] =
                    a[0 * 4 + row] * b[col * 4 + 0] +
                    a[1 * 4 + row] * b[col * 4 + 1] +
                    a[2 * 4 + row] * b[col * 4 + 2] +
                    a[3 * 4 + row] * b[col * 4 + 3];
            }
        }

        return result;
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    {
        m_projection = identity();

        m_projection[0] = 2.0f / (right - left);
        m_projection[5] = 2.0f / (top - bottom);
        m_projection[10] = -1.0f;
        m_projection[12] = -(right + left) / (right - left);
        m_projection[13] = -(top + bottom) / (top - bottom);

        recalculateViewMatrix();
    }

    void OrthographicCamera::setPosition(float x, float y, float z)
    {
        m_positionX = x;
        m_positionY = y;
        m_positionZ = z;
        recalculateViewMatrix();
    }

    void OrthographicCamera::setRotation(float rotationDegrees)
    {
        m_rotation = rotationDegrees;
        recalculateViewMatrix();
    }

    const std::array<float, 16>& OrthographicCamera::viewProjectionMatrix() const
    {
        return m_viewProjection;
    }

    void OrthographicCamera::recalculateViewMatrix()
    {
        const float radians = m_rotation * 3.14159265359f / 180.0f;
        const float c = std::cos(radians);
        const float s = std::sin(radians);

        m_view = identity();

        m_view[0] = c;
        m_view[1] = -s;
        m_view[4] = s;
        m_view[5] = c;

        m_view[12] = -(c * m_positionX + s * m_positionY);
        m_view[13] = -(-s * m_positionX + c * m_positionY);
        m_view[14] = -m_positionZ;

        m_viewProjection = multiply(m_projection, m_view);
    }
}