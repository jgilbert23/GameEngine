#pragma once

#include "Engine/Core/Log.hpp"

#include <cstdlib>

#ifdef ENGINE_ENABLE_ASSERTS

#define ENGINE_ASSERT(condition, message)                           \
    do                                                              \
    {                                                               \
        if (!(condition))                                           \
        {                                                           \
            ENGINE_CRITICAL(message);                               \
            std::abort();                                           \
        }                                                           \
    } while (false)

#else

#define ENGINE_ASSERT(condition, message) do { (void)sizeof(condition); } while (false)

#endif