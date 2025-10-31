#!/usr/bin/env python3
"""
Generate Cow Fart Animation for C64
Creates a 32-frame animation of a man standing behind a farting cow
with intense wind effects blowing the man's hair like a hurricane.
Output: 160x200 pixel PNG images using Colodore palette
"""

import math
import sys
import os
from PIL import Image, ImageDraw

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

# Animation parameters
WIDTH = 160   # pixels (C64 multicolor mode width)
HEIGHT = 200  # pixels (C64 standard height)
FRAMES = 32   # number of animation frames

# Color indices for objects
COLOR_BLACK = 0
COLOR_WHITE = 1
COLOR_RED = 2
COLOR_CYAN = 3
COLOR_PURPLE = 4
COLOR_GREEN = 5
COLOR_BLUE = 6
COLOR_YELLOW = 7
COLOR_ORANGE = 8
COLOR_BROWN = 9
COLOR_LIGHT_RED = 10
COLOR_DARK_GREY = 11
COLOR_MEDIUM_GREY = 12
COLOR_LIGHT_GREEN = 13
COLOR_LIGHT_BLUE = 14
COLOR_LIGHT_GREY = 15

def draw_cow(draw, frame):
    """
    Draw a cow facing left (tail on left, head on right)
    Position: right side of screen
    """
    # Cow body position (shifted right)
    cow_x = 90
    cow_y = 120
    
    # Cow body (brown rectangular)
    body_color = COLODORE_PALETTE_RGB[COLOR_BROWN]
    draw.rectangle([cow_x, cow_y, cow_x + 40, cow_y + 30], fill=body_color)
    
    # Cow head (brown, to the right)
    draw.ellipse([cow_x + 35, cow_y - 10, cow_x + 50, cow_y + 10], fill=body_color)
    
    # Cow snout (lighter brown)
    snout_color = COLODORE_PALETTE_RGB[COLOR_ORANGE]
    draw.ellipse([cow_x + 45, cow_y - 2, cow_x + 52, cow_y + 8], fill=snout_color)
    
    # Cow ears (small triangular shapes)
    draw.polygon([(cow_x + 38, cow_y - 10), (cow_x + 36, cow_y - 16), (cow_x + 42, cow_y - 10)], fill=body_color)
    draw.polygon([(cow_x + 46, cow_y - 10), (cow_x + 44, cow_y - 16), (cow_x + 50, cow_y - 10)], fill=body_color)
    
    # Cow legs (4 legs, brown)
    leg_width = 4
    draw.rectangle([cow_x + 5, cow_y + 30, cow_x + 5 + leg_width, cow_y + 45], fill=body_color)
    draw.rectangle([cow_x + 15, cow_y + 30, cow_x + 15 + leg_width, cow_y + 45], fill=body_color)
    draw.rectangle([cow_x + 25, cow_y + 30, cow_x + 25 + leg_width, cow_y + 45], fill=body_color)
    draw.rectangle([cow_x + 35, cow_y + 30, cow_x + 35 + leg_width, cow_y + 45], fill=body_color)
    
    # Cow tail (on the left, wagging slightly based on frame)
    tail_offset = int(math.sin(frame * 0.3) * 3)
    draw.line([(cow_x, cow_y + 5), (cow_x - 8, cow_y - 5 + tail_offset)], 
              fill=body_color, width=2)
    
    # Cow spots (white)
    spot_color = COLODORE_PALETTE_RGB[COLOR_WHITE]
    draw.ellipse([cow_x + 8, cow_y + 5, cow_x + 16, cow_y + 12], fill=spot_color)
    draw.ellipse([cow_x + 22, cow_y + 10, cow_x + 28, cow_y + 18], fill=spot_color)
    draw.ellipse([cow_x + 10, cow_y + 18, cow_x + 18, cow_y + 25], fill=spot_color)

