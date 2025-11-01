# OpenGL Morphing 3D Models Demo

A demonstration of 3D animation using GL_POINTS (pixel-based rendering) with 64 vertices per model. The demo features 2 different 3D models that smoothly morph into each other.

## Features

- **2 Unique 3D Models**: Each with exactly 64 vertices
  1. **Torus** - Pink/purple donut shape
  2. **Star** - Rainbow-colored spiky star shape

- **Looping Animation**: Each model's animation loops in 640 frames (10.67 seconds at 60 FPS) and plays twice before morphing to the next model

- **Smooth Morphing**: After 2 loops, the model smoothly morphs into the next model over a 2-second transition period

- **GL_POINTS Rendering**: All models are rendered using OpenGL point primitives (pixel-based mesh)

- **Coordinate Export**: Exports projected screen coordinates (x, y) every 10 frames using `gluProject`
  - 6 coordinate snapshots per second at 60 FPS
  - Output printed to console in real-time

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

- **Total Vertices**: 64 per model
- **Frame Rate**: 60 FPS
- **Frames per Loop**: 640 frames (10.67 seconds)
- **Loops Before Morph**: 2 (total 21.33 seconds per model)
- **Morph Duration**: 2 seconds (smooth transition using smoothstep interpolation)
- **Total Animation Cycle**: ~43 seconds (2 models × 21.33 seconds each)
- **Rendering Mode**: GL_POINTS with point size of 5.0
- **Coordinate Export**: Every 10 frames (6 exports per second)

## Animation Sequence

The animation cycles through the models in this order:
1. Torus (loops 2×640 frames) → Star (2 second morph)
2. Star (loops 2×640 frames) → Torus (2 second morph)
... and repeats

Each model displays its looping animation twice (2 × 640 frames = 1280 frames = 21.33 seconds), then smoothly morphs into the next model during the last 2 seconds.

## Controls

- Close the window to exit (standard GLUT window controls)

## Implementation Notes

- Uses smooth interpolation (smoothstep) for natural-looking morphing transitions
- Point smoothing enabled for better visual quality
- Depth testing enabled for proper 3D rendering
- All models centered at origin (0, 0, 0)
- Camera orbits at distance of 10 units from center
- Coordinate export uses `gluProject` to convert 3D world coordinates to 2D screen coordinates
- Export outputs x,y coordinates for all 64 vertices to console every 10 frames
- Each model's animation is designed to loop seamlessly in 640 frames
