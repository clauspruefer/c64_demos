# OpenGL Morphing 3D Models Demo

A demonstration of 3D animation using GL_POINTS (pixel-based rendering) with 64 vertices per model. The demo features 2 different 3D models that smoothly morph into each other with exportable frame sequences for video production.

## Features

- **2 Unique 3D Models**: Each with exactly 64 vertices
  1. **Torus** - Pink/purple donut shape
  2. **Star** - Rainbow-colored spiky star shape

- **Looping Animation**: Each model's animation loops in 180 frames (18 seconds at 10 FPS) and plays 3 times before morphing to the next model

- **Multi-Axis Rotation**: Objects rotate around Y, X, and Z axes simultaneously for dynamic movement
  - Y-axis: 360° per 180 frames (1 full rotation)
  - X-axis: 720° per 180 frames (2 full rotations)
  - Z-axis: 360° per 180 frames (1 full rotation)
  - All rotations complete in 180 frames for perfect seamless looping

- **Smooth Morphing**: After 3 loops, the model smoothly morphs into the next model over 180 frames (18 seconds)

- **GL_POINTS Rendering**: All models are rendered using OpenGL point primitives (pixel-based mesh)

- **Frame Export**: Exports every frame as PPM image for video production
  - 180 frames per loop × 3 loops = 540 frames per model
  - 180 frames for morph transition
  - Total: 1440 frames for complete animation cycle (torus loops → morph → star loops → morph → repeat)

- **Coordinate Export**: Exports projected screen coordinates (x, y) every frame using `gluProject`
  - Output printed to console in real-time
  - Also saved to files: `coordinates_000000.txt`, `coordinates_000001.txt`, etc.
  - Format: `vertex[0]:x,y` on each line for all 64 vertices per frame
  - Coordinate files are exported at the same time as PPM image frames

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
- **Frames per Loop**: 180 frames (18 seconds)
- **Loops Before Morph**: 3 (total 54 seconds per model)
- **Morph Duration**: 180 frames (18 seconds)
- **Total Animation Cycle**: 144 seconds (2 minutes 24 seconds)
  - Model 1: 540 frames (54 seconds)
  - Morph 1→2: 180 frames (18 seconds)
  - Model 2: 540 frames (54 seconds)
  - Morph 2→1: 180 frames (18 seconds)
  - Total: 1440 frames
- **Rendering Mode**: GL_POINTS with point size of 5.0
- **Frame Export**: Every frame saved as PPM (convert to PNG with ImageMagick)
- **Coordinate Export**: x,y coordinates for all 64 vertices saved to text files
- **Camera**: Fixed at (0, 0, 10) looking at origin

## Animation Sequence

The animation cycles through the models in this order:
1. Torus (loops 3×180 frames = 540 frames) → Morph to Star (180 frames)
2. Star (loops 3×180 frames = 540 frames) → Morph to Torus (180 frames)
... and repeats

Each model displays its looping animation three times (3 × 180 frames = 540 frames = 54 seconds), then smoothly morphs into the next model during 180 frames (18 seconds).

The rotation animation is separate from the morphing animation - objects rotate 360° per loop (180 frames), making each loop seamless and exportable as a loopable video segment.

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

The complete animation (1440 frames) creates a seamless loop suitable for continuous playback.

## Controls

- Close the window to exit (standard GLUT window controls)

## Implementation Notes

- Uses smooth interpolation (smoothstep) for natural-looking morphing transitions
- Point smoothing enabled for better visual quality
- Depth testing enabled for proper 3D rendering
- All models centered at origin (0, 0, 0)
- Fixed camera position for consistent framing across all frames
- Coordinate export uses `gluProject` to convert 3D world coordinates to 2D screen coordinates
- Export outputs x,y coordinates for all 64 vertices to console AND files every frame
- Coordinate files: `coordinates_000000.txt` (one per frame, same numbering as PPM frames)
- Each coordinate file contains 64 lines in format: `vertex[0]:x,y`
- Each model's animation is designed to loop seamlessly in 180 frames
- Object rotation is independent from morphing - ensures seamless loops
- Multi-axis rotation for dynamic movement:
  - Y-axis: 360° per 180 frames (1 full rotation, 2° per frame)
  - X-axis: 720° per 180 frames (2 full rotations, 4° per frame)
  - Z-axis: 360° per 180 frames (1 full rotation, 2° per frame)
- All rotation angles align perfectly at frame 0 and frame 180 for seamless looping
- PPM format used for frame export (easily convertible to PNG/MP4)
