# OpenGL Morphing Spheres

A 3D OpenGL demonstration program featuring three spheres of different sizes that morph into each other.

## Features

- **Three connected spheres** with different sizes (small, medium, large)
- **Geometric morphing** - spheres blend smoothly into each other through vertex deformation
- **TWF texture** - procedurally generated texture with "TWF" text displayed on the middle sphere
- **3D rendering** with proper lighting and shading
- **Rotating view** for better visualization of the 3D morphing effect

## Building

Requirements:
- C++11 compiler (g++)
- OpenGL development libraries
- GLU (OpenGL Utility Library)
- GLUT/FreeGLUT

On Ubuntu/Debian:
```bash
sudo apt-get install build-essential freeglut3-dev libglu1-mesa-dev
```

Build:
```bash
make
```

## Running

```bash
./morphing-spheres
```

or

```bash
make run
```

## Description

The program displays three spheres arranged vertically:
- **Top sphere** (smallest, red): 0.6 radius
- **Middle sphere** (medium, textured): 1.0 radius - displays "TWF" texture
- **Bottom sphere** (largest, blue): 1.4 radius

The spheres morph into each other through:
1. Vertex blending at the poles of adjacent spheres
2. Semi-transparent connecting regions that smooth the transitions
3. Color gradients that blend between sphere colors

The entire structure rotates slowly to showcase the 3D morphing effect from all angles.

## Technical Details

- Sphere geometry: 64 segments × 32 rings (4,096 triangles)
- Texture: 256×128 procedurally generated bitmap
- Lighting: Single directional light with ambient, diffuse, and specular components
- Blending: Alpha blending for transparent connections
- Morphing: Geometric deformation using vertex position interpolation

## Author

Created as part of the c64_demos repository tools collection.
