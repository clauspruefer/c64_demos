#!/usr/bin/env python3
"""
Export C64 ninja eyes animation frames as PNG images.
This script renders each frame of the eye and cube animations.
"""

from PIL import Image, ImageDraw, ImageFont
import os

# C64 color palette (RGB values)
C64_COLORS = {
    0x00: (0, 0, 0),        # Black
    0x01: (255, 255, 255),  # White
    0x02: (136, 0, 0),      # Red
    0x03: (170, 255, 238),  # Cyan
    0x04: (204, 68, 204),   # Purple
    0x05: (0, 204, 85),     # Green
    0x06: (0, 0, 170),      # Blue
    0x07: (238, 238, 119),  # Yellow
    0x08: (221, 136, 85),   # Orange
    0x09: (102, 68, 0),     # Brown
    0x0a: (255, 119, 119),  # Light red
    0x0b: (51, 51, 51),     # Dark gray
    0x0c: (119, 119, 119),  # Gray
    0x0d: (170, 255, 102),  # Light green
    0x0e: (0, 136, 255),    # Light blue
    0x0f: (187, 187, 187),  # Light gray
}

# PETSCII character mappings (simplified for visualization)
PETSCII_CHARS = {
    0x20: ' ',   # Space
    0x40: '─',   # Horizontal line
    0x49: '╗',   # Upper-right corner
    0x4a: '╚',   # Lower-left corner
    0x4b: '╝',   # Lower-right corner
    0x55: '│',   # Vertical line / left edge
    0x7e: '●',   # Circle (for pupil)
    0xa0: '█',   # Block (for cube vertices)
    0xe0: '▐',   # Right half block
}

# Character size in pixels (8x8 for C64)
CHAR_WIDTH = 8
CHAR_HEIGHT = 8
SCALE = 4  # Scale factor for output images

def load_eye_data():
    """Load eye character and color data from generated file"""
    eye_chars = []
    eye_colors = []
    
    with open('eye-data.i', 'r') as f:
        lines = f.readlines()
        in_chars = False
        in_colors = False
        
        for line in lines:
            line = line.strip()
            if line.startswith('eye_chars:'):
                in_chars = True
                continue
            elif line.startswith('eye_colors:'):
                in_chars = False
                in_colors = True
                continue
            
            if line.startswith('!byte'):
                # Parse byte data
                data_str = line.split('!byte')[1].strip()
                values = [int(x.strip().replace('$', ''), 16) for x in data_str.split(',')]
                
                if in_chars:
                    eye_chars.extend(values)
                elif in_colors:
                    eye_colors.extend(values)
    
    return eye_chars, eye_colors

def load_cube_data():
    """Load cube rotation data from generated file"""
    frames = []
    
    with open('cube-rotation-data.i', 'r') as f:
        lines = f.readlines()
        
        for line in lines:
            line = line.strip()
            if line.startswith('!byte'):
                # Parse byte data
                data_str = line.split('!byte')[1].strip()
                values = [int(x.strip().replace('$', ''), 16) for x in data_str.split(',')]
                frames.append(values)
    
    return frames

