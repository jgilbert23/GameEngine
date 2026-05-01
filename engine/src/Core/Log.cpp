#include "Engine/Core/Log.hpp"

namespace Engine
{
    void Log::message(
        LogLevel level,
        std::string_view text,
        const std::source_location& location
    )
    {
        std::cout
            << "[" << toString(level) << "] "
            << location.file_name() << ":"
            << location.line() << " - "
            << text
            << '\n';
    }

    const char* Log::toString(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Trace:    return "TRACE";
            case LogLevel::Info:     return "INFO";
            case LogLevel::Warn:     return "WARN";
            case LogLevel::Error:    return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
        }

        return "UNKNOWN";
    }
}