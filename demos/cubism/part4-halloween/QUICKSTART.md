# Quick Start Guide - Halloween Demo

## What You Need

1. **ACME Cross-Assembler** - To compile the .asm file
   - Download from: http://sourceforge.net/projects/acme-crossass/
   - Or install via package manager: `apt-get install acme` (Linux)

2. **VICE Emulator** - To run the demo
   - Download from: https://vice-emu.sourceforge.io/
   - Recommended: x64 (C64 emulator)

## Building the Demo

```bash
# Navigate to the demo directory
cd demos/cubism/part4-halloween

# Compile with ACME
acme main.asm

# This creates: cubism-part4-halloween.prg
```

## Running the Demo

### Option 1: VICE Emulator (Recommended)
```bash
x64 cubism-part4-halloween.prg
```

The demo will auto-start and you'll see:
- Floating pumpkin sprites
- Halloween-colored raster bars
- Spooky messages

### Option 2: Manual Load in VICE
1. Start VICE: `x64`
2. Attach the PRG: File → Autostart disk/tape image
3. Select `cubism-part4-halloween.prg`
4. It will load and run automatically

### Option 3: Real C64 Hardware
1. Transfer the .prg file to a C64 disk image
2. Load on real hardware:
   ```
   LOAD"CUBISM-PART4-HALLOWEEN.PRG",8,1
   RUN
   ```

## Preview Without C64

If you don't have ACME or VICE installed, you can still see what the demo would look like:

### Static Preview
```bash
cat PREVIEW.txt
```

### Animated Preview (requires Python 3)
```bash
python3 preview.py
```
Press Ctrl+C to exit the animation.

## Customization

### Regenerate Sine Wave Data
If you want to modify the pumpkin movement:

```bash
cd python
python3 gen-halloween-sin.py
```

This regenerates:
- `pumpkin-sine-x.i` - Horizontal movement
- `pumpkin-sine-y.i` - Vertical wobble

Edit the script to change amplitude, frequency, or offset.

### Modify Colors
Edit `halloween-colors.i` to change the raster bar colors.

C64 color codes:
- 0 = Black
- 2 = Red  
- 4 = Purple
- 5 = Green
- 8 = Orange
- 9 = Brown

### Modify Sprite
Edit `pumpkin-sprite.i` to change the pumpkin design.
Each sprite is 21 rows of 3 bytes (63 bytes total).

## Troubleshooting

**"ACME not found"**
- Install ACME assembler first

**"Syntax error" during assembly**
- Make sure you're using ACME, not other assemblers (KickAss, CA65, etc.)
- ACME uses `!` directives, not `.` directives

**"File not found" errors**
- Make sure all .i include files are in the same directory
- Check that Python script generated the sine wave files

**Demo doesn't start**
- Check VICE configuration
- Try different VICE versions (x64sc vs x64)
- Ensure .prg file was created successfully

## Files Overview

- `main.asm` - Main demo program (assemble this)
- `pumpkin-sprite.i` - Sprite graphic data
- `pumpkin-sine-x.i` - Generated horizontal movement
- `pumpkin-sine-y.i` - Generated vertical movement  
- `halloween-colors.i` - Raster bar color data
- `python/gen-halloween-sin.py` - Sine wave generator
- `preview.py` - Terminal animation preview
- `PREVIEW.txt` - Static ASCII preview
- `README.md` - Full documentation

## Happy Halloween! 🎃

Enjoy the spooky demo!
