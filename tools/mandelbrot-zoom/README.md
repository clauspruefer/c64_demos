# Mandelbrot Endless Zoom Animation Generator

This tool generates a 4000-frame endless zoom animation into the Mandelbrot set, rendered in the Colodore C64 palette at 320x200 resolution.

Available in both Python and C++ implementations.

## Features

- **4000 frames** of smooth zoom animation
- **320x200 resolution** matching C64 hi-res mode
- **Colodore palette** - accurate C64 colors
- **PNG export** for each frame
- **Endless zoom** effect into the Mandelbrot set

## Python Version

### Requirements

- Python 3.x
- Pillow (PIL) library

Install dependencies:
```bash
pip3 install Pillow
```

### Usage

Run the script from this directory:
```bash
python3 generate_mandelbrot_zoom.py
```

## C++ Version

### Requirements

- g++ compiler with C++11 support
- libpng development library

Install dependencies (Ubuntu/Debian):
```bash
sudo apt-get install libpng-dev
```

### Build

```bash
make
```

This will compile `generate_mandelbrot_zoom.cpp` into an executable.

### Usage

Run the compiled program:
```bash
./generate_mandelbrot_zoom
```

### Clean

To remove the compiled executable and generated frames:
```bash
make clean
```

## Output

Both versions will:
1. Create a `frames/` directory
2. Generate 4000 PNG images (frame_0000.png to frame_3999.png)
3. Each frame is 320x200 pixels using the Colodore C64 palette

**Note:** The C++ version is significantly faster than Python (typically 5-10x speedup).

## Creating a Video

You can create a video from the generated frames using ffmpeg:
```bash
ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p mandelbrot_zoom.mp4
```

This creates a ~2.5 minute video at 25 fps (PAL C64 frame rate).

## Customization

You can modify the following parameters in either script:

- `FRAMES`: Number of frames to generate (default: 4000)
- `ZOOM_FACTOR`: Zoom speed per frame (default: 1.02)
- `CENTER_X`, `CENTER_Y`: Coordinates in the Mandelbrot set to zoom into
- `MAX_ITER`: Maximum iterations for Mandelbrot calculation (affects detail)
- `WIDTH`, `HEIGHT`: Output resolution (default: 320x200)

## Technical Details

The animation zooms into coordinates (-0.743643887037151, 0.131825904205330) in the Mandelbrot set, known as the "seahorse valley" - a classic target for Mandelbrot zoom animations that reveals beautiful fractal patterns.

The zoom factor of 1.02 per frame results in a final zoom level of approximately 1.9e+34x after 4000 frames, revealing incredible detail in the fractal structure.
