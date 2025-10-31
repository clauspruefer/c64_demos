#!/usr/bin/env python3
"""
Generate C64 exploding chicken animation data
Creates a comic-style exploding chicken animation using hires mode and colodore palette
Enhanced Color Mode (ECM) with max 64 characters
Output: Animation frames in C64 assembly format
"""

import math
import sys
import os
from PIL import Image, ImageDraw

# Colodore palette (C64 color values)
# https://www.colodore.com/
COLODORE_PALETTE_RGB = {
    0x00: (0, 0, 0),           # Black
    0x01: (255, 255, 255),     # White
    0x02: (146, 74, 64),       # Red
    0x03: (132, 197, 204),     # Cyan
    0x04: (147, 81, 182),      # Purple
    0x05: (114, 177, 75),      # Green
    0x06: (72, 58, 170),       # Blue
    0x07: (213, 223, 124),     # Yellow
    0x08: (153, 105, 45),      # Orange
    0x09: (103, 82, 0),        # Brown
    0x0a: (193, 129, 120),     # Light Red
    0x0b: (96, 96, 96),        # Dark Grey
    0x0c: (138, 138, 138),     # Medium Grey
    0x0d: (179, 236, 145),     # Light Green
    0x0e: (134, 122, 222),     # Light Blue
    0x0f: (179, 179, 179),     # Light Grey
}

# Animation parameters for ECM (Enhanced Color Mode)
WIDTH = 8   # 8 characters width (max for small display)
HEIGHT = 8  # 8 characters height  
FRAMES = 16 # number of animation frames
CHAR_SIZE_PIXELS = 8  # Each character is 8x8 pixels in the output

# Colors for chicken and explosion effect
COLOR_BG = 0x00      # Black background
COLOR_CHICKEN = 0x07 # Yellow for chicken body
COLOR_BEAK = 0x08    # Orange for beak
COLOR_EYE = 0x00     # Black for eye
COLOR_EXPLOSION_1 = 0x02  # Red
COLOR_EXPLOSION_2 = 0x08  # Orange
COLOR_EXPLOSION_3 = 0x07  # Yellow
COLOR_EXPLOSION_4 = 0x01  # White (bright flash)

def draw_chicken(draw, frame, width, height):
    """Draw a simple chicken that starts whole and explodes"""
    center_x = width // 2
    center_y = height // 2
    
    if frame < 6:
        # Draw intact chicken (frames 0-5)
        # Body (yellow circle)
        size = 25 - frame * 2  # Slight vibration before explosion
        body_color = COLODORE_PALETTE_RGB[COLOR_CHICKEN]
        draw.ellipse([center_x - size, center_y - size, 
                     center_x + size, center_y + size], 
                     fill=body_color, outline=body_color)
        
        # Head (smaller yellow circle)
        head_size = 12 - frame
        draw.ellipse([center_x - head_size, center_y - 25 - frame, 
                     center_x + head_size, center_y - 15 + frame],
                     fill=body_color, outline=body_color)
        
        # Eye (black dot)
        eye_color = COLODORE_PALETTE_RGB[COLOR_EYE]
        draw.ellipse([center_x - 2, center_y - 22, 
                     center_x + 2, center_y - 18],
                     fill=eye_color, outline=eye_color)
        
        # Beak (orange triangle)
        beak_color = COLODORE_PALETTE_RGB[COLOR_BEAK]
        draw.polygon([
            (center_x + 8, center_y - 20),
            (center_x + 15, center_y - 17),
            (center_x + 8, center_y - 14)
        ], fill=beak_color, outline=beak_color)
        
    elif frame < 10:
        # Initial explosion phase (frames 6-9)
        explosion_frame = frame - 6
        
        # Draw explosion bursts
        num_bursts = 8
        for i in range(num_bursts):
            angle = (i / num_bursts) * 2 * math.pi
            distance = explosion_frame * 8
            x = center_x + int(math.cos(angle) * distance)
            y = center_y + int(math.sin(angle) * distance)
            
            # Alternate colors for comic effect
            if i % 3 == 0:
                color = COLODORE_PALETTE_RGB[COLOR_EXPLOSION_4]  # White flash
                size = 8
            elif i % 3 == 1:
                color = COLODORE_PALETTE_RGB[COLOR_EXPLOSION_1]  # Red
                size = 6
            else:
                color = COLODORE_PALETTE_RGB[COLOR_EXPLOSION_2]  # Orange
                size = 7
            
            draw.ellipse([x - size, y - size, x + size, y + size],
                        fill=color, outline=color)
        
        # Add central flash
        if explosion_frame < 2:
            flash_color = COLODORE_PALETTE_RGB[COLOR_EXPLOSION_4]
            flash_size = 15 - explosion_frame * 5
            draw.ellipse([center_x - flash_size, center_y - flash_size,
                         center_x + flash_size, center_y + flash_size],
                        fill=flash_color, outline=flash_color)
    
    else:
        # Dissipating explosion (frames 10-15)
        explosion_frame = frame - 10
        fade_factor = 1.0 - (explosion_frame / 6.0)
        
        # Draw expanding, fading explosion particles
        num_particles = 12
        for i in range(num_particles):
            angle = (i / num_particles) * 2 * math.pi
            distance = 25 + explosion_frame * 5
            x = center_x + int(math.cos(angle) * distance)
            y = center_y + int(math.sin(angle) * distance)
            
            # Use fading colors
            if fade_factor > 0.5:
                color = COLODORE_PALETTE_RGB[COLOR_EXPLOSION_2]  # Orange
            elif fade_factor > 0.2:
                color = COLODORE_PALETTE_RGB[COLOR_EXPLOSION_1]  # Red
            else:
                color = COLODORE_PALETTE_RGB[COLOR_BG]  # Fade to black
            
            size = int(4 * fade_factor)
            if size > 0:
                draw.ellipse([x - size, y - size, x + size, y + size],
                            fill=color, outline=color)

