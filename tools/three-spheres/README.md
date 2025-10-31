# Three Spheres OpenGL 3D Display

This OpenGL C++ program displays **3 static (non-animated) spheres** with different sizes and Y positions.

## Sphere Specifications

### Sphere 1 (Red) - Large, Top Position
- **Radius**: `1.5`
- **Y Position**: `3.0`
- **Y Range**: `1.5 to 4.5`

### Sphere 2 (Green) - Small, Middle Position
- **Radius**: `0.6`
- **Y Position**: `0.5`
- **Y Range**: `-0.1 to 1.1`
- **Note**: Top position (1.1) is smaller than Sphere 1's end position (1.5) ✓

### Sphere 3 (Blue) - Medium, Bottom Position
- **Radius**: `1.0`
- **Y Position**: `-2.5`
- **Y Range**: `-3.5 to -1.5`
- **Note**: Bigger than Sphere 2 (0.6) but smaller than Sphere 1 (1.5) ✓

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

## Features

- ✓ OpenGL 3D rendering with proper depth testing
- ✓ Three spheres with distinct sizes and positions
- ✓ Lighting and shading for realistic 3D appearance
- ✓ Static (non-animated) display
- ✓ Positioned according to Y-axis requirements:
  - Sphere 1: Top of screen (largest)
  - Sphere 2: Middle (smallest, top Y < Sphere 1 bottom Y)
  - Sphere 3: Bottom (medium size)

## Files

- `three-spheres.cpp` - Main C++ source code
- `Makefile` - Build configuration
- `README.md` - This file
