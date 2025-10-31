# Exploding Chicken

A comic-style exploding chicken animation for the Commodore 64.

## Features

- **Enhanced Color Mode (ECM)**: Uses C64's Enhanced Color Mode for up to 4 background colors
- **Colodore Palette**: Uses the authentic Colodore color palette for accurate C64 colors
- **Hires Mode**: Character-based display in text mode (8x8 characters = 64 characters total)
- **16-Frame Animation**: Smooth animation loop showing a chicken exploding in comic style
- **Compact Size**: Only 1.3KB including all animation data

## Animation Sequence

1. **Frames 0-5**: Intact chicken with slight vibration (pre-explosion warning)
2. **Frames 6-9**: Initial explosion with burst particles and flash effect
3. **Frames 10-15**: Dissipating explosion with fading particles

## Technical Details

- **Display Mode**: Enhanced Color Mode (ECM) with character graphics
- **Resolution**: 8x8 characters centered on screen
- **Total Characters**: 64 characters (within ECM limit)
- **Frame Rate**: ~15 fps (updates every 4 video frames)
- **Animation Loop**: 16 frames, seamlessly loops
- **Colors Used**: Black, Yellow (chicken), Orange (explosion), Red (explosion), White (flash)

## Building

To rebuild the animation from source:

```bash
# Generate animation data
python3 generate_chicken.py

# Assemble the program
acme -f cbm -o exploding-chicken.prg main.asm
```

## Running

Load and run in VICE emulator:
```bash
x64 exploding-chicken.prg
```

Or on real C64 hardware, transfer the .prg file and load it normally:
```
LOAD "EXPLODING-CHICKEN.PRG",8,1
RUN
```

## Files

- `main.asm` - Main assembly program with display logic
- `generate_chicken.py` - Python script to generate animation frames
- `chicken-data.i` - Generated animation data (included by main.asm)
- `frames/` - Directory containing PNG renders of each frame (for reference)
- `exploding-chicken.prg` - Compiled C64 program

## Color Palette

Using the Colodore palette (https://www.colodore.com/):
- Background: Black ($00)
- Chicken: Yellow ($07)  
- Beak: Orange ($08)
- Explosion: Red ($02), Orange ($08), Yellow ($07), White ($01)

## Animation Generation

The `generate_chicken.py` script creates the animation by:
1. Drawing each frame as a PIL image using the Colodore RGB palette
2. Converting each 8x8 pixel block to the closest C64 color
3. Exporting frame data as assembly directives for ACME
4. Saving PNG previews to the frames/ directory

## Credits

Created for the c64_demos repository using:
- ACME cross-assembler
- Python 3 with PIL/Pillow
- Colodore C64 color palette
