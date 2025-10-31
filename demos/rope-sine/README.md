# Rope Sine Wave Animation

A C64 demo featuring 24 vertical ropes animated in a sine wave pattern.

## Description

This demo displays 24 vertical lines ("ropes") that move up and down in a smooth sine wave pattern across the screen. Each rope follows a different phase of the sine wave, creating a flowing wave effect.

## Features

- 24 vertical ropes evenly distributed across the screen
- Smooth sine wave animation with 256 frames
- Character-based graphics for maximum compatibility
- Raster interrupt-driven animation

## Technical Details

- **Display Mode**: Standard character mode
- **Animation**: Raster interrupt at line 50
- **Ropes**: 24 vertical lines using PETSCII block characters
- **Wave Data**: 256 frames × 24 ropes pre-calculated sine wave positions
- **Colors**: White ropes on black background

## Building

The demo requires the ACME cross-assembler:

```bash
# Install ACME (on Ubuntu/Debian)
sudo apt-get install acme

# Generate sine wave data
python3 gen-rope-sine.py

# Assemble the program
acme main.asm
```

This will produce `rope-sine.prg` which can be run on a C64 or in an emulator.

## Running

### On VICE Emulator
```bash
x64 rope-sine.prg
```

### On Real Hardware
Transfer `rope-sine.prg` to your C64 using your preferred method and run:
```
LOAD "ROPE-SINE.PRG",8,1
RUN
```

## Files

- `main.asm` - Main assembly source code
- `gen-rope-sine.py` - Python script to generate sine wave data
- `rope-sine-data.i` - Generated sine wave position data (included by main.asm)
- `rope-sine.prg` - Compiled C64 program

## Implementation Notes

The demo uses character mode rather than sprites to display all 24 ropes simultaneously without complex sprite multiplexing. Each rope is drawn as a vertical column using PETSCII block characters. The Y position of each rope is calculated from pre-generated sine wave data, creating the flowing wave effect.

The animation runs at 50Hz (PAL) / 60Hz (NTSC) synchronized to the raster interrupt, providing smooth motion.
