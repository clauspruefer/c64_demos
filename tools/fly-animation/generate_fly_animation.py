#!/usr/bin/env python3
"""
Generate Fly Animation for C64 - inspired by "Mandibules" movie
Creates an animation of a fly approaching the camera, getting closer until only its eyes are visible
Output: 320x200 pixel PNG images using Colodore palette
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
WIDTH = 320   # pixels
HEIGHT = 200  # pixels
FRAMES = 150  # number of animation frames
BACKGROUND_COLOR = 0  # Black background
FLY_BODY_COLOR = 11  # Dark Grey for fly body
FLY_EYE_BASE_COLOR = 2  # Red for eye base
FLY_EYE_FACET_COLOR = 10  # Light Red for eye facets
FLY_WING_COLOR = 12  # Medium Grey for wings

# Fly starting position and size
INITIAL_DISTANCE = 100.0  # Arbitrary units
FINAL_DISTANCE = 1.0  # Very close to camera
INITIAL_FLY_SIZE = 0.3  # Small fly at start

def draw_hexagon(draw, center_x, center_y, radius, color):
    """Draw a hexagon to represent one facet of the compound eye"""
    points = []
    for i in range(6):
        angle = math.pi / 3 * i
        x = center_x + radius * math.cos(angle)
        y = center_y + radius * math.sin(angle)
        points.append((x, y))
    draw.polygon(points, fill=color, outline=color)

def draw_compound_eye(draw, center_x, center_y, eye_radius, facet_size):
    """
    Draw a compound eye with hexagonal facets
    The facets get smaller towards the edges for realism
    """
    # Draw base eye circle
    base_color = COLODORE_PALETTE_RGB[FLY_EYE_BASE_COLOR]
    draw.ellipse([
        center_x - eye_radius, center_y - eye_radius,
        center_x + eye_radius, center_y + eye_radius
    ], fill=base_color)
    
    # Draw hexagonal facets in a grid pattern
    facet_color = COLODORE_PALETTE_RGB[FLY_EYE_FACET_COLOR]
    
    # Number of facet rows depends on eye size
    num_rows = max(3, int(eye_radius / facet_size))
    
    for row in range(-num_rows, num_rows + 1):
        for col in range(-num_rows, num_rows + 1):
            # Hexagonal grid offset
            offset_x = col * facet_size * 1.5
            offset_y = row * facet_size * math.sqrt(3)
            if col % 2 == 1:
                offset_y += facet_size * math.sqrt(3) / 2
            
            facet_x = center_x + offset_x
            facet_y = center_y + offset_y
            
            # Only draw facets within the eye circle
            dist = math.sqrt((facet_x - center_x)**2 + (facet_y - center_y)**2)
            if dist < eye_radius - facet_size:
                # Facets get slightly smaller towards edges
                size_factor = 1.0 - (dist / eye_radius) * 0.3
                draw_hexagon(draw, facet_x, facet_y, facet_size * size_factor * 0.8, facet_color)

def draw_fly_body(draw, center_x, center_y, size):
    """Draw the fly's body (simplified ellipse)"""
    body_color = COLODORE_PALETTE_RGB[FLY_BODY_COLOR]
    body_width = size * 0.8
    body_height = size * 1.2
    
    draw.ellipse([
        center_x - body_width / 2, center_y - body_height / 2,
        center_x + body_width / 2, center_y + body_height / 2
    ], fill=body_color)

def draw_wings(draw, center_x, center_y, size, wing_angle):
    """Draw simplified fly wings"""
    wing_color = COLODORE_PALETTE_RGB[FLY_WING_COLOR]
    
    wing_length = size * 1.5
    wing_width = size * 0.8
    
    # Left wing
    left_wing_x = center_x - size * 0.3
    left_wing_y = center_y - size * 0.5
    
    # Calculate wing position based on angle (flapping)
    angle_offset = math.sin(wing_angle) * 0.3
    
    draw.ellipse([
        left_wing_x - wing_length, left_wing_y - wing_width / 2 + angle_offset * size,
        left_wing_x, left_wing_y + wing_width / 2 + angle_offset * size
    ], fill=wing_color, outline=wing_color)
    
    # Right wing
    right_wing_x = center_x + size * 0.3
    right_wing_y = center_y - size * 0.5
    
    draw.ellipse([
        right_wing_x, right_wing_y - wing_width / 2 - angle_offset * size,
        right_wing_x + wing_length, right_wing_y + wing_width / 2 - angle_offset * size
    ], fill=wing_color, outline=wing_color)

