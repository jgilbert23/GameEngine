````markdown
# 🎮 Game Engine User Guide

Welcome to the **C++ Game Engine**.  
This guide explains how to use the engine to build simple 2D applications.

---

# 🚀 Getting Started

## Build & Run

```bash
git clone <repo>
cd GameEngine

cmake -S . -B build
cmake --build build

./build/sandbox/Sandbox
````

---

# 🧱 Basic Concepts

## 1. Application

The engine runs through an `Application` instance.

```cpp
Engine::Application app(config);
app.run();
```

---

## 2. Layers

All game logic lives inside **Layers**.

```cpp
class MyLayer : public Engine::Layer
{
public:
    void onAttach() override {}
    void onUpdate(float dt) override {}
    void onEvent(Engine::Event& event) override {}
};
```

Add it to the app:

```cpp
app.pushLayer(std::make_unique<MyLayer>());
```

---

## 3. Game Loop

You do **not** write the loop. The engine handles:

```text
pollEvents → update → render → swapBuffers
```

You only implement:

```cpp
void onUpdate(float dt)
```

---

# 🎨 Rendering

## Basic Rendering Example

```cpp
#include "Engine/Renderer/Renderer2D.hpp"
#include "Engine/Renderer/OrthographicCamera.hpp"

class GameLayer : public Engine::Layer
{
public:
    GameLayer()
        : m_camera(-10.0f, 10.0f, -5.625f, 5.625f)
    {}

    void onUpdate(float dt) override
    {
        Engine::Renderer2D::beginScene(m_camera);

        Engine::Renderer2D::drawQuad(
            0.0f, 0.0f,
            2.0f, 2.0f,
            0.2f, 0.6f, 1.0f, 1.0f
        );

        Engine::Renderer2D::endScene();
    }

private:
    Engine::OrthographicCamera m_camera;
};
```

---

## Drawing Multiple Objects

```cpp
Renderer2D::drawQuad(-2.0f, 0.0f, 1.0f, 1.0f, 1, 0, 0, 1);
Renderer2D::drawQuad( 2.0f, 0.0f, 1.0f, 1.0f, 0, 1, 0, 1);
```

---

## Textured Rendering

```cpp
#include "Engine/Renderer/Texture2D.hpp"

Engine::Texture2D texture("assets/texture.png");

Renderer2D::drawTexturedQuad(
    0.0f, 0.0f,
    2.0f, 2.0f,
    texture
);
```

---

# 🎥 Camera

## Creating a Camera

```cpp
Engine::OrthographicCamera camera(
    -10.0f, 10.0f,
    -5.625f, 5.625f
);
```

---

## Moving the Camera

```cpp
camera.setPosition(x, y, 0.0f);
```

---

## Rotating the Camera

```cpp
camera.setRotation(45.0f);
```

---

# ⌨️ Input

## Keyboard Input

```cpp
#include <GLFW/glfw3.h>

if (Engine::Input::isKeyPressed(GLFW_KEY_SPACE))
{
    // do something
}
```

---

## Mouse Input

```cpp
if (Engine::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
{
    // click logic
}

float x = Engine::Input::mouseX();
float y = Engine::Input::mouseY();
```

---

## Controller Input

```cpp
if (Engine::Input::isControllerPresent(0))
{
    float axis = Engine::Input::controllerAxis(0, 0);
}
```

---

# ⚡ Fixed Timestep (Physics)

Use `onFixedUpdate` for deterministic logic:

```cpp
void onFixedUpdate(float dt) override
{
    // physics updates here
}
```

---

# 📡 Events

## Handling Events

```cpp
void onEvent(Engine::Event& event) override
{
    if (event.type == Engine::EventType::KeyPressed)
    {
        // handle key press
    }

    if (event.type == Engine::EventType::WindowResize)
    {
        // handle resize
    }
}
```

---

# 🧵 Framebuffers (Advanced)

## Create Framebuffer

```cpp
Engine::Framebuffer fb({1280, 720});
```

## Use it

```cpp
fb.bind();

// render scene

fb.unbind();
```

---

# 🧠 Best Practices

### ✔ Always wrap rendering in a scene

```cpp
Renderer2D::beginScene(camera);
Renderer2D::endScene();
```

---

### ✔ Use delta time

```cpp
position += velocity * dt;
```

---

### ✔ Keep logic in layers

* No logic in `main()`
* No logic in `Window`

---

### ✔ Use fixed update for physics

* Avoid physics in `onUpdate`

---

# ⚠️ Common Issues

## ❌ Nothing renders

Check:

* `beginScene()` / `endScene()` pairing
* camera bounds
* clear color

---

## ❌ Texture not showing

Check:

* correct file path
* image format (RGB/RGBA)

---

## ❌ OpenGL errors

Ensure:

* GLAD initialized after context creation
* Using OpenGL 3.3 functions only

---

## ❌ Crash on exit

Ensure:

* Renderer shutdown happens before GLFW termination

---

# 🧪 Minimal Working Example

```cpp
int main()
{
    Engine::Application app;

    class DemoLayer : public Engine::Layer
    {
    public:
        DemoLayer()
            : camera(-10, 10, -5, 5)
        {}

        void onUpdate(float dt) override
        {
            Engine::Renderer2D::beginScene(camera);

            Engine::Renderer2D::drawQuad(
                0, 0, 2, 2,
                1, 0, 0, 1
            );

            Engine::Renderer2D::endScene();
        }

    private:
        Engine::OrthographicCamera camera;
    };

    app.pushLayer(std::make_unique<DemoLayer>());
    app.run();
}
```

---

# 📈 Next Steps

You can extend the engine with:

* ECS (Entity Component System)
* Scene serialization
* Editor (ImGui)
* Physics (Box2D)
* Animation system

---

# 💡 Tips

* Start small (draw quads)
* Add input
* Build a simple game (Breakout / Pong)
* Then expand engine features

---

# 🎯 Summary

With this engine you can:

* Create windows
* Handle input
* Render 2D objects
* Load textures
* Build simple games

---

Happy building 🚀
