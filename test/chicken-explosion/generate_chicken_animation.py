#!/usr/bin/env python3
"""
Generate Full HD comic-style chicken with explosives animation
Creates a sequence showing:
1. Chicken looking at camera
2. Picking on floor
3. Looking at camera
4. Picking on floor again
5. Pecking at camera glass multiple times
6. Explosion

Output: Full HD (1920x1080) PNG frames
"""

import math
import os
from PIL import Image, ImageDraw, ImageFont


# Animation parameters
WIDTH = 1920  # Full HD width
HEIGHT = 1080  # Full HD height

# Color palette - comic style
BACKGROUND_COLOR = (135, 206, 235)  # Sky blue
CHICKEN_BODY = (255, 255, 200)  # Light yellow
CHICKEN_OUTLINE = (50, 50, 50)  # Dark grey
BEAK_COLOR = (255, 165, 0)  # Orange
COMB_COLOR = (220, 20, 60)  # Crimson
EYE_WHITE = (255, 255, 255)
EYE_BLACK = (0, 0, 0)
GROUND_COLOR = (139, 90, 43)  # Brown
EXPLOSIVE_COLOR = (70, 70, 70)  # Dark grey for TNT
EXPLOSIVE_RED = (200, 0, 0)  # Red stripes
FUSE_COLOR = (100, 50, 0)  # Brown fuse
SPARK_COLOR = (255, 200, 0)  # Yellow spark
EXPLOSION_COLORS = [
    (255, 69, 0),   # Orange-red
    (255, 140, 0),  # Dark orange
    (255, 215, 0),  # Gold
    (255, 255, 0),  # Yellow
    (255, 69, 0),   # Orange-red
]


