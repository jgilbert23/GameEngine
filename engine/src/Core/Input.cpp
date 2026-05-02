#include "Engine/Core/Input.hpp"

#include <GLFW/glfw3.h>

namespace Engine
{
    GLFWwindow* Input::s_window = nullptr;

    void Input::setWindow(GLFWwindow* window)
    {
        s_window = window;
    }

    bool Input::isKeyPressed(int key)
    {
        if (!s_window)
        {
            return false;
        }

        const int state = glfwGetKey(s_window, key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(int button)
    {
        if (!s_window)
        {
            return false;
        }

        return glfwGetMouseButton(s_window, button) == GLFW_PRESS;
    }

    float Input::mouseX()
    {
        double x = 0.0;
        double y = 0.0;

        if (s_window)
        {
            glfwGetCursorPos(s_window, &x, &y);
        }

        return static_cast<float>(x);
    }

    float Input::mouseY()
    {
        double x = 0.0;
        double y = 0.0;

        if (s_window)
        {
            glfwGetCursorPos(s_window, &x, &y);
        }

        return static_cast<float>(y);
    }

    bool Input::isControllerPresent(int controllerId)
    {
        return glfwJoystickPresent(controllerId) == GLFW_TRUE;
    }

    const char* Input::controllerName(int controllerId)
    {
        return glfwGetJoystickName(controllerId);
    }

    float Input::controllerAxis(int controllerId, int axis)
    {
        int count = 0;
        const float* axes = glfwGetJoystickAxes(controllerId, &count);

        if (!axes || axis < 0 || axis >= count)
        {
            return 0.0f;
        }

        return axes[axis];
    }

    bool Input::controllerButtonPressed(int controllerId, int button)
    {
        int count = 0;
        const unsigned char* buttons = glfwGetJoystickButtons(controllerId, &count);

        if (!buttons || button < 0 || button >= count)
        {
            return false;
        }

        return buttons[button] == GLFW_PRESS;
    }
}