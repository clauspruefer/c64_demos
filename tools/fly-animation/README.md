# Fly Animation Generator

Generates a realistic fly animation inspired by the movie "Mandibules" where a fly approaches the camera from a distance until only its giant compound eyes are visible.

## Overview

This tool creates an animation sequence showing:
- A fly starting far away in the distance
- The fly gradually approaching directly towards the camera
- The camera position remains static throughout
- As the fly gets closer, more detail becomes visible
- Eventually only the two gigantic compound eyes fill the screen

## Features

- **Realistic perspective**: The fly grows in size using perspective scaling
- **Compound eyes**: Detailed hexagonal facet pattern on the eyes when close
- **Wing animation**: Subtle wing flapping during flight
- **Flight wobble**: Natural flight path with slight wobbling
- **C64 Colodore palette**: Uses accurate C64 color palette for authenticity

## Output

- Resolution: 320x200 pixels (C64 standard)
- Format: PNG images
- Number of frames: 150 (configurable)
- Frame naming: `frame_0000.png` to `frame_0149.png`

## Usage

```bash
cd tools/fly-animation
python3 generate_fly_animation.py
```

The script will:
1. Create a `frames/` directory
2. Generate all animation frames as PNG files
3. Save them sequentially as `frame_0000.png`, `frame_0001.png`, etc.

## Creating a Video

You can convert the PNG frames to a video using ffmpeg:

```bash
ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p fly_animation.mp4
```

## Technical Details

### Animation Parameters

- **Duration**: 150 frames (6 seconds at 25fps)
- **Approach**: Exponential distance decay for dramatic effect
- **Color Palette**: Colodore C64 palette (16 colors)
- **Background**: Black (#000000)

### Fly Components

- **Body**: Dark grey ellipse
- **Eyes**: Red base with light red hexagonal facets
- **Wings**: Medium grey, animated with flapping motion

### Distance Stages

1. **Far (frames 0-60)**: Small fly with simple rendering, wings visible
2. **Medium (frames 61-100)**: Larger fly with compound eye details
3. **Close (frames 101-149)**: Only the two giant eyes visible with detailed facets

## Requirements

- Python 3.6+
- Pillow (PIL)

Install requirements:
```bash
pip3 install pillow
```

## Inspiration

The animation is inspired by the 2020 French comedy film "Mandibules" directed by Quentin Dupieux, which features memorable close-up shots of a fly from the fly's perspective.
