# Bresenham 2D Line Drawing Algorithm

This directory contains a simple implementation of Bresenham's line drawing algorithm in Python3.

## Overview

Bresenham's line algorithm is an efficient way to draw lines on a raster display. It uses only integer arithmetic, making it fast and suitable for computer graphics.

## Files

- `bresenham_line.py` - Main implementation with the LineDrawer class

## Usage

### Basic Usage

```python
from bresenham_line import LineDrawer

# Create a canvas (320x200 pixels, black background)
drawer = LineDrawer(width=320, height=200, background_color=(0, 0, 0))

# Draw a line from (10, 10) to (100, 50) in white
drawer.draw_line(10, 10, 100, 50, color=(255, 255, 255))

# Draw another line in red
drawer.draw_line(50, 150, 200, 180, color=(255, 0, 0))

# Save the image
drawer.save('my_lines.png')
```

### Running the Demo

Run the built-in demo to see several lines drawn:

```bash
python3 bresenham_line.py
```

This will create `bresenham_demo.png` with:
- A colored box (red, green, blue, yellow edges)
- Two diagonal lines (magenta and cyan)
- A star pattern from the center (white)

## API Reference

### LineDrawer Class

#### `__init__(width=320, height=200, background_color=(0, 0, 0))`
Initialize a new line drawer with a canvas.

**Parameters:**
- `width` (int): Canvas width in pixels (default: 320)
- `height` (int): Canvas height in pixels (default: 200)
- `background_color` (tuple): RGB color tuple for background (default: black)

#### `draw_line(x0, y0, x1, y1, color=(255, 255, 255))`
Draw a line using Bresenham's algorithm.

**Parameters:**
- `x0` (int): Starting x coordinate
- `y0` (int): Starting y coordinate
- `x1` (int): Ending x coordinate
- `y1` (int): Ending y coordinate
- `color` (tuple): RGB color tuple for the line (default: white)

#### `save(filename)`
Save the canvas as a PNG file.

**Parameters:**
- `filename` (str): Output filename (should end with .png)

#### `clear()`
Clear the canvas to the background color.

## Requirements

- Python 3.x
- Pillow (PIL) library

Install requirements:
```bash
pip3 install Pillow
```

## Algorithm Details

The implementation uses the classic Bresenham algorithm:
1. Calculate the absolute differences in x and y coordinates
2. Determine the step direction for x and y
3. Initialize the error term
4. Iterate through points, adjusting x or y based on the error term
5. Draw pixels only within canvas bounds

## Examples

### Drawing a Simple Line
```python
drawer = LineDrawer(width=100, height=100)
drawer.draw_line(0, 0, 99, 99, color=(255, 0, 0))  # Red diagonal
drawer.save('diagonal.png')
```

### Drawing Multiple Lines
```python
drawer = LineDrawer(width=200, height=200)

# Draw a triangle
drawer.draw_line(100, 20, 20, 180, color=(255, 0, 0))
drawer.draw_line(20, 180, 180, 180, color=(0, 255, 0))
drawer.draw_line(180, 180, 100, 20, color=(0, 0, 255))

drawer.save('triangle.png')
```

### Custom Canvas Size and Background
```python
# HD canvas with blue background
drawer = LineDrawer(width=1920, height=1080, background_color=(0, 0, 128))
drawer.draw_line(0, 540, 1920, 540, color=(255, 255, 0))  # Horizontal yellow line
drawer.save('hd_line.png')
```
