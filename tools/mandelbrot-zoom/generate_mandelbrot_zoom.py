#!/usr/bin/env python3
"""
Generate Mandelbrot endless zoom animation for C64
Creates a 4000-frame endless zoom into the Mandelbrot set
Output: 320x200 pixel PNG images using Colodore palette
"""

import math
import sys
import os
from PIL import Image

# Colodore palette - a more accurate C64 color palette
# Based on Colodore palette by Pepto: https://www.colodore.com/
COLODORE_PALETTE_RGB = [
    (0x00, 0x00, 0x00),  # 0: Black
    (0xFF, 0xFF, 0xFF),  # 1: White
    (0x81, 0x33, 0x38),  # 2: Red
    (0x75, 0xCE, 0xC8),  # 3: Cyan
    (0x8E, 0x3C, 0x97),  # 4: Purple
    (0x56, 0xAC, 0x4D),  # 5: Green
    (0x2E, 0x2C, 0x9B),  # 6: Blue
    (0xED, 0xF1, 0x71),  # 7: Yellow
    (0x8E, 0x50, 0x29),  # 8: Orange
    (0x55, 0x38, 0x00),  # 9: Brown
    (0xC4, 0x6C, 0x71),  # 10: Light Red
    (0x4A, 0x4A, 0x4A),  # 11: Dark Grey
    (0x7B, 0x7B, 0x7B),  # 12: Medium Grey
    (0xA9, 0xFF, 0x9F),  # 13: Light Green
    (0x70, 0x6D, 0xEB),  # 14: Light Blue
    (0xB2, 0xB2, 0xB2),  # 15: Light Grey
]

# Create a palette for Mandelbrot rendering
# Smooth gradient from dark to light
MANDELBROT_PALETTE = [
    0x00,  # Black
    0x06,  # Dark Blue
    0x0b,  # Dark Grey
    0x04,  # Violette (Purple)
    0x0e,  # Light Blue
    0x0a,  # Light Red
    0x0f,  # Light Grey
    0x0d,  # Light Green
    0x01,  # White
]

# Animation parameters
WIDTH = 320   # pixels
HEIGHT = 200  # pixels
FRAMES = 4000  # number of animation frames
MAX_ITER = 256  # maximum iterations for Mandelbrot calculation

# Zoom animation parameters
ZOOM_FACTOR = 1.02  # Zoom factor per frame (smaller = slower zoom)
# Interesting point in the Mandelbrot set to zoom into
# This is a classic zoom target near the "seahorse valley"
CENTER_X = -0.743643887037151
CENTER_Y = 0.131825904205330

def mandelbrot(c_real, c_imag, max_iter):
    """
    Calculate Mandelbrot set membership
    Returns number of iterations before escape (or max_iter if it doesn't escape)
    """
    z_real = 0.0
    z_imag = 0.0
    
    for i in range(max_iter):
        # Check if we've escaped (|z| > 2)
        if z_real * z_real + z_imag * z_imag > 4.0:
            return i
        
        # z = z^2 + c
        z_real_new = z_real * z_real - z_imag * z_imag + c_real
        z_imag = 2.0 * z_real * z_imag + c_imag
        z_real = z_real_new
    
    return max_iter

def iteration_to_color(iteration, max_iter):
    """
    Convert iteration count to palette color index
    Uses smooth coloring for better gradients
    """
    if iteration == max_iter:
        return 0  # Black for points in the set
    
    # Smooth coloring with logarithmic mapping
    # This creates nice smooth color gradients
    smooth_iter = iteration + 1 - math.log(math.log(2)) / math.log(2)
    
    # Map to palette (cycling through colors)
    palette_index = int(smooth_iter * 2) % len(MANDELBROT_PALETTE)
    
    return MANDELBROT_PALETTE[palette_index]

def generate_frame(frame_num):
    """
    Generate one frame of the Mandelbrot zoom animation
    Returns image data as PIL Image
    """
    # Calculate zoom level for this frame
    zoom = ZOOM_FACTOR ** frame_num
    
    # Calculate the scale (size of the complex plane window)
    # Start with a view that shows the full Mandelbrot set
    initial_scale = 3.0
    scale = initial_scale / zoom
    
    # Create image
    img = Image.new('RGB', (WIDTH, HEIGHT))
    pixels = img.load()
    
    # Render Mandelbrot set
    for py in range(HEIGHT):
        for px in range(WIDTH):
            # Map pixel coordinates to complex plane
            # Center the zoom on our target point
            x_ratio = (px - WIDTH / 2) / (WIDTH / 2)
            y_ratio = (py - HEIGHT / 2) / (HEIGHT / 2)
            
            c_real = CENTER_X + x_ratio * scale * (WIDTH / HEIGHT)
            c_imag = CENTER_Y + y_ratio * scale
            
            # Calculate Mandelbrot iteration
            iteration = mandelbrot(c_real, c_imag, MAX_ITER)
            
            # Convert to color
            color_index = iteration_to_color(iteration, MAX_ITER)
            rgb = COLODORE_PALETTE_RGB[color_index]
            
            pixels[px, py] = rgb
    
    return img

def main():
    print(f"Generating Mandelbrot endless zoom animation...")
    print(f"  Resolution: {WIDTH}x{HEIGHT} pixels")
    print(f"  Frames: {FRAMES}")
    print(f"  Zoom center: ({CENTER_X}, {CENTER_Y})")
    print(f"  Zoom factor per frame: {ZOOM_FACTOR}")
    print(f"  Final zoom level: {ZOOM_FACTOR ** FRAMES:.2e}x")
    
    # Create output directory for PNG frames
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Generate all frames
    print("Generating frames...")
    for frame in range(FRAMES):
        if frame % 100 == 0:
            print(f"  Frame {frame}/{FRAMES} ({100*frame/FRAMES:.1f}%)...")
        
        # Generate frame
        img = generate_frame(frame)
        
        # Save frame as PNG
        filename = os.path.join(frames_dir, f'frame_{frame:04d}.png')
        img.save(filename)
    
    print(f"Done! Generated {FRAMES} frames")
    print(f"PNG frames saved to: {frames_dir}/")
    print(f"\nTo create a video from frames, you can use ffmpeg:")
    print(f"  ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p mandelbrot_zoom.mp4")

if __name__ == '__main__':
    main()
