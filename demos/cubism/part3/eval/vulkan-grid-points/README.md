# Vulkan 3D Cube Demo

A Vulkan-based C++ program that renders a 3D cube with interactive rotation and zoom controls. The architecture is designed to be easily extensible for individual vertex animations.

## Features

- **Mesh-based 3D cube rendering** using Vulkan API
- **Interactive rotation** - Rotate the cube using keyboard controls
- **Interactive zoom** - Adjust camera distance for zoom in/out
- **Colored faces** - Front face in red, back face in green
- **Extensible architecture** - Designed for easy addition of per-vertex animations

## Requirements

### System Dependencies

- **Vulkan SDK** - Download from [LunarG](https://vulkan.lunarg.com/)
- **GLFW3** - For window and input handling
- **GLM** - OpenGL Mathematics library
- **CMake** - Version 3.10 or higher
- **C++ Compiler** - supporting C++17 or higher

### Installing Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install vulkan-sdk libglfw3-dev libglm-dev cmake build-essential
```

#### macOS (using Homebrew)
```bash
brew install vulkan-sdk glfw glm cmake
```

#### Windows
- Install [Vulkan SDK](https://vulkan.lunarg.com/)
- Install [GLFW](https://www.glfw.org/download.html)
- Install [GLM](https://github.com/g-truc/glm/releases)

## Building

```bash
cd demos/cubism/part3
mkdir build && cd build
cmake ..
make
```

## Running

```bash
./VulkanCube
```

The application will open an 800x600 window displaying a rotating 3D cube.

## Controls

| Key | Action |
|-----|--------|
| **A** | Rotate cube left (counter-clockwise on Y-axis) |
| **D** | Rotate cube right (clockwise on Y-axis) |
| **W** | Zoom in (move camera closer) |
| **S** | Zoom out (move camera farther) |
| **ESC** | Exit application |

## Architecture

The program is structured for extensibility:

### Vertex Data
The cube is defined by 8 vertices with position and color attributes:
```cpp
const std::vector<Vertex> vertices = {
    // Front face (red)
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
    // ... more vertices
};
```

### Adding Individual Point Animation

To animate individual vertices:

1. **Modify vertex positions dynamically**: Update the vertex buffer data in the render loop
2. **Add custom vertex attributes**: Extend the `Vertex` structure with animation parameters
3. **Update shaders**: Modify `shader.vert` to process animation data

Example for per-vertex animation:
```cpp
// In the main loop or update function
void animateVertices() {
    // Create a staging buffer with modified vertex data
    std::vector<Vertex> animatedVertices = vertices;
    
    // Modify specific vertices
    for (size_t i = 0; i < animatedVertices.size(); i++) {
        float offset = std::sin(time + i * 0.5f) * 0.1f;
        animatedVertices[i].pos.y += offset;
    }
    
    // Update the vertex buffer with new data
    // (requires recreating staging buffer and copying to GPU)
}
```

### Transformation Pipeline

The rendering uses a Model-View-Projection (MVP) matrix system:

- **Model Matrix**: Handles object rotation (controlled by A/D keys)
- **View Matrix**: Handles camera position (controlled by W/S keys for zoom)
- **Projection Matrix**: Handles perspective transformation

These are updated in the `updateUniformBuffer()` function and passed to shaders via uniform buffers.

## File Structure

```
part3/
├── CMakeLists.txt      # Build configuration
├── main.cpp            # Main application code
├── shader.vert         # Vertex shader (GLSL)
├── shader.frag         # Fragment shader (GLSL)
└── README.md           # This file
```

## Troubleshooting

### "failed to find GPUs with Vulkan support"
- Ensure your GPU supports Vulkan
- Update your graphics drivers
- Verify Vulkan SDK is properly installed

### "failed to open file!" (shader files)
- Make sure to run the executable from the build directory
- The compiled shaders (`vert.spv` and `frag.spv`) must be in the same directory as the executable

### Build fails with missing libraries
- Verify all dependencies are installed
- Check CMake output for specific missing components
- Ensure `VULKAN_SDK` environment variable is set (if on Windows)

## Technical Details

- **Graphics API**: Vulkan 1.0
- **Windowing**: GLFW 3.x
- **Mathematics**: GLM (OpenGL Mathematics)
- **Shader Language**: GLSL 4.50 (compiled to SPIR-V)
- **Rendering**: Triangle list with indexed drawing
- **Synchronization**: Double-buffered with semaphores and fences

## Extension Ideas

The current architecture supports easy extension to:

1. **Per-vertex animation**: Modify vertex positions over time
2. **Texture mapping**: Add texture coordinates and sampler
3. **Lighting**: Implement Phong or PBR shading
4. **Multiple objects**: Instantiate multiple cubes with different transforms
5. **Particle systems**: Render individual animated points
6. **Post-processing**: Add render passes for effects

## License

Part of the c64_demos repository. See repository root for license information.