def draw_chicken(draw, center_x, center_y, head_tilt=0, beak_open=0, eye_direction=0, scale=1.0):
    """
    Draw a comic-style chicken
    
    Args:
        draw: PIL ImageDraw object
        center_x, center_y: Position of chicken center
        head_tilt: Head tilt angle in degrees (positive = down, for pecking)
        beak_open: How much the beak is open (0-10)
        eye_direction: Eye direction (-1=left, 0=center, 1=right)
        scale: Size multiplier
    """
    # Scale all dimensions
    s = scale
    
    # Body (main oval)
    body_left = center_x - int(80 * s)
    body_top = center_y - int(50 * s)
    body_right = center_x + int(80 * s)
    body_bottom = center_y + int(100 * s)
    
    # Draw body outline
    draw.ellipse([body_left, body_top, body_right, body_bottom], 
                 fill=CHICKEN_BODY, outline=CHICKEN_OUTLINE, width=int(4 * s))
    
    # Head position (adjusted for tilt)
    head_x = center_x
    head_y = center_y - int(60 * s)
    
    # Apply head tilt
    tilt_rad = math.radians(head_tilt)
    beak_offset_x = int(math.sin(tilt_rad) * 40 * s)
    beak_offset_y = int(math.cos(tilt_rad) * 40 * s)
    
    # Head (circle)
    head_size = int(60 * s)
    draw.ellipse([head_x - head_size, head_y - head_size, 
                  head_x + head_size, head_y + head_size],
                 fill=CHICKEN_BODY, outline=CHICKEN_OUTLINE, width=int(4 * s))
    
    # Comb (on top of head)
    comb_points = [
        (head_x - int(20 * s), head_y - head_size),
        (head_x - int(15 * s), head_y - head_size - int(25 * s)),
        (head_x - int(5 * s), head_y - head_size - int(15 * s)),
        (head_x + int(5 * s), head_y - head_size - int(30 * s)),
        (head_x + int(15 * s), head_y - head_size - int(20 * s)),
        (head_x + int(20 * s), head_y - head_size),
    ]
    draw.polygon(comb_points, fill=COMB_COLOR, outline=CHICKEN_OUTLINE, width=int(2 * s))
    
    # Eyes (two circles)
    eye_size = int(12 * s)
    eye_offset = int(25 * s)
    pupil_size = int(6 * s)
    
    # Left eye
    left_eye_x = head_x - eye_offset
    left_eye_y = head_y - int(10 * s)
    draw.ellipse([left_eye_x - eye_size, left_eye_y - eye_size,
                  left_eye_x + eye_size, left_eye_y + eye_size],
                 fill=EYE_WHITE, outline=CHICKEN_OUTLINE, width=int(2 * s))
    
    # Left pupil
    pupil_x = left_eye_x + int(eye_direction * 5 * s)
    draw.ellipse([pupil_x - pupil_size, left_eye_y - pupil_size,
                  pupil_x + pupil_size, left_eye_y + pupil_size],
                 fill=EYE_BLACK)
    
    # Right eye
    right_eye_x = head_x + eye_offset
    right_eye_y = head_y - int(10 * s)
    draw.ellipse([right_eye_x - eye_size, right_eye_y - eye_size,
                  right_eye_x + eye_size, right_eye_y + eye_size],
                 fill=EYE_WHITE, outline=CHICKEN_OUTLINE, width=int(2 * s))
    
    # Right pupil
    pupil_x = right_eye_x + int(eye_direction * 5 * s)
    draw.ellipse([pupil_x - pupil_size, right_eye_y - pupil_size,
                  pupil_x + pupil_size, right_eye_y + pupil_size],
                 fill=EYE_BLACK)
    
    # Beak (triangle, affected by tilt and open state)
    beak_tip_x = head_x + beak_offset_x + int(70 * s)
    beak_tip_y = head_y + beak_offset_y
    beak_top_y = beak_tip_y - int(15 * s) - beak_open
    beak_bottom_y = beak_tip_y + int(15 * s) + beak_open
    
    beak_points = [
        (head_x + int(45 * s), beak_top_y),
        (beak_tip_x, beak_tip_y),
        (head_x + int(45 * s), beak_bottom_y),
    ]
    draw.polygon(beak_points, fill=BEAK_COLOR, outline=CHICKEN_OUTLINE, width=int(3 * s))
    
    # Legs (two simple lines)
    leg_bottom_y = center_y + int(100 * s)
    leg_height = int(50 * s)
    
    # Left leg
    draw.line([center_x - int(25 * s), leg_bottom_y, 
               center_x - int(25 * s), leg_bottom_y + leg_height],
              fill=BEAK_COLOR, width=int(6 * s))
    # Left foot (three toes)
    foot_y = leg_bottom_y + leg_height
    draw.line([center_x - int(40 * s), foot_y, center_x - int(25 * s), foot_y],
              fill=BEAK_COLOR, width=int(4 * s))
    draw.line([center_x - int(10 * s), foot_y, center_x - int(25 * s), foot_y],
              fill=BEAK_COLOR, width=int(4 * s))
    
    # Right leg
    draw.line([center_x + int(25 * s), leg_bottom_y,
               center_x + int(25 * s), leg_bottom_y + leg_height],
              fill=BEAK_COLOR, width=int(6 * s))
    # Right foot
    draw.line([center_x + int(10 * s), foot_y, center_x + int(25 * s), foot_y],
              fill=BEAK_COLOR, width=int(4 * s))
    draw.line([center_x + int(40 * s), foot_y, center_x + int(25 * s), foot_y],
              fill=BEAK_COLOR, width=int(4 * s))


