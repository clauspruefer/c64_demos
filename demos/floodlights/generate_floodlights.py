#!/usr/bin/env python3
"""
Generate C64 floodlight animation data
Creates 3 moving and zooming circles with shadows and gradients
Output: 32x10 character screen (320 bytes color RAM data)
"""

import math
import sys
import os
from PIL import Image

# 32-color gradient - smooth transition from black to bright
# Creating a custom gradient with 32 steps
GRADIENT = list(range(32))

# RGB values for 32-color gradient
# Smooth transition: black -> dark red/brown -> orange -> yellow -> white
GRADIENT_RGB = {
    0: (0, 0, 0),           # Black
    1: (16, 0, 0),          # Very dark red
    2: (32, 0, 0),          # Dark red
    3: (48, 0, 0),          
    4: (64, 8, 0),          
    5: (80, 16, 0),         # Dark brown
    6: (96, 24, 0),         
    7: (112, 32, 0),        
    8: (128, 40, 0),        # Brown
    9: (144, 56, 0),        
    10: (160, 72, 8),       
    11: (176, 88, 16),      # Orange-brown
    12: (192, 104, 24),     
    13: (208, 120, 32),     # Orange
    14: (224, 136, 48),     
    15: (240, 152, 64),     
    16: (255, 168, 80),     # Bright orange
    17: (255, 176, 96),     
    18: (255, 184, 112),    
    19: (255, 192, 128),    # Light orange
    20: (255, 200, 144),    
    21: (255, 208, 160),    # Orange-yellow
    22: (255, 216, 176),    
    23: (255, 224, 192),    # Light yellow
    24: (255, 232, 208),    
    25: (255, 240, 224),    
    26: (255, 244, 232),    # Near white
    27: (255, 248, 240),    
    28: (255, 250, 245),    
    29: (255, 252, 250),    
    30: (255, 254, 252),    
    31: (255, 255, 255),    # White
}

# Animation parameters
WIDTH = 32  # characters
HEIGHT = 10  # characters
FRAMES = 64  # number of animation frames (reduced for memory constraints)
NUM_CIRCLES = 3
CHAR_SIZE_PIXELS = 8  # Each character is 8x8 pixels in the output PNG

class Circle:
    """Represents a floodlight circle with position and size"""
    def __init__(self, index):
        self.index = index
        # Each circle has different frequency for variation
        self.freq_x = 0.02 + index * 0.007
        self.freq_y = 0.015 + index * 0.005
        self.freq_size = 0.025 + index * 0.008
        # Phase offset for each circle
        self.phase_x = index * math.pi * 0.66
        self.phase_y = index * math.pi * 0.44
        self.phase_size = index * math.pi * 0.33
        
    def get_position_and_size(self, frame):
        """Calculate circle position and radius for given frame"""
        # Position oscillates across the screen
        x = WIDTH / 2 + math.sin(frame * self.freq_x + self.phase_x) * (WIDTH / 3)
        y = HEIGHT / 2 + math.sin(frame * self.freq_y + self.phase_y) * (HEIGHT / 3)
        
        # Radius oscillates between min and max
        min_radius = 3.0
        max_radius = 10.0
        radius = min_radius + (max_radius - min_radius) * (0.5 + 0.5 * math.sin(frame * self.freq_size + self.phase_size))
        
        return x, y, radius

def calculate_intensity(x, y, circles_data):
    """
    Calculate combined light intensity at position (x, y) from all circles
    Returns value 0.0 (dark) to 1.0 (bright)
    """
    total_intensity = 0.0
    
    for cx, cy, radius in circles_data:
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
            total_intensity += intensity
    
    # Clamp to [0, 1]
    return min(1.0, total_intensity)

