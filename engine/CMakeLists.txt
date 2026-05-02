add_library(GameEngine STATIC
    src/Core/Application.cpp
    src/Core/FileSystem.cpp
    src/Core/Log.cpp
    src/Core/Timer.cpp
    src/Core/Window.cpp
    src/Core/Input.cpp

    src/Renderer/RenderCommand.cpp
    src/Renderer/Renderer2D.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture2D.cpp
    src/Renderer/OrthographicCamera.cpp
    src/Renderer/Framebuffer.cpp

    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(GameEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(GameEngine
    PUBLIC glfw
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