# OpenGL Cylinder with Helix Texture and Scroll Text

This OpenGL program renders a 3D cylinder with textures that wind around it in a helix pattern, featuring a scrolling text effect.

## Features

- **3D Cylinder Geometry**: Generates a procedural cylinder with configurable parameters
- **Helix Texture Pattern**: Textures wind around the cylinder from bottom to top like a rope or helix
- **Multiple Colored Strands**: Three distinct colored strands (red, green, blue) intertwine around the cylinder
- **Scroll Text Effect**: A repeating text message is encoded in the texture, creating a scrolling effect
- **Smooth Rotation**: The cylinder rotates continuously for better visualization

## Parameters

The following parameters can be adjusted in the source code:

- `RADIUS`: Cylinder radius (default: 2.0)
- `HEIGHT`: Cylinder height (default: 8.0)
- `RADIAL_SEGMENTS`: Number of segments around the cylinder (default: 32)
- `HEIGHT_SEGMENTS`: Number of segments along the height (default: 64)
- `HELIX_ROTATIONS`: Number of complete helix rotations from bottom to top (default: 4.0)
- `NUM_HELIX_STRANDS`: Number of texture strands winding around (default: 3)
- `SCROLL_TEXT`: The text message to display in the texture

## Building

### Requirements

- g++ compiler with C++11 support
- OpenGL development libraries (libGL, libGLU)
- GLUT library (freeglut3-dev)
- GLM library (libglm-dev)

### Install dependencies (Ubuntu/Debian)

```bash
sudo apt-get install freeglut3-dev libglew-dev libglm-dev
```

### Build

```bash
make
```

### Run

```bash
make run
```

or directly:

```bash
./opengl-cylinder-texture
```

## Implementation Details

The program is based on the reference implementation `demos/cubism/part2/opengl/opengl-colored-rotating-cube.cpp` and creates:

1. **Cylinder Geometry**: Vertices are generated in a cylindrical pattern with proper texture coordinates
2. **Helix Mapping**: Texture coordinates are calculated using a helical formula that winds around the cylinder
3. **Color Coding**: Different colored strands help visualize the helix pattern
4. **Text Integration**: Characters from the scroll text modulate the brightness to create a text effect
5. **Smooth Animation**: The cylinder rotates continuously at 60 FPS

## Technical Notes

- The helix pattern is created by modifying the U texture coordinate based on the V coordinate (height)
- Multiple copies of the scroll text are distributed vertically along the cylinder
- The color gradient helps distinguish between different helix strands
- OpenGL quad strips are used for efficient rendering of the cylinder surface
