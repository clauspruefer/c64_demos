# Realistic Fly Animation Generator

Generates a photorealistic fly animation inspired by the movie "Mandibules" where a fly approaches the camera from a distance until only its giant compound eyes are visible.

## Overview

This tool creates a high-quality, realistic animation sequence showing:
- A fly starting far away in the distance
- The fly gradually approaching directly towards the camera
- The camera position remains static throughout
- Photorealistic 3D-style rendering with gradients and lighting
- Eventually only the two gigantic compound eyes fill the screen

## Features

- **Full HD Resolution**: 1920x1080 pixels for high-quality output
- **Realistic rendering**: 3D-style shading with gradients and lighting effects
- **Photorealistic compound eyes**: Detailed hexagonal facet pattern with individual lighting
- **Advanced texturing**: Body segments, hairs, and realistic fly anatomy
- **Semi-transparent wings**: With visible veins and blur effects
- **Natural motion**: Flight wobble and wing flapping animation
- **Depth effects**: Subtle depth-of-field blur when very close

## Output

- Resolution: 1920x1080 pixels (Full HD)
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

**Note**: Rendering takes longer than the C64 version due to high resolution and realistic 3D-style effects.

## Creating a Video

You can convert the PNG frames to a video using ffmpeg:

```bash
ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p fly_animation_hd.mp4
```

## Technical Details

### Animation Parameters

- **Duration**: 150 frames (6 seconds at 25fps)
- **Approach**: Exponential distance decay for dramatic effect
- **Rendering**: Photorealistic with gradient fills and 3D lighting

### Realistic Rendering Features

- **Gradient shading**: Radial gradients on body and eyes for 3D effect
- **Dynamic lighting**: Facets are lit based on angle relative to light source
- **Spherical projection**: Eye facets follow sphere curvature
- **Reflections**: Specular highlights on compound eyes
- **Transparency**: Semi-transparent wings with alpha blending
- **Texture details**: Body hairs, wing veins, and surface details

### Color Palette

- **Background**: Dark blue-grey (#0F1419)
- **Body**: Dark grey with lighter highlights
- **Eyes**: Dark reddish-brown base with red facets
- **Wings**: Semi-transparent grey with darker veins
- **Highlights**: Natural specular reflections

### Distance Stages

1. **Far (frames 0-80)**: Full fly visible with wings, body, and eyes
2. **Close (frames 81-149)**: Only the two giant eyes with detailed facets and lighting

## Requirements

- Python 3.6+
- Pillow (PIL) with ImageFilter support

Install requirements:
```bash
pip3 install pillow
```

## Inspiration

The animation is inspired by the 2020 French comedy film "Mandibules" directed by Quentin Dupieux, which features memorable close-up shots of a fly from the fly's perspective. This version creates a realistic, photographic-quality animation rather than a stylized C64 version.
