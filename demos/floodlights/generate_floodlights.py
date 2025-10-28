#!/usr/bin/env python3
"""
Generate C64 floodlight animation data
Creates a single zooming circle with C64 color gradient
Output: 32x10 character screen (320 bytes color RAM data)
"""

import math
import sys
import os
from PIL import Image

# 9-color C64 gradient using specific C64 palette colors
# Mapping: 0->black, 1->brown, 2->dark grey, 3->orange, 4->mid grey, 5->light red, 6->light grey, 7->light green, 8->white
GRADIENT = [0x00, 0x09, 0x0b, 0x08, 0x0c, 0x0a, 0x0f, 0x0d, 0x01]

# RGB values for C64 colors (Pepto palette)
# Based on Pepto's palette: http://www.pepto.de/projects/colorvic/
C64_PALETTE_RGB = {
    0x00: (0, 0, 0),           # Black
    0x01: (255, 255, 255),     # White
    0x02: (136, 0, 0),         # Red
    0x03: (170, 255, 238),     # Cyan
    0x04: (204, 68, 204),      # Purple
    0x05: (0, 204, 85),        # Green
    0x06: (0, 0, 170),         # Blue
    0x07: (238, 238, 119),     # Yellow
    0x08: (221, 136, 85),      # Orange
    0x09: (102, 68, 0),        # Brown
    0x0a: (255, 119, 119),     # Light Red
    0x0b: (51, 51, 51),        # Dark Grey
    0x0c: (119, 119, 119),     # Medium Grey
    0x0d: (170, 255, 102),     # Light Green
    0x0e: (0, 136, 255),       # Light Blue
    0x0f: (187, 187, 187),     # Light Grey
}

# Create gradient RGB mapping from the gradient indices
GRADIENT_RGB = {i: C64_PALETTE_RGB[color] for i, color in enumerate(GRADIENT)}

# Animation parameters
WIDTH = 32  # characters
HEIGHT = 10  # characters
FRAMES = 16  # number of animation frames
NUM_CIRCLES = 1  # Single circle
CHAR_SIZE_PIXELS = 8  # Each character is 8x8 pixels in the output PNG

# 8x8 Bayer dithering matrix for ordered dithering
BAYER_MATRIX_8x8 = [
    [ 0, 32,  8, 40,  2, 34, 10, 42],
    [48, 16, 56, 24, 50, 18, 58, 26],
    [12, 44,  4, 36, 14, 46,  6, 38],
    [60, 28, 52, 20, 62, 30, 54, 22],
    [ 3, 35, 11, 43,  1, 33,  9, 41],
    [51, 19, 59, 27, 49, 17, 57, 25],
    [15, 47,  7, 39, 13, 45,  5, 37],
    [63, 31, 55, 23, 61, 29, 53, 21]
]

class Circle:
    """Represents a single zooming circle at screen center"""
    def __init__(self):
        pass
        
    def get_position_and_size(self, frame):
        """Calculate circle position and radius for given frame"""
        # Circle stays at center of screen
        x = WIDTH / 2
        y = HEIGHT / 2
        
        # Radius grows linearly from very small to large over all frames
        min_radius = 0.5
        max_radius = 12.0
        radius = min_radius + (max_radius - min_radius) * (frame / (FRAMES - 1))
        
        return x, y, radius

def calculate_intensity(x, y, circles_data):
    """
    Calculate light intensity at position (x, y) from the circle
    Returns value 0.0 (dark) to 1.0 (bright)
    """
    # Single circle - extract position and radius
    cx, cy, radius = circles_data[0]
    
    # Calculate distance from circle center
    dx = x - cx
    dy = y - cy
    distance = math.sqrt(dx * dx + dy * dy)
    
    # Calculate intensity based on distance (inverse square law approximation)
    if distance < radius * 1.5:
        # Intensity falls off with distance
        intensity = max(0.0, 1.0 - (distance / (radius * 1.5)))
        # Square the falloff for more dramatic effect
        intensity = intensity * intensity
        return intensity
    
    return 0.0

def intensity_to_color(intensity):
    """
    Convert intensity value to gradient color index
    intensity: 0.0 to 1.0
    """
    # Map to gradient palette (9 colors)
    palette_index = int(intensity * (len(GRADIENT) - 1))
    palette_index = max(0, min(len(GRADIENT) - 1, palette_index))
    
    return GRADIENT[palette_index]

def generate_frame(frame_num, circle):
    """Generate one frame of animation"""
    # Get circle position and size for this frame
    circles_data = [circle.get_position_and_size(frame_num)]
    
    # Generate color data for each character position
    frame_data = []
    for y in range(HEIGHT):
        for x in range(WIDTH):
            # Calculate intensity from the circle
            intensity = calculate_intensity(x, y, circles_data)
            
            # Convert to color
            color = intensity_to_color(intensity)
            frame_data.append(color)
    
    return frame_data