def generate_frame(frame_num):
    """
    Generate one frame of the fly animation
    The fly starts far away and approaches the camera
    Returns image data as PIL Image
    """
    # Create image with black background
    img = Image.new('RGB', (WIDTH, HEIGHT), COLODORE_PALETTE_RGB[BACKGROUND_COLOR])
    draw = ImageDraw.Draw(img)
    
    # Calculate fly's distance from camera (exponential approach)
    # Using exponential for more dramatic effect as fly gets very close
    progress = frame_num / FRAMES
    distance = INITIAL_DISTANCE * math.exp(-5 * progress)
    
    # Calculate fly size based on distance (inverse square law-ish for perspective)
    fly_size = INITIAL_FLY_SIZE * (INITIAL_DISTANCE / distance)
    
    # Center of screen
    center_x = WIDTH / 2
    center_y = HEIGHT / 2
    
    # Add slight wobble to make flight more realistic
    wobble_x = math.sin(frame_num * 0.3) * 5 * (1 - progress)
    wobble_y = math.cos(frame_num * 0.2) * 3 * (1 - progress)
    
    fly_x = center_x + wobble_x
    fly_y = center_y + wobble_y
    
    # Wing flapping animation
    wing_angle = frame_num * 0.5
    
    # Draw different parts based on distance
    if fly_size < 30:
        # Fly is far away - draw simplified version
        draw_wings(draw, fly_x, fly_y, fly_size, wing_angle)
        draw_fly_body(draw, fly_x, fly_y, fly_size)
        
        # Draw simple eyes
        eye_radius = fly_size * 0.4
        eye_separation = fly_size * 0.6
        
        eye_color = COLODORE_PALETTE_RGB[FLY_EYE_BASE_COLOR]
        
        # Left eye
        draw.ellipse([
            fly_x - eye_separation / 2 - eye_radius, fly_y - fly_size * 0.3 - eye_radius,
            fly_x - eye_separation / 2 + eye_radius, fly_y - fly_size * 0.3 + eye_radius
        ], fill=eye_color)
        
        # Right eye
        draw.ellipse([
            fly_x + eye_separation / 2 - eye_radius, fly_y - fly_size * 0.3 - eye_radius,
            fly_x + eye_separation / 2 + eye_radius, fly_y - fly_size * 0.3 + eye_radius
        ], fill=eye_color)
    else:
        # Fly is close - draw detailed compound eyes
        # When very close, mostly show the eyes
        if fly_size > 80:
            # Only eyes visible
            eye_radius = min(fly_size * 0.8, WIDTH * 0.45)
            eye_separation = fly_size * 0.5
            facet_size = max(2, eye_radius / 15)
            
            # Left eye
            draw_compound_eye(draw, fly_x - eye_separation / 2, fly_y, eye_radius, facet_size)
            
            # Right eye
            draw_compound_eye(draw, fly_x + eye_separation / 2, fly_y, eye_radius, facet_size)
        else:
            # Medium distance - show body and detailed eyes
            draw_fly_body(draw, fly_x, fly_y, fly_size)
            
            eye_radius = fly_size * 0.5
            eye_separation = fly_size * 0.6
            facet_size = max(1.5, eye_radius / 12)
            
            # Left eye
            draw_compound_eye(draw, fly_x - eye_separation / 2, fly_y - fly_size * 0.2, eye_radius, facet_size)
            
            # Right eye
            draw_compound_eye(draw, fly_x + eye_separation / 2, fly_y - fly_size * 0.2, eye_radius, facet_size)
    
    return img

def main():
    print(f"Generating Fly Animation (inspired by 'Mandibules')...")
    print(f"  Resolution: {WIDTH}x{HEIGHT} pixels")
    print(f"  Frames: {FRAMES}")
    print(f"  Animation: Fly approaching camera until eyes fill screen")
    
    # Create output directory for PNG frames
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Generate all frames
    print("Generating frames...")
    for frame in range(FRAMES):
        if frame % 10 == 0:
            print(f"  Frame {frame}/{FRAMES} ({100*frame/FRAMES:.1f}%)...")
        
        # Generate frame
        img = generate_frame(frame)
        
        # Save frame as PNG
        filename = os.path.join(frames_dir, f'frame_{frame:04d}.png')
        img.save(filename)
    
    print(f"Done! Generated {FRAMES} frames")
    print(f"PNG frames saved to: {frames_dir}/")
    print(f"\nTo create a video from frames, you can use ffmpeg:")
    print(f"  ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p fly_animation.mp4")

if __name__ == '__main__':
    main()
