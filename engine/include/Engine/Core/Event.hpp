#pragma once

namespace Engine
{
    enum class EventType
    {
        None,
        WindowClose,
        WindowResize,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    struct Event
    {
        EventType type = EventType::None;
        bool handled = false;

        int key = 0;
        int button = 0;
        int width = 0;
        int height = 0;

        float x = 0.0f;
        float y = 0.0f;
        float xOffset = 0.0f;
        float yOffset = 0.0f;
    };
}