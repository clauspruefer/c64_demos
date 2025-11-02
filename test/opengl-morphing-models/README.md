# OpenGL Rotating 3D Models Demo

A demonstration of 3D animation using GL_POINTS (pixel-based rendering) with 64 vertices per model. The demo features 2 different 3D models that rotate on multiple axes with exportable frame sequences for video production.

## Features

- **2 Unique 3D Models**: Each with exactly 64 vertices
  1. **Torus** - Pink/purple donut shape
  2. **Star** - Rainbow-colored spiky star shape

- **Looping Animation**: Each model's animation loops in 120 frames (12 seconds at 10 FPS) and plays 3 times

- **Multi-Axis Rotation**: Models rotate around X, Y, and Z axes simultaneously for dynamic movement

- **GL_POINTS Rendering**: All models are rendered using OpenGL point primitives (pixel-based mesh)

- **Frame Export**: Exports every frame as PPM image for video production
  - 120 frames per loop × 3 loops = 360 frames per model
  - Total: 720 frames for complete animation cycle (torus loops 3× → star loops 3× → repeat)

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
- **Loops per Model**: 3 (total 36 seconds per model)
- **Total Animation Cycle**: 72 seconds (1 minute 12 seconds)
  - Model 1 (Torus): 360 frames (36 seconds)
  - Model 2 (Star): 360 frames (36 seconds)
  - Total: 720 frames
- **Rendering Mode**: GL_POINTS with point size of 5.0
- **Frame Export**: Every frame saved as PPM (convert to PNG with ImageMagick)
- **Camera**: Fixed at (0, 0, 10) looking at origin

## Animation Sequence

The animation cycles through the models in this order:
1. Torus (loops 3×120 frames = 360 frames)
2. Star (loops 3×120 frames = 360 frames)
... and repeats

Each model displays its looping animation three times (3 × 120 frames = 360 frames = 36 seconds), then switches directly to the next model.

Each individual 120-frame loop is seamlessly loopable. Frames 1-120 complete one full rotation, with frame 1 = frame 121. The rotation continues smoothly across all loops and model switches.

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

The complete animation (720 frames) creates a seamless loop suitable for continuous playback.

## Controls

- Close the window to exit (standard GLUT window controls)

## Implementation Notes

- Point smoothing enabled for better visual quality
- Depth testing enabled for proper 3D rendering
- All models centered at origin (0, 0, 0)
- Fixed camera position for consistent framing across all frames
- Coordinate export uses `gluProject` to convert 3D world coordinates to 2D screen coordinates
- Export outputs x,y coordinates for all 64 vertices to console every frame
- Seamless looping: frames 1-120 complete one full rotation cycle
- Frame 1 = Frame 121 = Frame 241... (0° rotation)
- Frame 120 = Frame 240 = Frame 360... (357° rotation)
- Multi-axis rotation: Objects rotate around Y, X, and Z axes simultaneously
  - Y-axis: 360° per 120 frames (3° per frame)
  - X-axis: 252° per 120 frames (2.1° per frame) - 0.7× Y speed
  - Z-axis: 180° per 120 frames (1.5° per frame) - 0.5× Y speed
- PPM format used for frame export (easily convertible to PNG/MP4)
