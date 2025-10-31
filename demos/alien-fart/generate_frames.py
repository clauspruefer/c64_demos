#!/usr/bin/env python3
"""
Generate PNG frames for C64 Alien Fart animation
Reads sprite data from main.asm and renders each frame as a PNG image
"""

import os
import sys
import re
from PIL import Image, ImageDraw

# C64 screen dimensions in pixels (visible area)
SCREEN_WIDTH = 320
SCREEN_HEIGHT = 200

# Sprite size
SPRITE_WIDTH = 24
SPRITE_HEIGHT = 21

# C64 color palette (Pepto palette)
C64_PALETTE = {
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

# Sprite colors from the assembly code
SPRITE_COLORS = {
    'alien': 0x0a,      # Red
    'saucer': 0x0c,     # Gray
    'donut': 0x07,      # Yellow
    'man': 0x0f,        # Light gray
    'hamburger': 0x08,  # Orange
}

# Background and border colors
BG_COLOR = 0x06         # Blue background (sky)
BORDER_COLOR = 0x0e     # Light blue border
FART_COLOR = 0x05       # Green for fart effect


def parse_sprite_data(asm_file):
    """Parse sprite data from assembly file"""
    sprites = {}
    current_sprite = None
    sprite_data = []
    
    with open(asm_file, 'r') as f:
        lines = f.readlines()
    
    sprite_names = ['alien', 'saucer', 'donut', 'man', 'hamburger']
    sprite_idx = 0
    in_sprite = False
    
    for line in lines:
        # Check for sprite data start
        if 'Sprite 0: Alien' in line or 'alien_sprite:' in line:
            current_sprite = 'alien'
            sprite_data = []
            in_sprite = True
        elif 'Sprite 1: Flying Saucer' in line or 'saucer_sprite:' in line:
            current_sprite = 'saucer'
            sprite_data = []
            in_sprite = True
        elif 'Sprite 2: Donut' in line or 'donut_sprite:' in line:
            current_sprite = 'donut'
            sprite_data = []
            in_sprite = True
        elif 'Sprite 3: Man' in line or 'man_sprite:' in line:
            current_sprite = 'man'
            sprite_data = []
            in_sprite = True
        elif 'Sprite 4: Hamburger' in line or 'hamburger_sprite:' in line:
            current_sprite = 'hamburger'
            sprite_data = []
            in_sprite = True
        
        # Parse sprite data lines
        if in_sprite and '!byte' in line:
            # Extract binary patterns
            patterns = re.findall(r'%([01]{8})', line)
            if patterns:
                row = []
                for pattern in patterns:
                    byte = int(pattern, 2)
                    row.append(byte)
                sprite_data.append(row)
                
                # Check if we have all 21 rows
                if len(sprite_data) == 21:
                    sprites[current_sprite] = sprite_data
                    in_sprite = False
                    current_sprite = None
    
    return sprites


def render_sprite(img, draw, sprite_data, x, y, color):
    """Render a sprite on the image at given position"""
    color_rgb = C64_PALETTE[color]
    
    for row_idx, row in enumerate(sprite_data):
        for byte_idx, byte_val in enumerate(row):
            for bit_idx in range(8):
                if byte_val & (1 << (7 - bit_idx)):
                    pixel_x = x + byte_idx * 8 + bit_idx
                    pixel_y = y + row_idx
                    if 0 <= pixel_x < SCREEN_WIDTH and 0 <= pixel_y < SCREEN_HEIGHT:
                        draw.point((pixel_x, pixel_y), fill=color_rgb)


def generate_frame(sprites, frame_num, total_frames):
    """Generate a single animation frame"""
    # Determine animation state based on frame number
    # State 0: 0-59 (alien eating)
    # State 1: 60-179 (man walking)
    # State 2: 180-209 (alien flies and farts)
    # State 3: 210-269 (hamburger flying)
    
    if frame_num < 60:
        state = 0
        state_frame = frame_num
    elif frame_num < 180:
        state = 1
        state_frame = frame_num - 60
    elif frame_num < 210:
        state = 2
        state_frame = frame_num - 180
    else:
        state = 3
        state_frame = frame_num - 210
    
    # Create image with background color
    border_color = FART_COLOR if (state == 2 and 20 <= state_frame < 25) else BORDER_COLOR
    img = Image.new('RGB', (SCREEN_WIDTH, SCREEN_HEIGHT), C64_PALETTE[BG_COLOR])
    draw = ImageDraw.Draw(img)
    
    # Initial positions
    alien_x = 100
    man_x = 24
    
    # Calculate sprite positions based on state
    positions = {}
    
    if state == 0:
        # State 0: Alien eating donuts
        positions['alien'] = (alien_x, 100)
        positions['saucer'] = (alien_x, 100)
        # Donut bounces
        donut_offset = 10 if (state_frame & 0x08) else 5
        positions['donut'] = (alien_x + donut_offset, 90)
        positions['man'] = (man_x, 200)
        positions['hamburger'] = (man_x + 5, 190)
    
    elif state == 1:
        # State 1: Man walking
        man_x_pos = man_x + state_frame
        positions['alien'] = (alien_x, 100)
        positions['saucer'] = (alien_x, 100)
        positions['donut'] = (alien_x + 10, 90)
        positions['man'] = (man_x_pos, 200)
        positions['hamburger'] = (man_x_pos + 5, 190)
    
    elif state == 2:
        # State 2: Alien flies down
        alien_x_pos = alien_x + (state_frame // 4)
        alien_y_pos = 100 + (state_frame * 2)
        positions['alien'] = (alien_x_pos, alien_y_pos)
        positions['saucer'] = (alien_x_pos, alien_y_pos)
        positions['donut'] = (alien_x_pos + 10, alien_y_pos - 10)
        man_x_pos = man_x + 120  # Man has walked to middle
        positions['man'] = (man_x_pos, 200)
        positions['hamburger'] = (man_x_pos + 5, 190)
    
    else:
        # State 3: Hamburger flying
        alien_x_pos = 100 + 7
        alien_y_pos = 160
        positions['alien'] = (alien_x_pos, alien_y_pos)
        positions['saucer'] = (alien_x_pos, alien_y_pos)
        positions['donut'] = (alien_x_pos + 10, alien_y_pos - 10)
        man_x_pos = man_x + 120
        positions['man'] = (man_x_pos, 200)
        
        # Hamburger parabolic trajectory
        burger_x = (man_x_pos + 5) + (state_frame // 2)
        if state_frame < 30:
            # Going up
            burger_y = 190 - (state_frame * 3)
        else:
            # Going down
            burger_y = 190 - 90 + ((state_frame - 30) * 3)
        positions['hamburger'] = (burger_x, burger_y)
    
    # Render sprites in order (back to front)
    render_sprite(img, draw, sprites['saucer'], positions['saucer'][0], positions['saucer'][1], SPRITE_COLORS['saucer'])
    render_sprite(img, draw, sprites['alien'], positions['alien'][0], positions['alien'][1], SPRITE_COLORS['alien'])
    render_sprite(img, draw, sprites['donut'], positions['donut'][0], positions['donut'][1], SPRITE_COLORS['donut'])
    render_sprite(img, draw, sprites['man'], positions['man'][0], positions['man'][1], SPRITE_COLORS['man'])
    render_sprite(img, draw, sprites['hamburger'], positions['hamburger'][0], positions['hamburger'][1], SPRITE_COLORS['hamburger'])
    
    # Add fart effect as text/visual indicator during state 2
    if state == 2 and 20 <= state_frame < 25:
        # Draw a simple visual fart cloud
        fart_x = positions['alien'][0] - 10
        fart_y = positions['alien'][1] + 10
        for i in range(3):
            draw.ellipse([fart_x - 5 + i*4, fart_y - 5 + i*3, fart_x + 5 + i*4, fart_y + 5 + i*3], 
                        fill=C64_PALETTE[FART_COLOR])
    
    return img


def main():
    # Get the directory of this script
    script_dir = os.path.dirname(os.path.abspath(__file__))
    asm_file = os.path.join(script_dir, 'main.asm')
    output_dir = os.path.join(script_dir, 'frames')
    
    # Create output directory
    os.makedirs(output_dir, exist_ok=True)
    
    print("Parsing sprite data from main.asm...")
    sprites = parse_sprite_data(asm_file)
    
    if len(sprites) != 5:
        print(f"Error: Expected 5 sprites, found {len(sprites)}")
        print(f"Found sprites: {list(sprites.keys())}")
        return 1
    
    print(f"Found {len(sprites)} sprites: {', '.join(sprites.keys())}")
    
    # Generate all frames (270 total: 60 + 120 + 30 + 60)
    total_frames = 270
    print(f"\nGenerating {total_frames} frames...")
    
    for frame_num in range(total_frames):
        img = generate_frame(sprites, frame_num, total_frames)
        output_file = os.path.join(output_dir, f'frame_{frame_num:04d}.png')
        img.save(output_file)
        
        if (frame_num + 1) % 30 == 0:
            print(f"  Generated {frame_num + 1}/{total_frames} frames")
    
    print(f"\nAll frames saved to: {output_dir}")
    print(f"Total: {total_frames} PNG files")
    
    return 0


if __name__ == '__main__':
    sys.exit(main())