def draw_explosive_necklace(draw, center_x, center_y, scale=1.0, fuse_lit_progress=0):
    """
    Draw explosives around chicken's neck
    
    Args:
        draw: PIL ImageDraw object
        center_x, center_y: Position of chicken center
        scale: Size multiplier
        fuse_lit_progress: 0-1, how much of the fuse is burning
    """
    s = scale
    
    # Neck position (around the chicken's body top)
    neck_y = center_y - int(40 * s)
    
    # Draw multiple TNT sticks around neck
    num_sticks = 5
    stick_width = int(15 * s)
    stick_height = int(40 * s)
    
    for i in range(num_sticks):
        # Position around neck
        angle = (i / num_sticks) * math.pi - math.pi / 2
        radius = int(85 * s)
        stick_x = center_x + int(math.cos(angle) * radius)
        stick_y = neck_y + int(math.sin(angle) * radius * 0.3)
        
        # TNT stick (rectangle)
        draw.rectangle([stick_x - stick_width, stick_y - stick_height // 2,
                       stick_x + stick_width, stick_y + stick_height // 2],
                      fill=EXPLOSIVE_COLOR, outline=CHICKEN_OUTLINE, width=int(2 * s))
        
        # Red stripe
        draw.rectangle([stick_x - stick_width + int(3 * s), 
                       stick_y - int(5 * s),
                       stick_x + stick_width - int(3 * s), 
                       stick_y + int(5 * s)],
                      fill=EXPLOSIVE_RED)
        
        # Fuse
        fuse_start_x = stick_x
        fuse_start_y = stick_y - stick_height // 2
        fuse_end_y = fuse_start_y - int(20 * s)
        
        draw.line([fuse_start_x, fuse_start_y, fuse_start_x, fuse_end_y],
                 fill=FUSE_COLOR, width=int(3 * s))
        
        # Spark if fuse is lit
        if fuse_lit_progress > 0:
            # Spark position moves down the fuse
            spark_y = fuse_end_y + int((fuse_start_y - fuse_end_y) * fuse_lit_progress)
            spark_size = int(8 * s)
            draw.ellipse([fuse_start_x - spark_size, spark_y - spark_size,
                         fuse_start_x + spark_size, spark_y + spark_size],
                        fill=SPARK_COLOR, outline=(255, 100, 0), width=int(2 * s))


def draw_explosion(draw, center_x, center_y, progress=0):
    """
    Draw explosion effect
    
    Args:
        draw: PIL ImageDraw object
        center_x, center_y: Explosion center
        progress: 0-1, explosion animation progress
    """
    if progress <= 0:
        return
    
    # Multiple expanding circles with different colors
    max_radius = 600
    num_circles = 8
    
    for i in range(num_circles):
        radius = int(max_radius * progress * (1 - i * 0.1))
        if radius > 0:
            color = EXPLOSION_COLORS[i % len(EXPLOSION_COLORS)]
            # Add some transparency effect by drawing thinner circles
            alpha = int(255 * (1 - progress))
            draw.ellipse([center_x - radius, center_y - radius,
                         center_x + radius, center_y + radius],
                        fill=None, outline=color, width=int(30 * (1 - progress * 0.5)))
    
    # Draw explosive debris/particles
    if progress < 0.5:
        num_particles = 20
        for i in range(num_particles):
            angle = (i / num_particles) * 2 * math.pi
            distance = int(300 * progress)
            px = center_x + int(math.cos(angle) * distance)
            py = center_y + int(math.sin(angle) * distance)
            size = 10
            draw.ellipse([px - size, py - size, px + size, py + size],
                        fill=EXPLOSIVE_COLOR)


def draw_ground(draw):
    """Draw the ground"""
    ground_y = HEIGHT - 200
    draw.rectangle([0, ground_y, WIDTH, HEIGHT], fill=GROUND_COLOR)
    
    # Add some texture (simple lines)
    for i in range(5):
        y = ground_y + i * 30
        draw.line([0, y, WIDTH, y], fill=(120, 80, 40), width=2)


def generate_frame(frame_num, total_frames):
    """
    Generate one frame of the animation
    
    Animation sequence:
    - Frames 0-29: Looking at camera (standing still)
    - Frames 30-59: Picking on floor (head down)
    - Frames 60-89: Looking at camera again
    - Frames 90-119: Picking on floor again
    - Frames 120-179: Pecking at camera glass (multiple pecks)
    - Frames 180-209: Fuse burning
    - Frames 210+: Explosion
    """
    # Create image
    img = Image.new('RGB', (WIDTH, HEIGHT), BACKGROUND_COLOR)
    draw = ImageDraw.Draw(img)
    
    # Draw ground
    draw_ground(draw)
    
    # Chicken position
    chicken_x = WIDTH // 2
    chicken_y = HEIGHT - 350
    
    # Animation state variables
    head_tilt = 0
    beak_open = 0
    eye_direction = 0
    fuse_lit_progress = 0
    explosion_progress = 0
    
    # Determine animation phase
    if frame_num < 30:
        # Phase 1: Looking at camera
        eye_direction = 0
        head_tilt = 0
        # Occasional blink
        if frame_num % 20 > 18:
            beak_open = 2
    
    elif frame_num < 60:
        # Phase 2: Picking on floor
        phase_progress = (frame_num - 30) / 30
        head_tilt = 45 + math.sin(phase_progress * math.pi * 4) * 10
        if int(phase_progress * 8) % 2 == 0:
            beak_open = 5
    
    elif frame_num < 90:
        # Phase 3: Looking at camera again
        eye_direction = 0
        head_tilt = 0
    
    elif frame_num < 120:
        # Phase 4: Picking on floor again
        phase_progress = (frame_num - 90) / 30
        head_tilt = 45 + math.sin(phase_progress * math.pi * 4) * 10
        if int(phase_progress * 8) % 2 == 0:
            beak_open = 5
    
    elif frame_num < 180:
        # Phase 5: Pecking at camera glass
        phase_progress = (frame_num - 120) / 60
        # Multiple quick pecks
        peck_cycle = (phase_progress * 10) % 1
        if peck_cycle < 0.3:
            head_tilt = -15  # Lean forward
            beak_open = 8
        else:
            head_tilt = -5
            beak_open = 0
        eye_direction = 0
    
    elif frame_num < 210:
        # Phase 6: Fuse burning
        fuse_lit_progress = (frame_num - 180) / 30
        eye_direction = 0
        head_tilt = 0
        # Chicken looks worried
        if frame_num % 4 < 2:
            eye_direction = -1
        else:
            eye_direction = 1
    
    else:
        # Phase 7: Explosion
        explosion_progress = min(1.0, (frame_num - 210) / 20)
        eye_direction = 0
        head_tilt = 0
    
    # Draw chicken (unless fully exploded)
    if explosion_progress < 0.8:
        draw_chicken(draw, chicken_x, chicken_y, head_tilt, beak_open, eye_direction)
        draw_explosive_necklace(draw, chicken_x, chicken_y, fuse_lit_progress=fuse_lit_progress)
    
    # Draw explosion
    if explosion_progress > 0:
        draw_explosion(draw, chicken_x, chicken_y - 50, explosion_progress)
    
    return img


def main():
    print("Generating chicken explosion animation...")
    print(f"  Resolution: {WIDTH}x{HEIGHT} (Full HD)")
    
    # Create output directory
    frames_dir = 'frames'
    if not os.path.exists(frames_dir):
        os.makedirs(frames_dir)
        print(f"Created directory: {frames_dir}/")
    
    # Animation settings
    total_frames = 240  # About 8 seconds at 30fps
    
    # Generate all frames
    print(f"Generating {total_frames} frames...")
    for frame in range(total_frames):
        if frame % 30 == 0:
            print(f"  Frame {frame}/{total_frames}...")
        
        img = generate_frame(frame, total_frames)
        
        # Save frame
        filename = os.path.join(frames_dir, f'frame_{frame:04d}.png')
        img.save(filename)
    
    print(f"Done! Generated {total_frames} frames")
    print(f"Frames saved to: {frames_dir}/")
    print(f"\nTo create a video, you can use ffmpeg:")
    print(f"  ffmpeg -framerate 30 -i {frames_dir}/frame_%04d.png -c:v libx264 -pix_fmt yuv420p chicken_explosion.mp4")


if __name__ == '__main__':
    main()