def calculate_shadow(x, y, circles_data):
    """
    Calculate shadow factor based on circle occlusion
    Returns value 0.0 (full shadow) to 1.0 (no shadow)
    """
    shadow_factor = 1.0
    
    # Simple shadow: if point is between two circles, create shadow
    if len(circles_data) >= 2:
        for i, (cx1, cy1, r1) in enumerate(circles_data):
            for cx2, cy2, r2 in circles_data[i+1:]:
                # Calculate if point is in shadow zone between circles
                dx1 = x - cx1
                dy1 = y - cy1
                dx2 = x - cx2
                dy2 = y - cy2
                
                dist1 = math.sqrt(dx1*dx1 + dy1*dy1)
                dist2 = math.sqrt(dx2*dx2 + dy2*dy2)
                
                # If close to both circles but not in their bright zones
                if dist1 > r1 * 0.7 and dist2 > r2 * 0.7:
                    # Check if between them
                    circle_dist = math.sqrt((cx1-cx2)**2 + (cy1-cy2)**2)
                    if dist1 + dist2 < circle_dist * 1.4:
                        shadow_factor *= 0.6
    
    return shadow_factor

def intensity_to_color(intensity, shadow_factor):
    """
    Convert intensity value to gradient color index
    intensity: 0.0 to 1.0
    shadow_factor: 0.0 (full shadow) to 1.0 (no shadow)
    """
    # Apply shadow
    final_intensity = intensity * shadow_factor
    
    # Map to gradient palette (32 colors)
    palette_index = int(final_intensity * (len(GRADIENT) - 1))
    palette_index = max(0, min(len(GRADIENT) - 1, palette_index))
    
    return GRADIENT[palette_index]

def generate_frame(frame_num, circles):
    """Generate one frame of animation"""
    # Get all circle positions and sizes for this frame
    circles_data = [c.get_position_and_size(frame_num) for c in circles]
    
    # Generate color data for each character position
    frame_data = []
    for y in range(HEIGHT):
        for x in range(WIDTH):
            # Calculate intensity from all circles
            intensity = calculate_intensity(x, y, circles_data)
            
            # Calculate shadow factor
            shadow = calculate_shadow(x, y, circles_data)
            
            # Convert to color
            color = intensity_to_color(intensity, shadow)
            frame_data.append(color)
    
    return frame_data

def write_asm_data(frames_data, output_file):
    """Write frame data as ACME assembly include file"""
    with open(output_file, 'w') as f:
        f.write('; Floodlight animation data\n')
        f.write('; 3 moving and zooming circles with shadows\n')
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
    
    Args:
        frame_data: List of color indices for each character (WIDTH * HEIGHT elements)
        frame_num: Frame number for filename
        output_dir: Directory path to save the PNG file
    """
    # Create image with 8x8 pixel blocks for each character
    pixel_width = WIDTH * CHAR_SIZE_PIXELS
    pixel_height = HEIGHT * CHAR_SIZE_PIXELS
    
    # Create RGB image
    img = Image.new('RGB', (pixel_width, pixel_height))
    pixels = img.load()
    
    # Convert character colors to pixels
    for char_y in range(HEIGHT):
        for char_x in range(WIDTH):
            # Get color for this character
            char_index = char_y * WIDTH + char_x
            color_index = frame_data[char_index]
            rgb_color = GRADIENT_RGB.get(color_index, (0, 0, 0))
            
            # Fill 8x8 pixel block with this color
            for py in range(CHAR_SIZE_PIXELS):
                for px in range(CHAR_SIZE_PIXELS):
                    pixel_x = char_x * CHAR_SIZE_PIXELS + px
                    pixel_y = char_y * CHAR_SIZE_PIXELS + py
                    pixels[pixel_x, pixel_y] = rgb_color
    
    # Save image
    filename = os.path.join(output_dir, f'frame_{frame_num:04d}.png')
    img.save(filename)


def main():
    print(f"Generating floodlight animation...")
    print(f"  Screen size: {WIDTH}x{HEIGHT} characters")
    print(f"  Frames: {FRAMES}")
    print(f"  Circles: {NUM_CIRCLES}")
    
    # Create circle objects
    circles = [Circle(i) for i in range(NUM_CIRCLES)]
    
    # Create output directory for PNG frames
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Generate all frames
    print("Generating frames...")
    frames_data = []
    for frame in range(FRAMES):
        if frame % 32 == 0:
            print(f"  Frame {frame}/{FRAMES}...")
        frame_data = generate_frame(frame, circles)
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
