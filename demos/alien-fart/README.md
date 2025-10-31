# Alien Fart Animation

A humorous C64 demo featuring an alien in a flying saucer who surprises a passing man.

## Story

An alien sits in its flying saucer, peacefully eating donuts in the sky. A man walks by on the ground, eating a hamburger. The man cannot see the alien in its flying saucer high above.

The alien wants to threaten the man, so it flies down by the man's face and farts so loud that the man's hamburger goes flying approximately 20 meters away!

## Animation Sequence

```
State 0 (60 frames):        State 1 (120 frames):
     👽 🍩                        👽 🍩
     🛸                           🛸
                                 
                                  🚶 🍔 →

State 2 (30 frames):       State 3 (60 frames):
                                    🍩
        👽 🍩                       🛸
        🛸                          👽
       💨                   
      🚶  🍔→                🚶         🍔 → → →
                                      (flies 20m!)
```

1. **State 0** (60 frames): Alien eating donuts in the flying saucer
2. **State 1** (120 frames): Man walking from left to right eating a hamburger
3. **State 2** (30 frames): Alien flies down toward the man's face and farts (border flashes green for fart effect)
4. **State 3** (60 frames): Hamburger flies away in a parabolic arc due to the force of the fart
5. **Loop**: Animation resets and repeats

## Technical Details

- **Language**: 6502 Assembly (ACME assembler syntax)
- **Target**: Commodore 64
- **Sprites Used**:
  - Sprite 0: Alien (red)
  - Sprite 1: Flying Saucer (gray)
  - Sprite 2: Donut (yellow)
  - Sprite 3: Man (light gray)
  - Sprite 4: Hamburger (orange)

## Building

To build this demo, you need the ACME cross-assembler.

### Quick Build
```bash
./build.sh
```

### Manual Build
```bash
acme -f cbm -o alien-fart.prg main.asm
```

### Installing ACME (if needed)
```bash
# On Debian/Ubuntu:
sudo apt-get install acme

# Or download from: http://sourceforge.net/p/acme-crossass/
```

## Generating Animation Frames

To generate PNG files for each frame of the animation:

```bash
# Install Python dependencies
pip3 install Pillow

# Generate all 270 frames
./generate_frames.py
```

This will create a `frames/` directory containing:
- **270 PNG files** (frame_0000.png to frame_0269.png)
- Each frame is **320x200 pixels** (C64 screen resolution)
- Organized by animation state:
  - Frames 0-59: Alien eating donuts
  - Frames 60-179: Man walking with hamburger
  - Frames 180-209: Alien flies down and farts
  - Frames 210-269: Hamburger flying away

You can view a preview by opening `preview.html` in your browser after generating the frames.

## Running

Load the generated `alien-fart.prg` file in the VICE C64 emulator or transfer it to real C64 hardware.

In VICE:
```bash
x64 alien-fart.prg
```

Or autostart:
```bash
x64 -autostart alien-fart.prg
```

## Features

- Sprite-based animation
- Multi-state animation system
- Smooth movement and physics (parabolic trajectory for flying hamburger)
- Visual fart effect (border color flash)
- Continuous loop animation

## Credits

Created for the c64_demos repository as a fun animation showcase.
