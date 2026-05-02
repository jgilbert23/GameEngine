#include "Engine/Core/Timer.hpp"

namespace Engine
{
    Timer::Timer()
    {
        reset();
    }

    void Timer::reset()
    {
        m_startTime = Clock::now();
        m_lastTickTime = m_startTime;
    }

    float Timer::elapsedSeconds() const
    {
        const auto now = Clock::now();
        const auto elapsed = std::chrono::duration<float>(now - m_startTime);
        return elapsed.count();
    }

    float Timer::tick()
    {
        const auto now = Clock::now();
        const auto delta = std::chrono::duration<float>(now - m_lastTickTime);
        m_lastTickTime = now;
        return delta.count();
    }
}