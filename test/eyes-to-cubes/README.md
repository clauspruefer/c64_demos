# Eyes to Cubes Animation

This is a 3D OpenGL demonstration that renders **two realistic moving eyes** with pupils that **slowly morph into two colored fast-rotating cubes**. The cubes use the colors of the pupils as textures.

## Features

### Realistic Eyes (0-3 seconds)
- **Two complete eyeballs** with white sclera
- **Colored irises**: Left eye has blue iris, right eye has purple iris
- **Dark pupils** in the center of each iris
- **Dynamic eye movement**: Eyes look around naturally, tracking toward the camera viewport
- **Smooth animations**: Eyes move independently with sine wave patterns
- **Realistic lighting**: Three-dimensional shading with specular highlights

### Morphing Animation (3-7 seconds)
- **Smooth transformation**: Gradual morph from spherical eyes to cubic shapes
- **Ease-in-out timing**: Natural acceleration and deceleration
- **Simultaneous scaling**: Eyes shrink slightly while cubes grow
- **Rotation during morph**: 180-degree rotation adds visual interest to the transition
- **Color preservation**: Pupil colors transfer to cube textures

### Rotating Cubes (7+ seconds)
- **Fast rotation**: Cubes spin rapidly around diagonal axis
- **Textured faces**: Each cube face uses the original pupil color with shading variations
- **Left cube**: Blue (matching left pupil)
- **Right cube**: Purple (matching right pupil)
- **Continuous animation**: Cubes rotate indefinitely

## Technical Details

- **Rendering**: OpenGL with GLUT
- **Window Size**: 800×600 pixels
- **Frame Rate**: 60 FPS
- **Objects**: 
  - 2 spheres for eyeballs (32×32 segments each)
  - 2 smaller spheres for irises (24×24 segments each)
  - 2 tiny spheres for pupils (16×16 segments each)
  - 2 cubes for final state
- **Animation Timeline**:
  - 0.0s - 3.0s: Eyes looking around
  - 3.0s - 7.0s: Morphing transition
  - 7.0s+: Fast rotating cubes
- **Lighting**: Single positional light source with ambient, diffuse, and specular components
- **Materials**: Smooth shading with specular highlights

## Building

### Requirements
- g++ compiler with C++11 support
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)

### On Ubuntu/Debian:
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
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
./eyes-to-cubes
```

## Visual Description

The animation starts with two realistic eyeballs positioned side-by-side, looking directly at the camera. The eyes have:
- White sclera (the outer white part)
- Colored irises (left: blue, right: purple)
- Dark pupils in the center

The eyes move naturally for the first 3 seconds, creating the impression that they are "looking around" with smooth, organic motion patterns.

At the 3-second mark, the eyes begin to morph. The spherical shape gradually transforms into a cubic shape while rotating. The transformation is smooth and gradual, taking 4 seconds to complete.

Once fully transformed (at 7 seconds), the two cubes spin rapidly with the colors of the original pupils applied as textures to their faces, creating a mesmerizing visual effect.

## Color Scheme

- **Left Eye/Cube**: Blue tones (R:0.2, G:0.4, B:0.8)
- **Right Eye/Cube**: Purple tones (R:0.6, G:0.3, B:0.7)
- **Background**: Dark blue (R:0.1, G:0.1, B:0.15)
- **Sclera**: Off-white (R:0.95, G:0.95, B:0.95)

## Implementation Notes

- Uses smooth interpolation for morphing effect
- Implements ease-in-out timing function for natural transitions
- Lighting calculations provide depth and realism
- Eyes track camera viewport with procedural animation
- Cube rotation uses diagonal axis for visual interest
