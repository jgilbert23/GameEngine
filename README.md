# 🎮 C++ Game Engine (Modern C++ + OpenGL)

A lightweight 2D game engine built with **modern C++ (C++20)**, **OpenGL 3.3**, and a modular architecture designed for learning and extensibility.

---

## 🚀 Features

### Core Engine
- Application + game loop
- Layer system
- Event system (window, keyboard, mouse)
- Input handling
- Fixed timestep + delta time

### Rendering (OpenGL 3.3)
- Shader abstraction
- Texture loading (stb_image)
- 2D Renderer with batching
- Orthographic camera
- Framebuffer support

### Graphics Architecture
- VertexBuffer / IndexBuffer / VertexArray abstractions
- SpriteRenderer API
- Batched quad rendering
- Multi-texture support (up to 16 per batch)

---

## 🧱 Project Structure

```text
GameEngine/
├── engine/              # Core engine code
│   ├── Core/            # Application, Window, Input, Events
│   └── Renderer/        # OpenGL rendering system
│
├── sandbox/             # Example app using the engine
│   └── main.cpp
│
├── external/            # Third-party dependencies
│   ├── glfw/
│   ├── glad/
│   └── stb/
│
├── CMakeLists.txt
└── README.md
