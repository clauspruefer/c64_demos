# Cow Fart Animation Generator

This tool generates a funny 32-frame animation of a man standing behind a farting cow. The fart is so intense that the man's hair looks like a hurricane has blown it away!

## Features

- **32 frames** of hilarious animation
- **160x200 resolution** (C64 multicolor mode)
- **Colodore palette** - accurate C64 colors
- **PNG export** for each frame
- Animated cow with wagging tail
- Man with progressively wilder wind-blown hair
- Intense fart cloud expanding from cow's rear
- Wind lines showing the hurricane-force blast

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
python3 generate_cow_fart_animation.py
```

Or if executable:
```bash
./generate_cow_fart_animation.py
```

## Output

The script will:
1. Create a `frames/` directory
2. Generate 32 PNG images (frame_0000.png to frame_0031.png)
3. Each frame is 160x200 pixels using the Colodore C64 palette

## Creating Media from Frames

### Video (MP4)
Create a video from the generated frames using ffmpeg:
```bash
ffmpeg -framerate 12 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p cow_fart.mp4
```

This creates a ~2.6 second video at 12 fps.

### Animated GIF
Create a looping GIF:
```bash
ffmpeg -framerate 12 -i frames/frame_%04d.png -vf "scale=320:400:flags=neighbor" cow_fart.gif
```

The `scale` filter doubles the resolution for better visibility, and `flags=neighbor` preserves the pixelated C64 aesthetic.

## Animation Details

The animation shows:
- A brown cow with white spots on the right side of the screen
- A man (in grey shirt, brown pants) on the left, standing behind the cow
- Frame 0: Normal scene, cow's tail wagging
- Frames 1-15: Fart cloud begins emanating from cow's rear, expanding toward the man
- Frames 5-31: Man's hair increasingly blown by hurricane-force wind
- The fart cloud uses greenish, yellowish, and cyan colors for comedic effect
- Wind lines show the intensity of the blast
- The man's arms are pushed back by the wind force

## Customization

You can modify the following parameters in the script:

- `FRAMES`: Number of frames to generate (default: 32)
- `WIDTH`, `HEIGHT`: Output resolution (default: 160x200)
- Cow position and size
- Man position and size
- Hair animation intensity
- Fart cloud colors and expansion rate

## C64 Technical Details

The animation uses the Colodore palette, which is based on Pepto's accurate C64 color measurements. The 160x200 resolution matches the C64's multicolor bitmap mode, which allows for more colors per character cell at the cost of horizontal resolution.

Perfect for C64 demos, comedy sketches, or just having a laugh!
