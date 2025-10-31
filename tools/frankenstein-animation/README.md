# Frankenstein Animation Generator

This tool generates a classic Frankenstein monster face animation in black and white for the Commodore 64.

## Features

- **60 frames** of animated Frankenstein monster face
- **40x25 resolution** (full C64 screen)
- **Black and white palette** - authentic classic horror look
- **PNG export** for each frame
- **Animated effects** including blinking eyes and subtle breathing

## Requirements

- Python 3.x
- Pillow (PIL) library

Install dependencies:
```bash
pip3 install Pillow
```

## Usage

Run the script from this directory:
```bash
python3 generate_frankenstein.py
```

## Output

The script will:
1. Create a `frames/` directory
2. Generate 60 PNG images (frame_0000.png to frame_0059.png)
3. Create `frankenstein-data.i` assembly include file with animation data
4. Each frame is 40x25 characters using black and white C64 colors

## Animation Details

The Frankenstein face features:
- Classic flat-top head
- Iconic neck bolts
- Heavy eyebrows
- Forehead scar with stitches
- Blinking eyes (around frame 27-33)
- Subtle breathing/pulsing effect

## Creating a Video

You can create a video from the generated frames using ffmpeg:
```bash
ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p frankenstein.mp4
```

This creates a ~2.4 second looping video at 25 fps (PAL C64 frame rate).

## Customization

You can modify the following parameters in the script:

- `FRAMES`: Number of frames to generate (default: 60)
- `WIDTH`: Screen width in characters (default: 40)
- `HEIGHT`: Screen height in characters (default: 25)
- Face features and positions in the `draw_frankenstein_face()` function

## Integration with C64 Assembly

The generated `frankenstein-data.i` file can be included in C64 assembly programs:

```asm
!source "frankenstein-data.i"

; Access animation data
lda animation_data,x    ; Load color byte
sta $d800,x             ; Write to color RAM
```

See the main.asm file (if available) for a complete example of displaying the animation.
