#pragma once

#include <iostream>
#include <source_location>
#include <string_view>

namespace Engine
{
    enum class LogLevel
    {
        Trace,
        Info,
        Warn,
        Error,
        Critical
    };

    class Log
    {
    public:
        static void message(
            LogLevel level,
            std::string_view text,
            const std::source_location& location = std::source_location::current()
        );

    private:
        static const char* toString(LogLevel level);
    };
}

#define ENGINE_TRACE(msg)    ::Engine::Log::message(::Engine::LogLevel::Trace, msg)
#define ENGINE_INFO(msg)     ::Engine::Log::message(::Engine::LogLevel::Info, msg)
#define ENGINE_WARN(msg)     ::Engine::Log::message(::Engine::LogLevel::Warn, msg)
#define ENGINE_ERROR(msg)    ::Engine::Log::message(::Engine::LogLevel::Error, msg)
#define ENGINE_CRITICAL(msg) ::Engine::Log::message(::Engine::LogLevel::Critical, msg)