def generate_frame_image(frame_num):
    """Generate a single frame as PIL Image"""
    img_width = WIDTH * CHAR_SIZE_PIXELS
    img_height = HEIGHT * CHAR_SIZE_PIXELS
    
    # Create image with black background
    img = Image.new('RGB', (img_width, img_height), COLODORE_PALETTE_RGB[COLOR_BG])
    draw = ImageDraw.Draw(img)
    
    # Draw chicken/explosion
    draw_chicken(draw, frame_num, img_width, img_height)
    
    return img

def rgb_to_c64_color(rgb):
    """Find closest C64 color to given RGB value"""
    min_dist = float('inf')
    closest_color = 0
    
    for color_idx, color_rgb in COLODORE_PALETTE_RGB.items():
        # Calculate Euclidean distance in RGB space
        dist = sum((a - b) ** 2 for a, b in zip(rgb, color_rgb))
        if dist < min_dist:
            min_dist = dist
            closest_color = color_idx
    
    return closest_color

def image_to_c64_chars(img):
    """Convert image to C64 character/color data (simplified for ECM)"""
    # For simplicity in ECM, we'll use color RAM data
    # Each 8x8 block maps to one character with one color
    colors = []
    
    for char_y in range(HEIGHT):
        for char_x in range(WIDTH):
            # Sample the center pixel of each 8x8 character block
            pixel_x = char_x * CHAR_SIZE_PIXELS + CHAR_SIZE_PIXELS // 2
            pixel_y = char_y * CHAR_SIZE_PIXELS + CHAR_SIZE_PIXELS // 2
            
            rgb = img.getpixel((pixel_x, pixel_y))
            c64_color = rgb_to_c64_color(rgb)
            colors.append(c64_color)
    
    return colors

def generate_animation_data():
    """Generate complete animation data for all frames"""
    all_frame_data = []
    
    # Create output directory for frame images
    os.makedirs('frames', exist_ok=True)
    
    for frame in range(FRAMES):
        print(f"Generating frame {frame + 1}/{FRAMES}...")
        
        # Generate frame image
        img = generate_frame_image(frame)
        
        # Save frame as PNG for reference
        img.save(f'frames/frame_{frame:02d}.png')
        
        # Convert to C64 color data
        colors = image_to_c64_chars(img)
        all_frame_data.append(colors)
    
    return all_frame_data

def write_assembly_data(frame_data, output_file):
    """Write animation data to assembly file"""
    with open(output_file, 'w') as f:
        f.write("; Exploding chicken animation data\n")
        f.write("; Generated by generate_chicken.py\n")
        f.write(f"; {FRAMES} frames, {WIDTH}x{HEIGHT} characters each\n")
        f.write(f"; Total size: {len(frame_data)} frames * {WIDTH * HEIGHT} bytes = {len(frame_data) * WIDTH * HEIGHT} bytes\n\n")
        
        f.write("animation_data:\n")
        
        for frame_idx, colors in enumerate(frame_data):
            f.write(f"\n; Frame {frame_idx}\n")
            
            # Write colors in rows of 8 bytes for readability
            for i in range(0, len(colors), 8):
                row = colors[i:i+8]
                byte_str = ', '.join(f'${c:02x}' for c in row)
                f.write(f'!byte {byte_str}\n')

def main():
    """Main function"""
    print("Generating C64 Exploding Chicken Animation...")
    print(f"Resolution: {WIDTH}x{HEIGHT} characters")
    print(f"Frames: {FRAMES}")
    print(f"Using Colodore palette in Enhanced Color Mode")
    
    # Generate animation data
    frame_data = generate_animation_data()
    
    # Write to assembly include file
    output_file = 'chicken-data.i'
    write_assembly_data(frame_data, output_file)
    
    print(f"\nAnimation data written to {output_file}")
    print(f"Frame images saved to frames/ directory")
    print(f"Total data size: {len(frame_data) * WIDTH * HEIGHT} bytes")
    print("\nDone!")

if __name__ == '__main__':
    main()
