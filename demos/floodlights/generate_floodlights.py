#!/usr/bin/env python3
"""
Generate C64 floodlight animation data
Creates 3 moving and zooming circles with shadows and gradients
Output: 32x10 character screen (320 bytes color RAM data)
"""

import math
import sys

# C64 Pepto palette - optimized for gradients
# Order: black -> dark colors -> medium -> bright
PEPTO_GRADIENT = [
    0x00,  # 0 - Black
    0x0b,  # 1 - Dark grey  
    0x0c,  # 2 - Medium grey
    0x02,  # 3 - Red (shadow edge)
    0x0f,  # 4 - Light grey
    0x09,  # 5 - Brown (warm shadow)
    0x08,  # 6 - Orange (medium light)
    0x0a,  # 7 - Light red (bright edge)
    0x07,  # 8 - Yellow (bright center)
    0x01,  # 9 - White (hotspot)
]

# Animation parameters
WIDTH = 32  # characters
HEIGHT = 10  # characters
FRAMES = 64  # number of animation frames (reduced for memory constraints)
NUM_CIRCLES = 3

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
    Convert intensity value to C64 color index
    intensity: 0.0 to 1.0
    shadow_factor: 0.0 (full shadow) to 1.0 (no shadow)
    """
    # Apply shadow
    final_intensity = intensity * shadow_factor
    
    # Map to gradient palette
    palette_index = int(final_intensity * (len(PEPTO_GRADIENT) - 1))
    palette_index = max(0, min(len(PEPTO_GRADIENT) - 1, palette_index))
    
    return PEPTO_GRADIENT[palette_index]

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

def main():
    print(f"Generating floodlight animation...")
    print(f"  Screen size: {WIDTH}x{HEIGHT} characters")
    print(f"  Frames: {FRAMES}")
    print(f"  Circles: {NUM_CIRCLES}")
    
    # Create circle objects
    circles = [Circle(i) for i in range(NUM_CIRCLES)]
    
    # Generate all frames
    print("Generating frames...")
    frames_data = []
    for frame in range(FRAMES):
        if frame % 32 == 0:
            print(f"  Frame {frame}/{FRAMES}...")
        frame_data = generate_frame(frame, circles)
        frames_data.append(frame_data)
    
    # Write output file
    output_file = 'floodlights-data.i'
    print(f"Writing data to {output_file}...")
    write_asm_data(frames_data, output_file)
    
    print(f"Done! Generated {len(frames_data)} frames")
    print(f"Total data size: {len(frames_data) * WIDTH * HEIGHT} bytes")

if __name__ == '__main__':
    main()
