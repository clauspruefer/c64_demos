# OpenGL Textured Cylinder

A simple OpenGL 3D program that displays a rotating textured cylinder.

## Features

- **Window size**: 320x200 pixels (C64 demo resolution)
- **Cylinder diameter**: 150 pixels (radius: 75 pixels)
- **Texture**: 16 pixels width × 300 pixels height
- **Texture mapping**: Wraps around the cylinder from bottom to top
- **Animation**: Cylinder rotates continuously

## Texture Design

The procedural texture features:
- Vertical gradient from blue (bottom) through cyan to white (top)
- Vertical stripes (4-pixel pattern)
- Horizontal bands every 50 pixels

## Building

### Requirements

- C++ compiler with C++11 support (g++)
- OpenGL development libraries
- GLUT (freeglut3-dev on Debian/Ubuntu)

### Compile

```bash
make
```

### Run

```bash
make run
```

Or directly:

```bash
./opengl-textured-cylinder
```

### Clean

```bash
make clean
```

## Technical Details

- Uses OpenGL immediate mode rendering
- Implements cylinder geometry with triangulated mesh
- 32 segments around the circumference for smooth appearance
- 32 height segments for proper texture mapping
- Includes top and bottom caps
- Basic lighting enabled for 3D appearance
