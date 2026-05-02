#pragma once

struct GLFWwindow;

namespace Engine
{
    class Input
    {
    public:
        static void setWindow(GLFWwindow* window);

        static bool isKeyPressed(int key);
        static bool isMouseButtonPressed(int button);

        static float mouseX();
        static float mouseY();

        static bool isControllerPresent(int controllerId);
        static const char* controllerName(int controllerId);
        static float controllerAxis(int controllerId, int axis);
        static bool controllerButtonPressed(int controllerId, int button);

    private:
        static GLFWwindow* s_window;
    };
}