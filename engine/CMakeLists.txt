add_library(GameEngine STATIC
    src/Core/Application.cpp
    src/Core/FileSystem.cpp
    src/Core/Log.cpp
)

target_include_directories(GameEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(GameEngine
    PUBLIC
        $<$<BOOL:${ENGINE_ENABLE_ASSERTS}>:ENGINE_ENABLE_ASSERTS>
)

if (MSVC)
    target_compile_options(GameEngine PRIVATE /W4 /permissive-)
else()
    target_compile_options(GameEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()