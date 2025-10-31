# OpenGL Morphing 3D Models Demo

A demonstration of 3D animation using GL_POINTS (pixel-based rendering) with 128 vertices per model. The demo features 5 different 3D models that smoothly morph into each other.

## Features

- **5 Unique 3D Models**: Each with exactly 128 vertices
  1. **Sphere** - Blue gradient sphere
  2. **Cube** - Red/green gradient cube with edge emphasis
  3. **Torus** - Pink/purple donut shape
  4. **Double Helix** - DNA-like cyan and yellow double spiral
  5. **Star** - Rainbow-colored spiky star shape

- **Smooth Morphing**: Each model is displayed for 60 seconds, then smoothly morphs into the next model over a 2-second transition period

- **GL_POINTS Rendering**: All models are rendered using OpenGL point primitives (pixel-based mesh)

- **Dynamic Camera**: Camera orbits around the models for better viewing

- **Color Gradients**: Each model has unique color gradients to distinguish them

## Building

### Requirements
- Linux operating system
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)
- C++ compiler with C++11 support

### Install Dependencies (Ubuntu/Debian)
```bash
sudo apt-get install build-essential freeglut3-dev
```

### Compile
```bash
make
```

## Running

```bash
make run
```

Or directly:
```bash
./opengl-morphing-models
```

## Technical Details

- **Total Vertices**: 128 per model
- **Frame Rate**: 60 FPS
- **Display Duration**: 60 seconds per model
- **Morph Duration**: 2 seconds (smooth transition using smoothstep interpolation)
- **Total Animation Cycle**: 5 minutes (300 seconds)
- **Rendering Mode**: GL_POINTS with point size of 5.0

## Animation Sequence

The animation cycles through the models in this order:
1. Sphere → Cube (60s)
2. Cube → Torus (60s)
3. Torus → Double Helix (60s)
4. Double Helix → Star (60s)
5. Star → Sphere (60s)
... and repeats

During the last 2 seconds of each 60-second period, the current model smoothly morphs into the next model using vertex interpolation.

## Controls

- Close the window to exit (standard GLUT window controls)

## Implementation Notes

- Uses smooth interpolation (smoothstep) for natural-looking morphing transitions
- Point smoothing enabled for better visual quality
- Depth testing enabled for proper 3D rendering
- All models centered at origin (0, 0, 0)
- Camera orbits at distance of 10 units from center
