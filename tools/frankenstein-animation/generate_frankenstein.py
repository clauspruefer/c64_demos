#!/usr/bin/env python3
"""
Generate C64 Frankenstein animation data
Creates a classic Frankenstein monster face animation in black and white
Output: 40x25 character screen (1000 bytes color RAM data)
"""

import math
import sys
import os
from PIL import Image, ImageDraw

# Black and White C64 palette (using only black, greys, and white)
# This creates the classic black and white look
BLACK = 0x00       # Black
DARK_GREY = 0x0b   # Dark Grey
MED_GREY = 0x0c    # Medium Grey  
LIGHT_GREY = 0x0f  # Light Grey
WHITE = 0x01       # White

# RGB values for C64 colors (Pepto palette)
C64_PALETTE_RGB = {
    0x00: (0, 0, 0),           # Black
    0x01: (255, 255, 255),     # White
    0x0b: (51, 51, 51),        # Dark Grey
    0x0c: (119, 119, 119),     # Medium Grey
    0x0f: (187, 187, 187),     # Light Grey
}

# Animation parameters
WIDTH = 40   # characters (full C64 screen width)
HEIGHT = 25  # characters (full C64 screen)
FRAMES = 60  # number of animation frames
CHAR_SIZE_PIXELS = 8  # Each character is 8x8 pixels in the output PNG

