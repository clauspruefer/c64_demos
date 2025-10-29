# OpenGL Donut Particle Accelerator

This is a 3D OpenGL demonstration that renders a giant donut (torus) designed to look like a particle accelerator ring with technical computer terminals and wires.

## Features

- **Torus Geometry**: A mathematically generated donut shape replacing the cube from the original demo
- **Procedural Texturing**: No external texture files needed - all visual effects are generated procedurally
- **Particle Accelerator Appearance**:
  - Segmented bands to simulate accelerator sections
  - Wire patterns across the surface
  - Terminal indicators with bright spots
  - Energy glow effect on the inner rim (blue glow)
  - Green terminal screens positioned around the ring
  - Metallic steel base color
- **Lighting**: Full 3D lighting with specular highlights
- **Smooth Rotation**: The donut rotates continuously with a moving camera

## Building

### Requirements
- g++ compiler with C++11 support
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)
- GLM (OpenGL Mathematics library)

### On Ubuntu/Debian:
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglm-dev
```

### Compile:
```bash
make
```

### Run:
```bash
make run
```

Or directly:
```bash
./donut-accelerator
```

## Based On

This demo is based on `demos/cubism/part2/opengl/opengl-colored-rotating-cube.cpp` but replaces the cube geometry with a torus and adds procedural texturing to simulate a particle accelerator ring.

## Technical Details

- **Major Radius**: 2.0 (distance from center to tube center)
- **Minor Radius**: 0.5 (tube radius)
- **Segments**: 64 major segments × 32 minor segments
- **Window Size**: 800×600 pixels
- **Frame Rate**: 60 FPS
- **Rendering**: Double-buffered with depth testing and lighting enabled