def draw_character(draw, x, y, char_code, color_code, scale):
    """Draw a C64 character at the given position"""
    char_str = PETSCII_CHARS.get(char_code, '?')
    color = C64_COLORS.get(color_code, (255, 255, 255))
    
    # Draw character as a colored rectangle with text
    rect_x = x * CHAR_WIDTH * scale
    rect_y = y * CHAR_HEIGHT * scale
    rect_w = CHAR_WIDTH * scale
    rect_h = CHAR_HEIGHT * scale
    
    # Fill background
    if char_code != 0x20:  # Not a space
        draw.rectangle([rect_x, rect_y, rect_x + rect_w, rect_y + rect_h], 
                      fill=color, outline=None)
    
    # Draw character text
    if char_str != ' ':
        font_size = int(CHAR_HEIGHT * scale * 0.8)
        try:
            # Try to use a monospace font
            font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", font_size)
        except:
            font = ImageFont.load_default()
        
        text_color = (0, 0, 0) if color_code == 0x01 else color
        draw.text((rect_x + rect_w // 4, rect_y), char_str, fill=text_color, font=font)

def render_eye_frame(eye_chars, eye_colors, scale=SCALE):
    """Render a single eye frame"""
    # Eye is 8 chars wide x 3 chars tall
    width = 8 * CHAR_WIDTH * scale
    height = 3 * CHAR_HEIGHT * scale
    
    img = Image.new('RGB', (width, height), C64_COLORS[0x00])  # Black background
    draw = ImageDraw.Draw(img)
    
    # Draw each character
    for i in range(24):  # 8x3 = 24 characters
        row = i // 8
        col = i % 8
        char_code = eye_chars[i]
        color_code = eye_colors[i]
        draw_character(draw, col, row, char_code, color_code, scale)
    
    return img

def render_cube_frame(cube_data, frame_num, scale=SCALE):
    """Render a single cube rotation frame"""
    # Cube is rendered in a 3x3 character area
    width = 3 * CHAR_WIDTH * scale
    height = 3 * CHAR_HEIGHT * scale
    
    img = Image.new('RGB', (width, height), C64_COLORS[0x00])  # Black background
    draw = ImageDraw.Draw(img)
    
    # Get frame data (16 bytes = 8 vertices * 2 coordinates)
    frame_data = cube_data[frame_num]
    
    # Draw vertices
    for v in range(8):
        x = frame_data[v * 2]      # X coordinate (0-23)
        y = frame_data[v * 2 + 1]  # Y coordinate (0-7)
        
        # Convert to character grid position
        char_col = x // 8
        char_row = y // 4
        
        # Only draw if within bounds
        if char_col < 3 and char_row < 3:
            # Draw as a block character
            draw_character(draw, char_col, char_row, 0xa0, 0x0f, scale)  # Light gray block
    
    return img

def render_full_scene(eye_chars, eye_colors, cube_data, frame_num, scale=SCALE):
    """Render the full scene with both eyes and cubes"""
    # Screen is 40x25 characters
    width = 40 * CHAR_WIDTH * scale
    height = 25 * CHAR_HEIGHT * scale
    
    img = Image.new('RGB', (width, height), C64_COLORS[0x00])  # Black background
    draw = ImageDraw.Draw(img)
    
    # Draw title text at row 5, col 7
    title = "NINJA EYES - MORPHING PUPILS"
    for i, char in enumerate(title):
        if 7 + i < 40:
            char_code = ord(char)
            color_code = 0x0e  # Light blue
            # Simplified character drawing for ASCII
            rect_x = (7 + i) * CHAR_WIDTH * scale
            rect_y = 5 * CHAR_HEIGHT * scale
            rect_w = CHAR_WIDTH * scale
            rect_h = CHAR_HEIGHT * scale
            
            font_size = int(CHAR_HEIGHT * scale * 0.7)
            try:
                font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", font_size)
            except:
                font = ImageFont.load_default()
            
            draw.text((rect_x, rect_y), char, fill=C64_COLORS[color_code], font=font)
    
    # Draw left eye at row 10, col 8
    for i in range(24):
        row = 10 + (i // 8)
        col = 8 + (i % 8)
        char_code = eye_chars[i]
        color_code = eye_colors[i]
        draw_character(draw, col, row, char_code, color_code, scale)
    
    # Draw right eye at row 10, col 26
    for i in range(24):
        row = 10 + (i // 8)
        col = 26 + (i % 8)
        char_code = eye_chars[i]
        color_code = eye_colors[i]
        draw_character(draw, col, row, char_code, color_code, scale)
    
    # Draw cube in left pupil (row 11, col 10-12)
    frame_data = cube_data[frame_num]
    for v in range(8):
        x = frame_data[v * 2]
        y = frame_data[v * 2 + 1]
        
        char_col = 10 + (x // 8)
        char_row = 11 + (y // 4)
        
        if char_col < 13 and char_row < 14:
            draw_character(draw, char_col, char_row, 0xa0, 0x0f, scale)
    
    # Draw cube in right pupil (row 11, col 28-30)
    for v in range(8):
        x = frame_data[v * 2]
        y = frame_data[v * 2 + 1]
        
        char_col = 28 + (x // 8)
        char_row = 11 + (y // 4)
        
        if char_col < 31 and char_row < 14:
            draw_character(draw, char_col, char_row, 0xa0, 0x0f, scale)
    
    # Draw info text at row 22, col 7
    info = "  INSPIRED BY LAST NINJA  "
    for i, char in enumerate(info):
        if 7 + i < 40:
            char_code = ord(char)
            color_code = 0x0f  # Light gray
            rect_x = (7 + i) * CHAR_WIDTH * scale
            rect_y = 22 * CHAR_HEIGHT * scale
            
            font_size = int(CHAR_HEIGHT * scale * 0.7)
            try:
                font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", font_size)
            except:
                font = ImageFont.load_default()
            
            draw.text((rect_x, rect_y), char, fill=C64_COLORS[color_code], font=font)
    
    return img

def main():
    """Export all animation frames as PNG images"""
    print("Loading animation data...")
    eye_chars, eye_colors = load_eye_data()
    cube_data = load_cube_data()
    
    # Create output directory
    os.makedirs('frames', exist_ok=True)
    
    print("Exporting eye frame...")
    eye_img = render_eye_frame(eye_chars, eye_colors)
    eye_img.save('frames/eye_frame.png')
    print(f"  Saved: frames/eye_frame.png")
    
    print(f"\nExporting {len(cube_data)} cube rotation frames...")
    for i in range(len(cube_data)):
        cube_img = render_cube_frame(cube_data, i)
        filename = f'frames/cube_frame_{i:03d}.png'
        cube_img.save(filename)
        if i % 10 == 0:
            print(f"  Saved: {filename}")
    
    print(f"\nExporting {len(cube_data)} full scene frames...")
    for i in range(len(cube_data)):
        scene_img = render_full_scene(eye_chars, eye_colors, cube_data, i)
        filename = f'frames/scene_frame_{i:03d}.png'
        scene_img.save(filename)
        if i % 10 == 0:
            print(f"  Saved: {filename}")
    
    print(f"\n✓ Export complete!")
    print(f"  - 1 eye frame")
    print(f"  - {len(cube_data)} cube frames")
    print(f"  - {len(cube_data)} full scene frames")
    print(f"  - Total: {1 + len(cube_data) * 2} PNG files in frames/")

if __name__ == '__main__':
    main()
