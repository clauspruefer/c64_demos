# Floodlights Demo

A Commodore 64 demo featuring 3 moving and zooming circles acting as floodlights with shadows and color gradients.

## Features

- **3 Animated Circles**: Each circle moves independently with different motion patterns
- **Zooming Effect**: Circles zoom in and out at different rates
- **Overlapping/Morphing**: Circles sometimes overlap creating interesting visual effects
- **Color Gradients**: Uses the Pepto C64 palette for smooth color gradients
- **Shadow Effects**: Dynamic shadows based on circle positions
- **Screen Size**: 32x10 character display (320 bytes in color RAM at $d800)
- **Animation**: 64 pre-calculated frames that loop continuously

## Technical Details

- Animation data is pre-calculated using Python script
- Uses raster interrupt to update color RAM each frame
- Total animation data: ~20KB (64 frames × 320 bytes)
- Frame rate: 50 Hz (PAL) / 60 Hz (NTSC)

## Building

Generate the animation data and assemble:

```bash
python3 generate_floodlights.py
acme -o floodlights.prg -f cbm main.asm
```

## Running

Load in VICE emulator:
```bash
x64 floodlights.prg
```

Or on real hardware, load and run as you would any C64 program.

## Files

- `generate_floodlights.py` - Python script to generate animation data
- `floodlights-data.i` - Generated animation data (ACME format)
- `main.asm` - Main assembly program
- `floodlights.prg` - Compiled C64 program

## Color Palette

The demo uses a carefully selected gradient from the Pepto C64 palette:
- Black → Dark Grey → Medium Grey
- Red (shadows) → Orange → Yellow
- Light Grey → Light Red → Yellow → White (bright centers)

## Algorithm

Each frame:
1. Three circles are positioned using sine waves with different frequencies
2. Each circle's radius oscillates independently
3. For each character position, intensity is calculated based on distance from all circles
4. Shadow factors are calculated when characters are between circles
5. Final intensity is mapped to the gradient color palette
