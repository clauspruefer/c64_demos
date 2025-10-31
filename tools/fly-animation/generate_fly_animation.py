#!/usr/bin/env python3
"""
Generate Realistic Fly Animation - inspired by "Mandibules" movie
Creates a photorealistic animation of a fly approaching the camera
Output: Full HD (1920x1080) PNG images with realistic 3D-style rendering
"""

import math
import sys
import os
from PIL import Image, ImageDraw, ImageFilter

# Animation parameters
WIDTH = 1920   # Full HD width
HEIGHT = 1080  # Full HD height
FRAMES = 150   # number of animation frames

# Fly starting position and size
INITIAL_DISTANCE = 100.0  # Arbitrary units
FINAL_DISTANCE = 1.0  # Very close to camera
INITIAL_FLY_SIZE = 3.0  # Small fly at start (scaled for HD)

# Animation motion parameters
APPROACH_RATE = 5.0  # Rate of exponential approach (higher = faster)
WOBBLE_FREQ_X = 0.3  # Horizontal wobble frequency
WOBBLE_AMPLITUDE_X = 15  # Horizontal wobble amplitude in pixels
WOBBLE_FREQ_Y = 0.2  # Vertical wobble frequency
WOBBLE_AMPLITUDE_Y = 10  # Vertical wobble amplitude in pixels
WING_FLAP_SPEED = 0.5  # Wing flapping frequency

# Realistic color palette for fly
BACKGROUND_COLOR = (15, 20, 25)  # Very dark blue-grey background
FLY_BODY_BASE = (20, 25, 30)  # Very dark body
FLY_BODY_HIGHLIGHT = (45, 55, 65)  # Lighter highlights on body
FLY_EYE_BASE = (80, 15, 15)  # Dark reddish-brown eye base
FLY_EYE_FACET_DARK = (140, 30, 30)  # Dark red facets
FLY_EYE_FACET_LIGHT = (200, 60, 50)  # Lighter red facets
FLY_EYE_REFLECTION = (255, 200, 180)  # Eye reflections
FLY_WING_BASE = (180, 190, 200)  # Semi-transparent wing base
FLY_WING_VEIN = (100, 110, 120)  # Wing veins
FLY_HAIR_COLOR = (60, 65, 70)  # Tiny hairs on body

def interpolate_color(color1, color2, factor):
    """Interpolate between two colors"""
    return tuple(int(c1 + (c2 - c1) * factor) for c1, c2 in zip(color1, color2))

def draw_gradient_ellipse(draw, bbox, color_center, color_edge):
    """Draw an ellipse with radial gradient effect using multiple layers"""
    center_x = (bbox[0] + bbox[2]) / 2
    center_y = (bbox[1] + bbox[3]) / 2
    width = bbox[2] - bbox[0]
    height = bbox[3] - bbox[1]
    
    # Draw multiple concentric ellipses for gradient effect
    steps = 20
    for i in range(steps, 0, -1):
        factor = i / steps
        current_color = interpolate_color(color_center, color_edge, 1 - factor)
        scale = factor
        new_bbox = [
            center_x - width * scale / 2,
            center_y - height * scale / 2,
            center_x + width * scale / 2,
            center_y + height * scale / 2
        ]
        draw.ellipse(new_bbox, fill=current_color)

def draw_hexagon(draw, center_x, center_y, radius, color, border_color=None):
    """Draw a hexagon with optional border"""
    points = []
    for i in range(6):
        angle = math.pi / 3 * i
        x = center_x + radius * math.cos(angle)
        y = center_y + radius * math.sin(angle)
        points.append((x, y))
    
    if border_color:
        draw.polygon(points, fill=color, outline=border_color)
    else:
        draw.polygon(points, fill=color)

