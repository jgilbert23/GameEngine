#include "Engine/Core/Window.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Core/Log.hpp"

#include <GLFW/glfw3.h>

namespace Engine
{
    int Window::s_windowCount = 0;

    Window::Window(const WindowConfig& config)
        : m_config(config)
    {
        if (s_windowCount == 0)
        {
            ENGINE_ASSERT(glfwInit(), "Failed to initialize GLFW");
            ENGINE_INFO("GLFW initialized");
        }

        m_window = glfwCreateWindow(
            m_config.width,
            m_config.height,
            m_config.title.c_str(),
            nullptr,
            nullptr
        );

        ENGINE_ASSERT(m_window != nullptr, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);
        ENGINE_ASSERT(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
              "Failed to initialize GLAD");
        glfwSwapInterval(m_config.vSync ? 1 : 0);

        glfwSetWindowUserPointer(m_window, this);
        Input::setWindow(m_window);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            engineWindow->m_config.width = width;
            engineWindow->m_config.height = height;

            Event event;
            event.type = EventType::WindowResize;
            event.width = width;
            event.height = height;

            if (engineWindow->m_eventCallback)
            {
                engineWindow->m_eventCallback(event);
            }
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            auto* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            Event event;
            event.type = EventType::WindowClose;

            if (engineWindow->m_eventCallback)
            {
                engineWindow->m_eventCallback(event);
            }
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int, int action, int)
        {
            auto* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            Event event;

            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                event.type = EventType::KeyPressed;
            }
            else if (action == GLFW_RELEASE)
            {
                event.type = EventType::KeyReleased;
            }
            else
            {
                return;
            }

            event.key = key;

            if (engineWindow->m_eventCallback)
            {
                engineWindow->m_eventCallback(event);
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int)
        {
            auto* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            Event event;

            if (action == GLFW_PRESS)
            {
                event.type = EventType::MouseButtonPressed;
            }
            else if (action == GLFW_RELEASE)
            {
                event.type = EventType::MouseButtonReleased;
            }
            else
            {
                return;
            }

            event.button = button;

            if (engineWindow->m_eventCallback)
            {
                engineWindow->m_eventCallback(event);
            }
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
        {
            auto* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            Event event;
            event.type = EventType::MouseMoved;
            event.x = static_cast<float>(x);
            event.y = static_cast<float>(y);

            if (engineWindow->m_eventCallback)
            {
                engineWindow->m_eventCallback(event);
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            auto* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            Event event;
            event.type = EventType::MouseScrolled;
            event.xOffset = static_cast<float>(xOffset);
            event.yOffset = static_cast<float>(yOffset);

            if (engineWindow->m_eventCallback)
            {
                engineWindow->m_eventCallback(event);
            }
        });

        s_windowCount++;

        ENGINE_INFO("Window created");
    }

    Window::~Window()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
            s_windowCount--;
        }

        if (s_windowCount == 0)
        {
            glfwTerminate();
            ENGINE_INFO("GLFW terminated");
        }
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::setEventCallback(EventCallback callback)
    {
        m_eventCallback = std::move(callback);
    }

    GLFWwindow* Window::nativeHandle() const
    {
        return m_window;
    }

    int Window::width() const
    {
        return m_config.width;
    }

    int Window::height() const
    {
        return m_config.height;
    }
}