# Trinacria Diesel

Trinacria Diesel (Trinacria::DSL) is a simple batch renderer made for flexibility and simple use. It uses OpenGL for its graphic API and glad for acces to the driver functions. 

## Stack

- **Language:** C++

- **Build System**: [CMake](https://cmake.org/)

- **Window library used in demos:** [glfw](https://github.com/glfw/glfw)

- **OpenGL loader:** [glad](https://github.com/Dav1dde/glad/tree/master)

- **Math Library:** [glm](https://github.com/g-truc/glm)

- **documentation library:** [doxygen](https://github.com/doxygen/doxygen)

- **text library:** [msdf-atlas-gen](https://github.com/Chlumsky/msdf-atlas-gen)

## Theory

Trinacria Diesel is a system standing using 2 major static classes:

- **Renderer**: is a static class where you can add quads to a buffer using`CreateQuad` or add triangles using `CreateTriangle` that then you can `Draw` at the end of the frame example of other functions are `FlushBuffers`, `ClearColorBuffer`, `EndScene`, `Init`  [...]

- **LightSystem:** is a static class used for giving light to the things you draw using `SetupLight` that asks for data structs containing all the information a light could have. `Done` is a function that puts all the data given to the system into a texture that then gets passed via uniform to the shaders (reccomendation is to use the shader given to the demo). `Init` initializes the light system.

- **Other useful classes:** `Material`, `Sprite`, `Animation`, `Shader`, `Texture` [...]

## Build

```bash
git clone https://github.com/ZioCessoProgramma/Trinacria-2D-Renderer.git --recursive
cd Trinacria-2D-Renderer
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -B build
cd build
ninja
```

## Run the demo

```bash
cd ../Test1
./../build/bin/Test1
```

## Usage

How to use at its best the library

### Layer System:

In the headers of the library there's a `Layer.h` file that is an abstract class containing the 3 functions caraterizing a layer: `OnAttach` for when the layer is created, `OnDetach` for when it is destroyed, `OnUpdate` that runs every frame. Using the layer system is reccomended but not necessary.

### The Renderer

To make the renderer work the user sould call `Init` in the `OnAttach` function of the layer and should load the shaders by going into the renderer shader program variable 

(`Trinacria::DSL::Renderer::ShaderProgram`). After adding all the quads or triangles of the scene the user sould call `EndScene` then `Draw` and then `FlushBuffers`. To get resizing to work the user sould call `OnResize` every time the window resizes. On the closing of the game/app it's better to call `Cleanup` to assure that all the opengl components are destroyed.

### The LightSystem

Unlike the renderer the `Init` function should be called every frame before adding the lights. When the user's done he sould call the `Done` function.

### Material

To make every thing to draw correctly the user should call `Trinacria::DSL::Renderer::AddMaterial(Trinacria::DSL::SOLID_OPAQUE);`

to create your own material or to understand more deeply how the Material system works in Trinacria Diesel check the incoming documentation.

### Event System

check the incoming documentation.

### Example of a RendererLayer using Trinacria Diesel

```c++
void RendererLayer::OnAttach()
{
    // Texture loading
    Texture tex;
    tex.LoadTexture("path/to/texture");

    // Getting the renderer to work
    // The init function does not initialize the window
    Trinacria::DSL::Renderer::Init(glm::vec2(800, 600), window);

    // Have to use LoadCoreShader for ShaderProgram

    Trinacria::DSL::Renderer::ShaderProgram.LoadCoreShader(
    "path/to/vertex.vert", "path/to/fragment.frag");

    screenShader.LoadShader("path/to/vertex.vert",
     "path/to/fragment.frag");
}

void RendererLayer::OnUpdate()
{
    Trinacria::DSL::Renderer::ClearColorBuffer(glm::vec3(0.1f, 0.1f, 0.1f));

    // You can change size, rotation, etc...
    Trinacria::DSL::Transform transform(glm::vec2(1.f, 3.f));

    // You can put textures, sprites, etc...
    Trinacria::DSL::QuadData data(transform);

    Trinacria::DSL::CreateQuad(data);

    Trinacria::DSL::Renderer::EndScene();

    Trinacria::DSL::LightSystem::Init(
        Trinacria::DSL::LightSystem::DEFAULT_USE_OF_LIGHT
    );

    Trinacria::DSL::PointLightData pointLight(glm::vec3(1.f, 1.f, 1.f),
        glm::vec2(6.f, 7.f);

    Trinacria::DSL::LightSystem::Done();

    Trinacria::DSL::Renderer::Draw(screenShader);

    Trinacria::DSL::Renderer::FlushBuffers();
}

void RendererLayer::OnDetach()
{
    Trinacria::DSL::Renderer::Cleanup();
}
```
