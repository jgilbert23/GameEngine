#pragma once

#include <chrono>

namespace Engine
{
    class Timer
    {
    public:
        Timer();

        void reset();

        float elapsedSeconds() const;
        float tick();

    private:
        using Clock = std::chrono::high_resolution_clock;

        Clock::time_point m_startTime;
        Clock::time_point m_lastTickTime;
    };
}