def draw_man(draw, frame):
    """
    Draw a man standing behind the cow with hair blown by wind
    Position: left side of screen
    Hair gets progressively more wild with animation
    """
    # Man position (left of cow)
    man_x = 35
    man_y = 125
    
    # Body (light grey shirt)
    body_color = COLODORE_PALETTE_RGB[COLOR_LIGHT_GREY]
    draw.rectangle([man_x - 8, man_y, man_x + 8, man_y + 25], fill=body_color)
    
    # Arms (extending forward, bracing against wind)
    wind_push = min(frame * 0.3, 8)  # Arms pushed back by wind
    arm_color = COLODORE_PALETTE_RGB[COLOR_LIGHT_RED]
    # Left arm
    draw.rectangle([man_x - 12 - wind_push, man_y + 5, man_x - 8, man_y + 18], fill=arm_color)
    # Right arm
    draw.rectangle([man_x + 8, man_y + 5, man_x + 12 + wind_push, man_y + 18], fill=arm_color)
    
    # Legs (brown pants)
    leg_color = COLODORE_PALETTE_RGB[COLOR_BROWN]
    draw.rectangle([man_x - 6, man_y + 25, man_x - 2, man_y + 45], fill=leg_color)
    draw.rectangle([man_x + 2, man_y + 25, man_x + 6, man_y + 45], fill=leg_color)
    
    # Head (light red/flesh tone)
    head_color = COLODORE_PALETTE_RGB[COLOR_LIGHT_RED]
    draw.ellipse([man_x - 8, man_y - 20, man_x + 8, man_y], fill=head_color)
    
    # Eyes (black, wide with surprise)
    eye_color = COLODORE_PALETTE_RGB[COLOR_BLACK]
    draw.ellipse([man_x - 5, man_y - 12, man_x - 2, man_y - 8], fill=eye_color)
    draw.ellipse([man_x + 2, man_y - 12, man_x + 5, man_y - 8], fill=eye_color)
    
    # Mouth (open in shock)
    draw.ellipse([man_x - 3, man_y - 6, man_x + 3, man_y - 2], fill=eye_color)
    
    # Hair - blown wildly by hurricane wind!
    # Hair gets more intense and blown harder as animation progresses
    hair_color = COLODORE_PALETTE_RGB[COLOR_ORANGE]
    
    # Calculate wind intensity (increases over frames)
    wind_intensity = min(frame / FRAMES * 1.5, 1.5)
    
    # Multiple hair strands flying wildly to the left (away from cow)
    for i in range(12):
        angle_base = -math.pi * 0.3 - (i * 0.2)  # Pointing left and upward
        angle_variation = math.sin(frame * 0.4 + i * 0.5) * 0.4 * wind_intensity
        angle = angle_base + angle_variation
        
        # Hair length increases with wind
        length = 15 + wind_intensity * 12 + (i % 3) * 5
        
        # Hair starts from different points on the head
        start_angle = (i / 12) * math.pi * 0.8 - math.pi * 0.6
        start_x = man_x + int(math.cos(start_angle) * 7)
        start_y = man_y - 16 + int(math.sin(start_angle) * 7)
        
        # Calculate end point of hair strand
        end_x = start_x + int(math.cos(angle) * length)
        end_y = start_y + int(math.sin(angle) * length)
        
        # Draw hair strand with varying thickness
        thickness = 2 if i % 2 == 0 else 1
        draw.line([(start_x, start_y), (end_x, end_y)], 
                  fill=hair_color, width=thickness)

