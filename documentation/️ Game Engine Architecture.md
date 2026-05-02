````markdown
# 🏗️ Game Engine Architecture (C++20 + OpenGL 3.3)

## Overview

This project is a modular 2D game engine built using modern C++ and OpenGL.  
The architecture emphasizes:

- Clear system boundaries
- RAII-based resource management
- Minimal abstraction overhead
- Incremental extensibility (toward ECS, editor, 3D)

---

## 🔄 High-Level Flow

```text
main()
  ↓
Application
  ↓
run()  ← CORE LOOP
  ├── pollEvents()          (Window / GLFW)
  ├── onEvent()             (Application → Layers)
  ├── fixedUpdate()         (Deterministic systems)
  ├── update(dt)            (Game logic)
  ├── render()              (Renderer2D)
  └── swapBuffers()         (Window)
````

---

## 🧱 Core Systems

### 1. Application

**Responsibility:**

* Owns engine lifecycle
* Runs the main loop
* Dispatches updates and events
* Manages layers

**Key Components:**

* `run()`
* `onEvent()`
* `pushLayer()`

---

### 2. Window (Platform Layer)

**Responsibility:**

* Creates OpenGL context (GLFW)
* Handles OS events
* Manages swap buffers and vsync

**Important:**

* Initializes GLAD
* Owns the OpenGL context
* Must outlive all renderer resources

---

### 3. Layer System

**Purpose:**

* Decouple engine systems and gameplay logic

**Lifecycle:**

```text
onAttach()   → initialization
onUpdate()   → per-frame logic
onFixedUpdate() → deterministic updates
onEvent()    → input/events
onDetach()   → cleanup
```

**Execution Order:**

* Updates: front → back
* Events: back → front (top-most first)

---

### 4. Event System

**Design:**

* Lightweight struct-based events
* Type-based dispatch

**Flow:**

```text
GLFW → Window → Application → Layers
```

**Types:**

* Window events (resize, close)
* Input events (keyboard, mouse)

---

### 5. Input System

**Design:**

* Static interface (`Input`)
* Pull-based querying (e.g., `isKeyPressed()`)

**Backed by:**

* GLFW input APIs

---

### 6. Timing System

**Components:**

* `Timer` → delta time
* Fixed timestep accumulator

**Loop Integration:**

```cpp
while (running)
{
    float dt = timer.tick();

    accumulator += dt;

    while (accumulator >= fixedDelta)
        fixedUpdate();

    update(dt);
}
```

---

## 🎨 Rendering Architecture

### Overview

```text
Application
  ↓
Renderer2D (high-level API)
  ↓
RenderCommand (OpenGL abstraction)
  ↓
OpenGL (GLAD)
```

---

### 1. RenderCommand

**Purpose:**

* Thin abstraction over OpenGL calls

**Examples:**

* `setClearColor()`
* `clear()`
* `setViewport()`

---

### 2. Renderer2D

**Responsibility:**

* High-level 2D rendering API
* Batching
* Texture slot management
* Scene submission

**Core Methods:**

* `beginScene(camera)`
* `drawQuad(...)`
* `endScene()`

---

### 3. Batching System

**Goal:**
Minimize draw calls by grouping quads into one buffer.

**Process:**

1. Accumulate vertex data in CPU buffer
2. Upload once per frame
3. Draw in a single `glDrawElements`

**Limits:**

* Max quads per batch
* Max texture slots (16)

---

### 4. Shader System

**Responsibilities:**

* Compile/link shaders
* Bind/unbind programs
* Set uniforms

**Key Constraints:**

* GLSL 330 compatibility
* Texture arrays require `switch` (no dynamic indexing)

---

### 5. Texture System

**Features:**

* Image loading via `stb_image`
* GPU upload
* Slot-based binding

**OpenGL Model:**

```cpp
glActiveTexture(GL_TEXTURE0 + slot);
glBindTexture(GL_TEXTURE_2D, id);
```

---

### 6. Buffer Abstractions

#### VertexBuffer

* Wraps `GL_ARRAY_BUFFER`
* Supports dynamic updates

#### IndexBuffer

* Wraps `GL_ELEMENT_ARRAY_BUFFER`

#### VertexArray

* Owns attribute layout
* Binds VBO + IBO

---

### 7. Camera System

**Current:**

* Orthographic camera

**Provides:**

* View matrix
* Projection matrix
* View-projection matrix

---

### 8. SpriteRenderer

**Purpose:**

* High-level API for game code
* Wraps `Renderer2D`

---

### 9. Framebuffer

**Purpose:**

* Off-screen rendering
* Enables post-processing

**Attachments:**

* Color (RGBA8)
* Depth/stencil

---

## 🧠 Memory & Lifetime Model

### Key Rule

> All OpenGL resources must be destroyed **before** the context is destroyed.

### Correct Destruction Order

```text
Renderer2D::shutdown()
  → destroy shaders
  → destroy textures
  → destroy buffers

Window::~Window()
  → glfwTerminate()
```

---

## 🔌 External Dependencies

| Library | Purpose        |
| ------- | -------------- |
| GLFW    | Window + input |
| GLAD    | OpenGL loader  |
| stb     | Image loading  |

---

## ⚙️ OpenGL Version Strategy

The engine targets:

```text
OpenGL 3.3 Core
```

### Implications

* ❌ No Direct State Access (DSA)
* ❌ No `glCreate*` APIs
* ✅ Must use bind-based workflow

---

## 📦 Data Flow Example

```text
Game Layer
  ↓
SpriteRenderer::drawSprite()
  ↓
Renderer2D::drawQuad()
  ↓
Batch buffer (CPU)
  ↓
VertexBuffer::setData()
  ↓
glDrawElements()
```

---

## 🧩 Extensibility Points

Future systems can plug into:

* ECS (Entity Component System)
* Scene graph
* Editor (ImGui)
* 3D renderer
* Physics engine

---

## 📈 Future Architecture Evolution

* Replace Layer system with ECS
* Introduce Render Graph
* Add material system
* GPU instancing
* Deferred rendering

---

## ⚠️ Known Constraints

* GLSL sampler array indexing limitations
* Fixed max texture slots (16)
* No multithreading yet
* No resource manager (yet)

---

## 🎯 Summary

This architecture provides:

* Clean separation of concerns
* Modern C++ memory safety
* Efficient 2D rendering via batching
* Solid foundation for expansion

---

## 💡 Design Philosophy

> Start simple, make it correct, then make it fast, then make it scalable.

```