def write_asm_data(frames_data, output_file):
    """Write frame data as ACME assembly include file"""
    with open(output_file, 'w') as f:
        f.write('; Floodlight animation data\n')
        f.write('; Single zooming circle with C64 color gradient\n')
        f.write('; {} frames, {}x{} characters ({} bytes per frame)\n\n'.format(
            len(frames_data), WIDTH, HEIGHT, WIDTH * HEIGHT))
        
        f.write('FRAME_COUNT = {}\n'.format(len(frames_data)))
        f.write('FRAME_SIZE = {}\n'.format(WIDTH * HEIGHT))
        f.write('SCREEN_WIDTH = {}\n'.format(WIDTH))
        f.write('SCREEN_HEIGHT = {}\n\n'.format(HEIGHT))
        
        f.write('animation_data:\n')
        
        for frame_idx, frame_data in enumerate(frames_data):
            f.write('; Frame {}\n'.format(frame_idx))
            
            # Write data in rows for readability
            for row in range(HEIGHT):
                row_data = frame_data[row * WIDTH:(row + 1) * WIDTH]
                f.write('!byte ')
                f.write(','.join('${:02x}'.format(c) for c in row_data))
                f.write('\n')
            
            if frame_idx < len(frames_data) - 1:
                f.write('\n')

def save_frame_as_png(frame_data, frame_num, output_dir):
    """
    Save a single frame as PNG image with dithering
    Each character is represented as an 8x8 pixel block with ordered dithering
    
    Args:
        frame_data: List of C64 color values for each character (WIDTH * HEIGHT elements)
        frame_num: Frame number for filename
        output_dir: Directory path to save the PNG file
    """
    # Create image with 8x8 pixel blocks for each character
    pixel_width = WIDTH * CHAR_SIZE_PIXELS
    pixel_height = HEIGHT * CHAR_SIZE_PIXELS
    
    # Create RGB image
    img = Image.new('RGB', (pixel_width, pixel_height))
    pixels = img.load()
    
    # Convert character colors to pixels with dithering
    for char_y in range(HEIGHT):
        for char_x in range(WIDTH):
            # Get C64 color value for this character
            char_index = char_y * WIDTH + char_x
            c64_color = frame_data[char_index]
            
            # Find the gradient index for this C64 color
            try:
                gradient_index = GRADIENT.index(c64_color)
            except ValueError:
                gradient_index = 0  # Fallback to black if color not found
            
            # Get base RGB color from C64 palette
            base_rgb = C64_PALETTE_RGB.get(c64_color, (0, 0, 0))
            
            # Determine next color in gradient for dithering
            if gradient_index < len(GRADIENT) - 1:
                next_c64_color = GRADIENT[gradient_index + 1]
                next_rgb = C64_PALETTE_RGB.get(next_c64_color, base_rgb)
            else:
                next_rgb = base_rgb
            
            # Fill 8x8 pixel block with dithered color
            for py in range(CHAR_SIZE_PIXELS):
                for px in range(CHAR_SIZE_PIXELS):
                    pixel_x = char_x * CHAR_SIZE_PIXELS + px
                    pixel_y = char_y * CHAR_SIZE_PIXELS + py
                    
                    # Apply ordered dithering using Bayer matrix
                    # If color is at max, no dithering needed
                    if gradient_index < len(GRADIENT) - 1:
                        threshold = BAYER_MATRIX_8x8[py][px]
                        # Use threshold to decide between base and next color
                        # This creates a dithered pattern
                        if threshold < 32:  # Half the matrix values are < 32
                            pixels[pixel_x, pixel_y] = next_rgb
                        else:
                            pixels[pixel_x, pixel_y] = base_rgb
                    else:
                        pixels[pixel_x, pixel_y] = base_rgb
    
    # Save image
    filename = os.path.join(output_dir, f'frame_{frame_num:04d}.png')
    img.save(filename)


def main():
    print(f"Generating floodlight animation...")
    print(f"  Screen size: {WIDTH}x{HEIGHT} characters")
    print(f"  Frames: {FRAMES}")
    print(f"  Single zooming circle")
    
    # Create single circle
    circle = Circle()
    
    # Create output directory for PNG frames
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Generate all frames
    print("Generating frames...")
    frames_data = []
    for frame in range(FRAMES):
        if frame % 10 == 0:
            print(f"  Frame {frame}/{FRAMES}...")
        frame_data = generate_frame(frame, circle)
        frames_data.append(frame_data)
        
        # Save frame as PNG
        save_frame_as_png(frame_data, frame, frames_dir)
    
    # Write output file
    output_file = 'floodlights-data.i'
    print(f"Writing data to {output_file}...")
    write_asm_data(frames_data, output_file)
    
    print(f"Done! Generated {len(frames_data)} frames")
    print(f"Total data size: {len(frames_data) * WIDTH * HEIGHT} bytes")
    print(f"PNG frames saved to: {frames_dir}/")


if __name__ == '__main__':
    main()
