#pragma once

#include "Engine/Audio/Sound.hpp"

#include <memory>

namespace Engine
{
    struct AudioSourceComponent
    {
        std::shared_ptr<Sound> sound = nullptr;

        float volume = 1.0f;
        bool playOnStart = false;
        bool spatial = false;

        float minDistance = 1.0f;
        float maxDistance = 20.0f;
    };
}