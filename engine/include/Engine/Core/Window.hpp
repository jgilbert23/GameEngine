#pragma once

#include "Engine/Core/Event.hpp"

#include <functional>
#include <string>
#include <glad/glad.h>

struct GLFWwindow;

namespace Engine
{
    struct WindowConfig
    {
        std::string title = "Game Engine";
        int width = 1280;
        int height = 720;
        bool vSync = true;
    };

    class Window
    {
    public:
        using EventCallback = std::function<void(Event&)>;

        explicit Window(const WindowConfig& config);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void pollEvents();
        void swapBuffers();

        bool shouldClose() const;

        void setEventCallback(EventCallback callback);

        GLFWwindow* nativeHandle() const;

        int width() const;
        int height() const;

    private:
        GLFWwindow* m_window = nullptr;
        WindowConfig m_config;
        EventCallback m_eventCallback;

        static int s_windowCount;
    };
}