# OpenGL Morphing 3D Models Demo

A demonstration of 3D animation using GL_POINTS (pixel-based rendering) with 64 vertices per model. The demo features 2 different 3D models that smoothly morph into each other with exportable frame sequences for video production.

## Features

- **2 Unique 3D Models**: Each with exactly 64 vertices
  1. **Torus** - Pink/purple donut shape
  2. **Star** - Rainbow-colored spiky star shape

- **Looping Animation**: Each model's animation loops in 120 frames (12 seconds at 10 FPS) and plays 3 times before morphing to the next model

- **Smooth Morphing**: After 3 loops, the model smoothly morphs into the next model over 120 frames (12 seconds)

- **GL_POINTS Rendering**: All models are rendered using OpenGL point primitives (pixel-based mesh)

- **Frame Export**: Exports every frame as PPM image for video production
  - 120 frames per loop × 3 loops = 360 frames per model
  - 120 frames for morph transition
  - Total: 960 frames for complete animation cycle (torus loops → morph → star loops → morph → repeat)

- **Coordinate Export**: Exports projected screen coordinates (x, y) every frame using `gluProject`
  - Output printed to console in real-time

- **Fixed Camera**: Camera at fixed position (no zooming or orbiting)

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
- **Frame Rate**: 10 FPS
- **Frames per Loop**: 120 frames (12 seconds)
- **Loops Before Morph**: 3 (total 36 seconds per model)
- **Morph Duration**: 120 frames (12 seconds)
- **Total Animation Cycle**: 96 seconds (1 minute 36 seconds)
  - Model 1: 360 frames (36 seconds)
  - Morph 1→2: 120 frames (12 seconds)
  - Model 2: 360 frames (36 seconds)
  - Morph 2→1: 120 frames (12 seconds)
  - Total: 960 frames
- **Rendering Mode**: GL_POINTS with point size of 5.0
- **Frame Export**: Every frame saved as PPM (convert to PNG with ImageMagick)
- **Camera**: Fixed at (0, 0, 10) looking at origin

## Animation Sequence

The animation cycles through the models in this order:
1. Torus (loops 3×120 frames = 360 frames) → Morph to Star (120 frames)
2. Star (loops 3×120 frames = 360 frames) → Morph to Torus (120 frames)
... and repeats

Each model displays its looping animation three times (3 × 120 frames = 360 frames = 36 seconds), then smoothly morphs into the next model during 120 frames (12 seconds).

The rotation is continuous across the entire 960-frame animation cycle. Objects rotate at a constant speed (360° per 120 frames), completing 8 full rotations during the complete animation. This ensures frame 0 and frame 960 have identical rotation angles, making the animation perfectly loopable.

## Video Export

To create a video from the exported frames:

1. Run the program to generate PPM frames:
```bash
./opengl-morphing-models
```

2. Convert PPM to PNG (optional, for smaller file sizes):
```bash
for f in frame_*.ppm; do convert "$f" "${f%.ppm}.png"; done
```

3. Create MP4 video with ffmpeg:
```bash
# At 10 FPS (original speed)
ffmpeg -framerate 10 -i frame_%06d.png -c:v libx264 -pix_fmt yuv420p output.mp4

# Or at 30 FPS (3x speed)
ffmpeg -framerate 30 -i frame_%06d.png -c:v libx264 -pix_fmt yuv420p output_30fps.mp4
```

The complete animation (960 frames) creates a seamless loop suitable for continuous playback.

## Controls

- Close the window to exit (standard GLUT window controls)

## Implementation Notes

- Uses smooth interpolation (smoothstep) for natural-looking morphing transitions
- Point smoothing enabled for better visual quality
- Depth testing enabled for proper 3D rendering
- All models centered at origin (0, 0, 0)
- Fixed camera position for consistent framing across all frames
- Coordinate export uses `gluProject` to convert 3D world coordinates to 2D screen coordinates
- Export outputs x,y coordinates for all 64 vertices to console every frame
- Rotation is continuous across entire 960-frame cycle for perfect looping
- Objects complete 8 full rotations (8×120 frames = 960 frames) per animation cycle
- Morphing happens as a separate phase AFTER the loops complete
- Multi-axis rotation: Objects rotate around Y, X, and Z axes simultaneously
  - Y-axis: 360° per 120 frames (3° per frame) - completes 8 times in 960 frames
  - X-axis: 252° per 120 frames (2.1° per frame) - 0.7× Y speed
  - Z-axis: 180° per 120 frames (1.5° per frame) - 0.5× Y speed
- Frame 0 and frame 960 have identical rotation angles, ensuring seamless looping
- PPM format used for frame export (easily convertible to PNG/MP4)
