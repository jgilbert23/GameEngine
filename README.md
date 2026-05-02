````markdown
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
````

---

## 🔧 Dependencies

* OpenGL 3.3 Core
* GLFW (window + input)
* GLAD (OpenGL loader)
* stb_image (texture loading)
* CMake ≥ 3.24

---

## 🛠️ Build Instructions

### Linux / macOS

```bash
git clone <your-repo>
cd GameEngine

cmake -S . -B build
cmake --build build

./build/sandbox/Sandbox
```

---

### Windows (Visual Studio)

```bash
cmake -S . -B build
cmake --build build --config Release
```

Run:

```bash
build\sandbox\Release\Sandbox.exe
```

---

## 🎯 Example Usage

```cpp
class SandboxLayer : public Engine::Layer
{
public:
    void onUpdate(float dt) override
    {
        Engine::Renderer2D::beginScene(camera);

        Engine::Renderer2D::drawQuad(
            0.0f, 0.0f,
            2.0f, 2.0f,
            0.2f, 0.6f, 1.0f, 1.0f
        );

        Engine::Renderer2D::endScene();
    }

private:
    Engine::OrthographicCamera camera{-10.0f, 10.0f, -5.625f, 5.625f};
};
```

---

## 🧠 Design Goals

* Clean separation of engine systems
* Modern C++ practices (RAII, smart pointers)
* Cross-platform compatibility
* Educational clarity (minimal magic)
* Easy to extend into 3D or ECS-based systems

---

## 📈 Roadmap

* [ ] ECS (Entity Component System)
* [ ] Scene serialization (JSON/YAML)
* [ ] ImGui editor
* [ ] Camera controls + input system improvements
* [ ] Texture atlas support
* [ ] Lighting + 2D effects
* [ ] 3D renderer

---

## 🐛 Common Issues

### OpenGL errors (`glCreate* not found`)

You are using OpenGL 3.3 — replace with:

```
glGen*
glBind*
glTex*
```

---

### Segmentation fault on shutdown

Ensure:

* Renderer is shut down **before GLFW terminates**
* OpenGL objects are destroyed while context is valid

---

## 📄 License

MIT (or your preferred license)

---

## 🙌 Acknowledgments

* GLFW
* GLAD
* stb
* OpenGL community

---

## 💡 Notes

This project is designed as a **learning engine**, not a production engine (yet).
It prioritizes clarity, architecture, and incremental development.

---

## ⭐ Contributing

PRs, ideas, and improvements are welcome!

```
Just tell me 👍
```