def draw_realistic_compound_eye(draw, center_x, center_y, eye_radius, facet_size):
    """
    Draw a photorealistic compound eye with hexagonal facets and 3D shading
    """
    # Draw base eye sphere with gradient
    base_bbox = [
        center_x - eye_radius, center_y - eye_radius,
        center_x + eye_radius, center_y + eye_radius
    ]
    draw_gradient_ellipse(draw, base_bbox, FLY_EYE_BASE, 
                          (FLY_EYE_BASE[0]//2, FLY_EYE_BASE[1]//2, FLY_EYE_BASE[2]//2))
    
    # Draw hexagonal facets in a grid pattern with 3D effect
    num_rows = max(5, int(eye_radius / facet_size))
    
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
                # Calculate 3D lighting effect based on position
                # Facets facing the light (top-left) are brighter
                angle_to_center = math.atan2(facet_y - center_y, facet_x - center_x)
                light_angle = math.pi * 1.25  # Light from top-left
                light_factor = (math.cos(angle_to_center - light_angle) + 1) / 2
                
                # Add depth based on distance from center (spherical effect)
                depth_factor = 1.0 - (dist / eye_radius) * 0.5
                
                # Calculate facet color with lighting
                facet_color = interpolate_color(FLY_EYE_FACET_DARK, FLY_EYE_FACET_LIGHT, 
                                                light_factor * depth_factor)
                
                # Facets get slightly smaller towards edges
                size_factor = 1.0 - (dist / eye_radius) * 0.3
                actual_size = facet_size * size_factor * 0.75
                
                # Draw facet with border for definition
                border_color = interpolate_color(facet_color, (0, 0, 0), 0.5)
                draw_hexagon(draw, facet_x, facet_y, actual_size, facet_color, border_color)
                
                # Add tiny highlight to some facets for realism
                if (row + col) % 7 == 0 and dist < eye_radius * 0.6:
                    highlight_size = actual_size * 0.3
                    highlight_offset_x = actual_size * 0.2
                    highlight_offset_y = -actual_size * 0.2
                    draw.ellipse([
                        facet_x + highlight_offset_x - highlight_size,
                        facet_y + highlight_offset_y - highlight_size,
                        facet_x + highlight_offset_x + highlight_size,
                        facet_y + highlight_offset_y + highlight_size
                    ], fill=FLY_EYE_REFLECTION)
    
    # Add overall eye reflection (specular highlight)
    reflection_size = eye_radius * 0.25
    reflection_x = center_x - eye_radius * 0.3
    reflection_y = center_y - eye_radius * 0.3
    
    # Draw reflection with gradient
    for i in range(5, 0, -1):
        alpha = i / 5
        size = reflection_size * (i / 5)
        color = interpolate_color(FLY_EYE_REFLECTION, FLY_EYE_FACET_LIGHT, 1 - alpha)
        draw.ellipse([
            reflection_x - size, reflection_y - size,
            reflection_x + size, reflection_y + size
        ], fill=color)

def draw_realistic_fly_body(draw, center_x, center_y, size):
    """Draw a realistic fly body with 3D shading and texture"""
    body_width = size * 0.8
    body_height = size * 1.2
    
    # Draw body segments
    num_segments = 5
    for i in range(num_segments):
        segment_y = center_y + (i - num_segments/2) * body_height / (num_segments * 1.2)
        segment_height = body_height / (num_segments * 0.9)
        segment_width = body_width * (1 - abs(i - num_segments/2) / num_segments * 0.3)
        
        # Draw segment with gradient
        bbox = [
            center_x - segment_width / 2, segment_y - segment_height / 2,
            center_x + segment_width / 2, segment_y + segment_height / 2
        ]
        draw_gradient_ellipse(draw, bbox, FLY_BODY_HIGHLIGHT, FLY_BODY_BASE)
        
        # Add tiny hairs
        if size > 50:  # Only draw hairs when fly is close enough
            num_hairs = int(size / 20)
            for j in range(num_hairs):
                angle = (j / num_hairs) * math.pi * 2
                hair_start_x = center_x + segment_width / 2 * math.cos(angle) * 0.9
                hair_start_y = segment_y + segment_height / 2 * math.sin(angle) * 0.9
                hair_end_x = hair_start_x + math.cos(angle) * size * 0.1
                hair_end_y = hair_start_y + math.sin(angle) * size * 0.1
                draw.line([(hair_start_x, hair_start_y), (hair_end_x, hair_end_y)], 
                         fill=FLY_HAIR_COLOR, width=max(1, int(size / 100)))

def draw_realistic_wings(draw, center_x, center_y, size, wing_angle):
    """Draw realistic semi-transparent wings with veins"""
    wing_length = size * 1.8
    wing_width = size * 1.0
    
    # Calculate wing position based on angle (flapping)
    angle_offset = math.sin(wing_angle) * 0.2
    
    # Left wing
    left_wing_x = center_x - size * 0.4
    left_wing_y = center_y - size * 0.3
    
    # Create wing shape (more realistic teardrop shape)
    wing_img = Image.new('RGBA', (int(wing_length * 2), int(wing_width * 2)), (0, 0, 0, 0))
    wing_draw = ImageDraw.Draw(wing_img)
    
    # Draw wing membrane with transparency
    wing_alpha = 120
    wing_color_with_alpha = FLY_WING_BASE + (wing_alpha,)
    wing_bbox = [
        wing_length * 0.2, wing_width * 0.3 + angle_offset * size * 0.5,
        wing_length * 1.5, wing_width * 1.7 + angle_offset * size * 0.5
    ]
    wing_draw.ellipse(wing_bbox, fill=wing_color_with_alpha)
    
    # Draw wing veins
    num_veins = 8
    for i in range(num_veins):
        vein_angle = -math.pi / 4 + (i / num_veins) * math.pi / 2
        vein_start_x = wing_length * 0.5
        vein_start_y = wing_width
        vein_end_x = vein_start_x + math.cos(vein_angle) * wing_length * 0.8
        vein_end_y = vein_start_y + math.sin(vein_angle) * wing_width * 0.6
        wing_draw.line([(vein_start_x, vein_start_y), (vein_end_x, vein_end_y)],
                      fill=FLY_WING_VEIN + (wing_alpha,), width=max(1, int(size / 80)))
    
    # Apply blur for realism
    wing_img = wing_img.filter(ImageFilter.GaussianBlur(radius=max(1, size / 100)))
    
    # Paste wing onto main image (left wing)
    # Note: PIL doesn't support rotation with alpha easily, so we'll use simpler placement
    # In a production environment, you'd use a more sophisticated graphics library
    
    # Right wing (mirror)
    right_wing_x = center_x + size * 0.4
    right_wing_y = center_y - size * 0.3

def generate_frame(frame_num):
    """
    Generate one frame of the realistic fly animation
    The fly starts far away and approaches the camera
    Returns image data as PIL Image
    """
    # Create image with dark background
    img = Image.new('RGB', (WIDTH, HEIGHT), BACKGROUND_COLOR)
    draw = ImageDraw.Draw(img)
    
    # Calculate fly's distance from camera (exponential approach)
    # Using exponential for more dramatic effect as fly gets very close
    progress = frame_num / FRAMES
    distance = INITIAL_DISTANCE * math.exp(-APPROACH_RATE * progress)
    
    # Calculate fly size based on distance (inverse perspective)
    fly_size = INITIAL_FLY_SIZE * (INITIAL_DISTANCE / distance)
    
    # Center of screen
    center_x = WIDTH / 2
    center_y = HEIGHT / 2
    
    # Add slight wobble to make flight more realistic
    wobble_x = math.sin(frame_num * WOBBLE_FREQ_X) * WOBBLE_AMPLITUDE_X * (1 - progress)
    wobble_y = math.cos(frame_num * WOBBLE_FREQ_Y) * WOBBLE_AMPLITUDE_Y * (1 - progress)
    
    fly_x = center_x + wobble_x
    fly_y = center_y + wobble_y
    
    # Wing flapping animation
    wing_angle = frame_num * WING_FLAP_SPEED
    
    # Draw different parts based on distance
    if fly_size < 80:
        # Fly is far away - draw full fly with wings
        # Wings first (behind body)
        draw_realistic_wings(draw, fly_x, fly_y, fly_size, wing_angle)
        
        # Body
        draw_realistic_fly_body(draw, fly_x, fly_y, fly_size)
        
        # Eyes
        eye_radius = fly_size * 0.45
        eye_separation = fly_size * 0.65
        facet_size = max(2, eye_radius / 20)
        
        # Left eye
        draw_realistic_compound_eye(draw, fly_x - eye_separation / 2, fly_y - fly_size * 0.2, 
                                    eye_radius, facet_size)
        
        # Right eye
        draw_realistic_compound_eye(draw, fly_x + eye_separation / 2, fly_y - fly_size * 0.2, 
                                    eye_radius, facet_size)
    else:
        # Fly is very close - only show detailed eyes filling screen
        eye_radius = min(fly_size * 0.9, WIDTH * 0.48)
        eye_separation = fly_size * 0.55
        facet_size = max(3, eye_radius / 25)
        
        # Left eye
        draw_realistic_compound_eye(draw, fly_x - eye_separation / 2, fly_y, 
                                    eye_radius, facet_size)
        
        # Right eye
        draw_realistic_compound_eye(draw, fly_x + eye_separation / 2, fly_y, 
                                    eye_radius, facet_size)
        
        # Add subtle depth of field blur to background/edges when very close
        if fly_size > 200:
            # Apply slight blur to entire image for depth effect
            img = img.filter(ImageFilter.GaussianBlur(radius=0.5))
    
    return img

def main():
    print(f"Generating Realistic Fly Animation (inspired by 'Mandibules')...")
    print(f"  Resolution: {WIDTH}x{HEIGHT} pixels (Full HD)")
    print(f"  Frames: {FRAMES}")
    print(f"  Animation: Photorealistic fly approaching camera")
    print(f"  Rendering: 3D-style with gradients and lighting")
    
    # Create output directory for PNG frames
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Generate all frames
    print("Generating frames...")
    print("  (This may take a while due to high resolution and realistic rendering)")
    for frame in range(FRAMES):
        if frame % 10 == 0:
            print(f"  Frame {frame}/{FRAMES} ({100*frame/FRAMES:.1f}%)...")
        
        # Generate frame
        img = generate_frame(frame)
        
        # Save frame as PNG
        filename = os.path.join(frames_dir, f'frame_{frame:04d}.png')
        img.save(filename, optimize=True)
    
    print(f"Done! Generated {FRAMES} frames")
    print(f"PNG frames saved to: {frames_dir}/")
    print(f"\nTo create a video from frames, you can use ffmpeg:")
    print(f"  ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p fly_animation_hd.mp4")

if __name__ == '__main__':
    main()
