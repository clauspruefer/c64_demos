# Ninja Eyes Demo

A C64 animation inspired by the "Last Ninja" title cover picture featuring morphing eye pupils into 3D rotating cubes.

## Description

This demo displays two ninja eyes on the screen. The pupils of the eyes transform into small 3D rotating cubes, creating a mesmerizing morphing effect. The animation runs continuously, showing the cube vertices rotating in real-time within the pupil areas.

## Features

- Two ninja eyes rendered in character mode with PETSCII graphics
- Eye pupils morph into 3D wireframe cubes
- Smooth cube rotation animation with 64 pre-calculated frames
- Raster interrupt-driven animation for consistent frame timing
- Full 3D vertex projection with perspective
- Character-based rendering optimized for C64

## Visual Design

The eyes are designed with:
- Curved eye outline using PETSCII box-drawing characters
- White eye whites (color 1)
- Dark gray pupils (color 11) that serve as the morphing area
- Light blue title text ("NINJA EYES - MORPHING PUPILS")
- Light gray info text ("INSPIRED BY LAST NINJA")

## Technical Details

### Memory Map
- `$0801-$080c`: BASIC stub (SYS 2062)
- `$080e+`: Main program code
- `$2000+`: Cube rotation data (64 frames × 16 bytes = 1024 bytes)
- `$fb-$fc`: Zero page pointer for screen rendering

### Animation System
- Uses raster interrupts (line $80) for smooth animation
- 64 pre-calculated 3D cube rotation frames
- Character-based rendering in the pupil areas (3×3 character grid)
- Frame rate controlled by interrupt counter (updates every 4 VIC frames)
- Cube vertices rendered as block characters ($a0)

### 3D Mathematics
- Cube defined with 8 vertices in 3D space
- Rotation applied around X, Y, and Z axes with different rates
- Perspective projection: `factor = distance / (distance + z)`
- Coordinates scaled and centered for pupil area (24×8 pixel region)
- Vertices projected to 2D screen coordinates

### Graphics
- Character mode (40×25)
- Eye rendering using PETSCII text characters
- Cube visualization using character blocks at vertex positions
- Black background (color 0) with colored text elements
- Screen memory at $0400, color memory at $d800

## Building

The demo requires ACME assembler. Build process:

```bash
cd demos/cubism/ninja-eyes

# Generate rotation data
python3 generate_cube_rotation.py

# Generate eye graphics data  
python3 generate_eye_data.py

# Compile to ninja-eyes.prg
acme main.asm
```

## Running

### VICE Emulator
Load the compiled `ninja-eyes.prg` file in the VICE C64 emulator:
```bash
x64 ninja-eyes.prg
```

Or from within VICE:
```
LOAD "NINJA-EYES.PRG",8,1
RUN
```

### Real C64 Hardware
Transfer the PRG file to a C64 disk or SD card interface:
```
LOAD "NINJA-EYES.PRG",8,1
RUN
```

## Exporting Animation Frames

To export individual animation frames as PNG images:

```bash
cd demos/cubism/ninja-eyes
python3 export_frames.py
```

This generates:
- `frames/eye_frame.png` - Static eye graphic (1 frame)
- `frames/cube_frame_000.png` to `frames/cube_frame_063.png` - Cube rotation frames (64 frames)
- `frames/scene_frame_000.png` to `frames/scene_frame_063.png` - Full scene frames (64 frames)

Total: 129 PNG images showing the complete animation sequence.

The frames are rendered at 4× scale (1280×800 pixels for full scene) for better visibility.

## Files

- `main.asm` - Main assembly source code (C64 6510 assembly)
- `generate_cube_rotation.py` - Python script to generate 3D cube rotation data
- `generate_eye_data.py` - Python script to generate eye character/color data
- `export_frames.py` - Python script to export animation frames as PNG images
- `cube-rotation-data.i` - Generated rotation data (included by main.asm)
- `eye-data.i` - Generated eye graphics data (included by main.asm)
- `ninja-eyes.prg` - Compiled C64 program (excluded from git)
- `frames/` - Exported PNG animation frames (excluded from git)
- `README.md` - This documentation file

## Algorithm Details

### Cube Rotation Generation
The Python script `generate_cube_rotation.py`:
1. Defines cube with 8 vertices at coordinates ±1 in each dimension
2. For each of 64 frames:
   - Calculates rotation angle: `angle = (frame / 64) × 2π`
   - Applies rotation matrices around X, Y, Z axes
   - Projects 3D coordinates to 2D using perspective
   - Scales to fit pupil area (12×8 pixel region)
   - Outputs as byte pairs (x, y) for each vertex

### Animation Loop
The assembly code:
1. Initializes screen, draws eyes, sets up interrupts
2. In IRQ handler (triggered 50 times/second PAL):
   - Increments frame counter
   - Every 4th frame: advances to next cube rotation frame
   - Clears pupil areas (3×3 character grid)
   - Reads vertex data for current frame
   - Converts vertex coordinates to screen positions
   - Renders vertices as block characters
3. Loops back to step 2

## Inspiration

Inspired by the iconic "Last Ninja" game series by System 3 (1987-1991), particularly the atmospheric title screens featuring ninja characters with distinctive eyes. The game was known for its cinematic presentation and attention to visual detail on the C64.

## Author

Created for the c64_demos repository - Cubism demo collection.
Part of the Hitmen demo group productions.