def draw_frankenstein_face(frame_num):
    """
    Draw a classic Frankenstein monster face
    Returns a 2D array of grayscale values (0.0 = black, 1.0 = white)
    """
    # Create grayscale image
    img_width = WIDTH
    img_height = HEIGHT
    face = [[0.0 for _ in range(img_width)] for _ in range(img_height)]
    
    # Animation phase for effects (0.0 to 1.0)
    phase = (frame_num % FRAMES) / FRAMES
    
    # Head dimensions (in character coordinates)
    head_center_x = WIDTH // 2
    head_center_y = HEIGHT // 2
    head_width = 16
    head_height = 18
    
    # Draw basic head shape (rectangular with flat top)
    for y in range(HEIGHT):
        for x in range(WIDTH):
            dx = abs(x - head_center_x)
            dy = y - head_center_y
            
            # Main head rectangle
            if (dx < head_width // 2 and 
                dy > -head_height // 2 and 
                dy < head_height // 2):
                
                # Base skin tone (light grey)
                face[y][x] = 0.7
                
                # Add some shading on the sides
                if dx > head_width // 2 - 2:
                    face[y][x] = 0.5
    
    # Draw flat top of head (darker)
    top_y = head_center_y - head_height // 2
    for y in range(max(0, top_y - 2), min(HEIGHT, top_y + 2)):
        for x in range(head_center_x - head_width // 2, 
                      head_center_x + head_width // 2):
            if 0 <= x < WIDTH and 0 <= y < HEIGHT:
                face[y][x] = 0.3
    
    # Draw neck bolts (iconic Frankenstein feature)
    bolt_y = head_center_y + 3
    # Left bolt
    for by in range(bolt_y - 1, bolt_y + 2):
        for bx in range(head_center_x - head_width // 2 - 1, 
                       head_center_x - head_width // 2 + 2):
            if 0 <= bx < WIDTH and 0 <= by < HEIGHT:
                face[by][bx] = 0.4
    
    # Right bolt
    for by in range(bolt_y - 1, bolt_y + 2):
        for bx in range(head_center_x + head_width // 2 - 1, 
                       head_center_x + head_width // 2 + 2):
            if 0 <= bx < WIDTH and 0 <= by < HEIGHT:
                face[by][bx] = 0.4
    
    # Draw eyes (dark, square-ish)
    eye_y = head_center_y - 2
    eye_offset = 4
    
    # Blinking effect based on animation phase
    eye_open = 1.0
    if phase > 0.45 and phase < 0.55:
        eye_open = 0.0  # Eyes closed
    elif phase > 0.42 and phase < 0.45:
        eye_open = (phase - 0.42) / 0.03  # Closing
    elif phase > 0.55 and phase < 0.58:
        eye_open = 1.0 - (phase - 0.55) / 0.03  # Opening
    
    eye_height = max(1, int(3 * eye_open))
    
    # Left eye
    for ey in range(eye_y - eye_height // 2, eye_y + eye_height // 2 + 1):
        for ex in range(head_center_x - eye_offset - 2, 
                       head_center_x - eye_offset + 1):
            if 0 <= ex < WIDTH and 0 <= ey < HEIGHT:
                face[ey][ex] = 0.1  # Very dark
    
    # Right eye
    for ey in range(eye_y - eye_height // 2, eye_y + eye_height // 2 + 1):
        for ex in range(head_center_x + eye_offset - 1, 
                       head_center_x + eye_offset + 2):
            if 0 <= ex < WIDTH and 0 <= ey < HEIGHT:
                face[ey][ex] = 0.1  # Very dark
    
    # Draw heavy eyebrows (thick, dark)
    brow_y = eye_y - 3
    for by in range(brow_y, brow_y + 2):
        # Left brow
        for bx in range(head_center_x - eye_offset - 3, 
                       head_center_x - eye_offset + 2):
            if 0 <= bx < WIDTH and 0 <= by < HEIGHT:
                face[by][bx] = 0.2
        
        # Right brow  
        for bx in range(head_center_x + eye_offset - 2, 
                       head_center_x + eye_offset + 3):
            if 0 <= bx < WIDTH and 0 <= by < HEIGHT:
                face[by][bx] = 0.2
    
    # Draw nose (simple rectangular)
    nose_y = head_center_y
    for ny in range(nose_y - 1, nose_y + 2):
        for nx in range(head_center_x - 1, head_center_x + 2):
            if 0 <= nx < WIDTH and 0 <= ny < HEIGHT:
                face[ny][nx] = 0.5
    
    # Draw mouth (straight line, slightly frowning)
    mouth_y = head_center_y + 4
    for mx in range(head_center_x - 5, head_center_x + 6):
        if 0 <= mx < WIDTH and 0 <= mouth_y < HEIGHT:
            face[mouth_y][mx] = 0.2
    
    # Add scar on forehead (iconic feature)
    scar_y = head_center_y - 6
    scar_start_x = head_center_x - 2
    for i in range(7):
        sx = scar_start_x + i
        sy = scar_y + (1 if i % 2 == 0 else 0)
        if 0 <= sx < WIDTH and 0 <= sy < HEIGHT:
            face[sy][sx] = 0.3
    
    # Add stitches on scar
    for i in range(0, 7, 2):
        sx = scar_start_x + i
        # Stitch above
        if 0 <= sx < WIDTH and scar_y - 1 >= 0:
            face[scar_y - 1][sx] = 0.3
        # Stitch below
        if 0 <= sx < WIDTH and scar_y + 2 < HEIGHT:
            face[scar_y + 2][sx] = 0.3
    
    # Add subtle breathing/pulsing effect
    pulse = math.sin(phase * 2 * math.pi) * 0.05
    for y in range(HEIGHT):
        for x in range(WIDTH):
            if face[y][x] > 0.3:  # Only affect lighter areas
                face[y][x] = min(1.0, face[y][x] + pulse)
    
    return face

def grayscale_to_c64_color(value):
    """
    Convert grayscale value (0.0 to 1.0) to C64 black/white palette
    """
    if value < 0.2:
        return BLACK
    elif value < 0.4:
        return DARK_GREY
    elif value < 0.6:
        return MED_GREY
    elif value < 0.8:
        return LIGHT_GREY
    else:
        return WHITE

def generate_frame(frame_num):
    """Generate one frame of animation"""
    face = draw_frankenstein_face(frame_num)
    
    # Convert to C64 colors
    frame_data = []
    for y in range(HEIGHT):
        for x in range(WIDTH):
            color = grayscale_to_c64_color(face[y][x])
            frame_data.append(color)
    
    return frame_data

def write_asm_data(frames_data, output_file):
    """Write frame data as ACME assembly include file"""
    with open(output_file, 'w') as f:
        f.write('; Frankenstein animation data\n')
        f.write('; Classic Frankenstein monster face in black and white\n')
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
    Save a single frame as PNG image
    Each character is represented as an 8x8 pixel block
    """
    pixel_width = WIDTH * CHAR_SIZE_PIXELS
    pixel_height = HEIGHT * CHAR_SIZE_PIXELS
    
    # Create RGB image
    img = Image.new('RGB', (pixel_width, pixel_height))
    pixels = img.load()
    
    # Convert character colors to pixels
    for char_y in range(HEIGHT):
        for char_x in range(WIDTH):
            char_index = char_y * WIDTH + char_x
            c64_color = frame_data[char_index]
            rgb = C64_PALETTE_RGB.get(c64_color, (0, 0, 0))
            
            # Fill 8x8 pixel block with solid color
            for py in range(CHAR_SIZE_PIXELS):
                for px in range(CHAR_SIZE_PIXELS):
                    pixel_x = char_x * CHAR_SIZE_PIXELS + px
                    pixel_y = char_y * CHAR_SIZE_PIXELS + py
                    pixels[pixel_x, pixel_y] = rgb
    
    # Save image
    filename = os.path.join(output_dir, f'frame_{frame_num:04d}.png')
    img.save(filename)

def main():
    print(f"Generating Frankenstein animation...")
    print(f"  Screen size: {WIDTH}x{HEIGHT} characters")
    print(f"  Frames: {FRAMES}")
    print(f"  Black and white palette")
    
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
        frame_data = generate_frame(frame)
        frames_data.append(frame_data)
        
        # Save frame as PNG
        save_frame_as_png(frame_data, frame, frames_dir)
    
    # Write output file
    output_file = 'frankenstein-data.i'
    print(f"Writing data to {output_file}...")
    write_asm_data(frames_data, output_file)
    
    print(f"Done! Generated {len(frames_data)} frames")
    print(f"Total data size: {len(frames_data) * WIDTH * HEIGHT} bytes")
    print(f"PNG frames saved to: {frames_dir}/")

if __name__ == '__main__':
    main()
