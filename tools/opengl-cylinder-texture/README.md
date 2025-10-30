# OpenGL Cylinder with Helix Texture and Scroll Text

This OpenGL program renders a 3D cylinder with textures that wind around it in a helix pattern, featuring a scrolling text effect.

## Features

- **3D Cylinder Geometry**: Generates a procedural cylinder with configurable parameters
- **Helix Texture Pattern**: Textures wind around the cylinder from bottom to top like a rope or helix
- **Scroll Text Effect**: A repeating text message winds around the cylinder as individual letters stacked vertically
- **White/Grayscale Rendering**: Clean monochrome visualization for clear text visibility
- **Smooth Rotation**: The cylinder rotates continuously for better visualization

## Parameters

The following parameters can be adjusted in the source code:

- `RADIUS`: Cylinder radius (default: 2.0)
- `HEIGHT`: Cylinder height (default: 8.0)
- `RADIAL_SEGMENTS`: Number of segments around the cylinder (default: 32)
- `HEIGHT_SEGMENTS`: Number of segments along the height (default: 64)
- `HELIX_ROTATIONS`: Number of complete helix rotations from bottom to top (default: 4.0)
- `SCROLL_TEXT`: The text message to display winding around the cylinder

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
3. **Text Rope**: A white/grayscale band (the "rope") follows the helix path around the cylinder
4. **Text Integration**: Individual characters from the scroll text appear as bright letters along the helix path
5. **Smooth Animation**: The cylinder rotates continuously at 60 FPS

## Technical Notes

- The helix pattern is created by modifying the U texture coordinate based on the V coordinate (height)
- Multiple copies of the scroll text are distributed vertically along the cylinder
- Text appears as a bright band winding around the cylinder like a rope
- White/grayscale rendering provides clear contrast for text visibility
- The text "rope" width is controlled by the `textBandWidth` parameter (default: 0.2)
- OpenGL quad strips are used for efficient rendering of the cylinder surface