def draw_fart_cloud(draw, frame):
    """
    Draw an intense fart cloud/wind effect emanating from cow's rear
    Gets more intense as animation progresses
    """
    # Fart emanates from cow's rear (left side of cow)
    fart_x = 90
    fart_y = 130
    
    # Calculate fart intensity (ramps up in first half, stays strong)
    intensity = min(frame / (FRAMES * 0.5), 1.0)
    
    # Fart cloud colors - greenish, yellowish haze
    cloud_colors = [
        COLODORE_PALETTE_RGB[COLOR_LIGHT_GREEN],
        COLODORE_PALETTE_RGB[COLOR_YELLOW],
        COLODORE_PALETTE_RGB[COLOR_CYAN],
        COLODORE_PALETTE_RGB[COLOR_LIGHT_GREY],
    ]
    
    # Draw multiple expanding cloud puffs
    num_puffs = int(5 + intensity * 10)
    for i in range(num_puffs):
        # Puffs expand outward to the left (toward the man)
        expansion = (frame + i * 2) * 2
        offset_x = -expansion * (1 + i * 0.1)
        offset_y = math.sin(i * 0.7 + frame * 0.2) * 10 * intensity
        
        puff_x = fart_x + offset_x
        puff_y = fart_y + offset_y
        
        # Puff size increases with distance and intensity
        puff_size = 8 + i * 3 + expansion * 0.3
        
        # Choose color (cycle through cloud colors)
        color = cloud_colors[i % len(cloud_colors)]
        
        # Draw puff as ellipse (visible only if in frame bounds)
        if puff_x > -puff_size and puff_x < WIDTH + puff_size:
            draw.ellipse([
                puff_x - puff_size, puff_y - puff_size * 0.6,
                puff_x + puff_size, puff_y + puff_size * 0.6
            ], fill=color, outline=None)
    
    # Add some wind lines for extra intensity
    if intensity > 0.3:
        wind_color = COLODORE_PALETTE_RGB[COLOR_WHITE]
        for i in range(8):
            line_y = fart_y + (i - 4) * 6
            line_x_start = fart_x - 5
            line_length = 20 + frame * 1.5 + i * 3
            line_x_end = line_x_start - line_length
            
            # Wavy wind lines
            wave_offset = math.sin(frame * 0.3 + i * 0.5) * 3
            
            draw.line([
                (line_x_start, line_y),
                (line_x_end, line_y + wave_offset)
            ], fill=wind_color, width=1)

def generate_frame(frame_num):
    """
    Generate one frame of the cow fart animation
    Returns image data as PIL Image
    """
    # Create image with sky blue background
    bg_color = COLODORE_PALETTE_RGB[COLOR_LIGHT_BLUE]
    img = Image.new('RGB', (WIDTH, HEIGHT), color=bg_color)
    draw = ImageDraw.Draw(img)
    
    # Draw ground (green)
    ground_color = COLODORE_PALETTE_RGB[COLOR_GREEN]
    draw.rectangle([0, 170, WIDTH, HEIGHT], fill=ground_color)
    
    # Draw fart cloud first (behind characters)
    draw_fart_cloud(draw, frame_num)
    
    # Draw cow
    draw_cow(draw, frame_num)
    
    # Draw man with blown hair
    draw_man(draw, frame_num)
    
    return img

def main():
    print(f"Generating Cow Fart Animation...")
    print(f"  Resolution: {WIDTH}x{HEIGHT} pixels")
    print(f"  Frames: {FRAMES}")
    print(f"  Palette: Colodore C64")
    print(f"  Theme: Man behind farting cow with hurricane hair effect")
    
    # Create output directory for PNG frames
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Generate all frames
    print("Generating frames...")
    for frame in range(FRAMES):
        print(f"  Frame {frame + 1}/{FRAMES} ({100 * (frame + 1) / FRAMES:.1f}%)...")
        
        # Generate frame
        img = generate_frame(frame)
        
        # Save frame as PNG
        filename = os.path.join(frames_dir, f'frame_{frame:04d}.png')
        img.save(filename)
    
    print(f"\nDone! Generated {FRAMES} frames")
    print(f"PNG frames saved to: {frames_dir}/")
    print(f"\nTo create a video from frames, you can use ffmpeg:")
    print(f"  ffmpeg -framerate 12 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p cow_fart.mp4")
    print(f"\nTo create a looping GIF:")
    print(f"  ffmpeg -framerate 12 -i frames/frame_%04d.png -vf \"scale=320:400:flags=neighbor\" cow_fart.gif")

if __name__ == '__main__':
    main()
