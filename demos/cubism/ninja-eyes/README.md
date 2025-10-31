# Ninja Eyes Demo

A C64 animation inspired by the "Last Ninja" title cover picture featuring morphing eye pupils into 3D rotating cubes.

## Description

This demo displays two ninja eyes on the screen. The pupils of the eyes transform into small 3D rotating cubes, creating a mesmerizing morphing effect.

## Features

- Two ninja eyes rendered in character mode
- Eye pupils morph into 3D wireframe cubes
- Smooth cube rotation animation with 64 frames
- Raster interrupt-driven animation

## Technical Details

### Memory Map
- `$0801-$080c`: BASIC stub (SYS 2062)
- `$080e+`: Main program code
- `$2000+`: Cube rotation data (64 frames, 16 bytes each)

### Animation System
- Uses raster interrupts for smooth animation
- 64 pre-calculated 3D cube rotation frames
- Character-based rendering in the pupil areas
- Frame rate controlled by interrupt counter

### Graphics
- Character mode (40x25)
- Eye rendering using text characters
- Cube visualization using character blocks
- Black background with white/gray eye colors

## Building

Compile with ACME assembler:
```bash
cd demos/cubism/ninja-eyes
python3 generate_cube_rotation.py  # Generate rotation data
acme main.asm                       # Compile to ninja-eyes.prg
```

## Running

Load the compiled `ninja-eyes.prg` file in the VICE C64 emulator:
```bash
x64 ninja-eyes.prg
```

Or on real C64 hardware:
```
LOAD "NINJA-EYES.PRG",8,1
RUN
```

## Files

- `main.asm` - Main assembly source code
- `generate_cube_rotation.py` - Python script to generate 3D cube rotation data
- `cube-rotation-data.i` - Generated rotation data (included by main.asm)
- `ninja-eyes.prg` - Compiled C64 program (excluded from git)

## Inspiration

Inspired by the iconic "Last Ninja" game series, particularly the atmospheric title screens featuring ninja characters with distinctive eyes.

## Author

Created for the c64_demos repository - Cubism demo collection.
