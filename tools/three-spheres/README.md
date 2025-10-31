# Three Spheres OpenGL 3D Display

This OpenGL C++ program displays **3 overlapping spheres** forming a **single cohesive 3D model** (similar to a snowman shape) that can have textures applied to it.

## Sphere Specifications

### Sphere 1 (Red) - Top
- **Radius**: `1.5`
- **Y Position**: `1.5`
- **Y Range**: `0.0 to 3.0`

### Sphere 2 (Green) - Middle (Overlapping)
- **Radius**: `0.6`
- **Y Position**: `-0.3`
- **Y Range**: `-0.9 to 0.3`
- **Note**: Overlaps with Sphere 1 (both occupy Y range 0.0 to 0.3)

### Sphere 3 (Blue) - Bottom (Overlapping)
- **Radius**: `1.0`
- **Y Position**: `-1.5`
- **Y Range**: `-2.5 to -0.5`
- **Note**: Overlaps with Sphere 2 (both occupy Y range -0.9 to -0.5)

## Key Features

- **Overlapping geometry**: Spheres are positioned to overlap and form a single unified 3D model
- **Texture-ready**: The compound surface is suitable for texture mapping
- **Size variation**: Three different sphere sizes (1.5, 0.6, 1.0) create visual hierarchy

## Build and Run

```bash
cd tools/three-spheres
make
./three-spheres
```

## Dependencies

- OpenGL
- GLUT (freeglut3-dev)
- GLU (libglu1-mesa-dev)

On Ubuntu/Debian:
```bash
sudo apt-get install freeglut3-dev libglu1-mesa-dev mesa-common-dev
```

## Technical Details

- ✓ OpenGL 3D rendering with proper depth testing
- ✓ Three overlapping spheres forming a single 3D model
- ✓ Lighting and shading for realistic 3D appearance
- ✓ Static (non-animated) display
- ✓ Overlapping geometry creates a compound surface suitable for texturing
- ✓ Camera positioned to view the complete unified model

## Files

- `three-spheres.cpp` - Main C++ source code
- `Makefile` - Build configuration
- `README.md` - This file
