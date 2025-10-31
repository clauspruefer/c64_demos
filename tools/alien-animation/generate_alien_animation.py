#!/usr/bin/env python3
"""
Generate Full-HD animation: Alien in flying saucer with burp and fart effects
Creates a 1920x1080 pixel MP4 animation showing:
- Alien sitting in flying saucer drinking 3 beer bottles in parallel
- Human with cheeseburger passing by
- Alien burps, sending cheeseburger flying 30 meters
- Alien farts, making Rastafari hair fly like tornado
"""

import numpy as np
from PIL import Image, ImageDraw
import os
import sys

# Animation parameters
WIDTH = 1920
HEIGHT = 1080
FPS = 30
DURATION = 15  # seconds

# Scene timing
SCENE_1_DURATION = 3.0  # Alien drinking
SCENE_2_DURATION = 2.0  # Human walking
SCENE_3_DURATION = 1.0  # Saucer moves in front
SCENE_4_DURATION = 4.0  # Burp effect
SCENE_5_DURATION = 5.0  # Fart effect

class AlienAnimation:
    def __init__(self, width=WIDTH, height=HEIGHT, fps=FPS):
        self.width = width
        self.height = height
        self.fps = fps
        
    def draw_flying_saucer(self, draw, x, y, scale=1.0):
        """Draw a flying saucer"""
        w = int(200 * scale)
        h = int(80 * scale)
        
        # Main saucer body (ellipse)
        draw.ellipse([x - w, y - h//2, x + w, y + h//2], 
                     fill=(180, 180, 200, 255), outline=(100, 100, 120, 255), width=3)
        
        # Dome (semi-transparent)
        dome_h = int(40 * scale)
        draw.ellipse([x - w//2, y - h//2 - dome_h, x + w//2, y - h//2 + 10], 
                     fill=(150, 200, 250, 200), outline=(80, 120, 160, 255), width=2)
        
        # Lights
        for i in range(-2, 3):
            light_x = x + i * w // 3
            light_y = y + h//4
            draw.ellipse([light_x - 10, light_y - 5, light_x + 10, light_y + 5],
                        fill=(255, 255, 0, 255))
        
        return (x, y - h//2 - dome_h//2)  # Return center of dome for alien position
    
    def draw_alien(self, draw, x, y, scale=1.0, drinking=False):
        """Draw an alien character"""
        # Head (green oval)
        head_w = int(40 * scale)
        head_h = int(50 * scale)
        draw.ellipse([x - head_w, y - head_h, x + head_w, y + head_h],
                     fill=(100, 255, 100), outline=(50, 200, 50), width=2)
        
        # Eyes (large black ovals)
        eye_y = y - head_h//2
        draw.ellipse([x - 25, eye_y - 15, x - 10, eye_y + 5], fill=(0, 0, 0))
        draw.ellipse([x + 10, eye_y - 15, x + 25, eye_y + 5], fill=(0, 0, 0))
        
        # Body (small)
        body_y = y + head_h + 10
        draw.ellipse([x - 25, body_y, x + 25, body_y + 30], 
                     fill=(80, 200, 80), outline=(50, 150, 50), width=2)
        
        # Arms
        if drinking:
            # Arms holding bottles
            draw.line([x - 25, body_y + 10, x - 60, body_y - 20], 
                     fill=(80, 200, 80), width=8)
            draw.line([x, body_y + 10, x, body_y - 30], 
                     fill=(80, 200, 80), width=8)
            draw.line([x + 25, body_y + 10, x + 60, body_y - 20], 
                     fill=(80, 200, 80), width=8)
        else:
            draw.line([x - 25, body_y + 10, x - 50, body_y + 30], 
                     fill=(80, 200, 80), width=8)
            draw.line([x + 25, body_y + 10, x + 50, body_y + 30], 
                     fill=(80, 200, 80), width=8)
    
    def draw_beer_bottles(self, draw, x, y, count=3, scale=1.0):
        """Draw beer bottles"""
        bottle_w = int(15 * scale)
        bottle_h = int(50 * scale)
        spacing = int(80 * scale)
        
        for i in range(count):
            bottle_x = x + (i - 1) * spacing
            # Bottle body
            draw.rectangle([bottle_x - bottle_w, y, 
                          bottle_x + bottle_w, y + bottle_h],
                         fill=(139, 69, 19), outline=(101, 50, 15), width=2)
            # Neck
            draw.rectangle([bottle_x - bottle_w//2, y - 15, 
                          bottle_x + bottle_w//2, y],
                         fill=(139, 69, 19), outline=(101, 50, 15), width=2)
            # Cap
            draw.rectangle([bottle_x - bottle_w//2 - 2, y - 20, 
                          bottle_x + bottle_w//2 + 2, y - 15],
                         fill=(192, 192, 192))
            # Beer level
            beer_level = y + bottle_h - 10
            draw.rectangle([bottle_x - bottle_w + 3, beer_level, 
                          bottle_x + bottle_w - 3, y + bottle_h - 3],
                         fill=(255, 215, 0))
    
    def draw_human(self, draw, x, y, scale=1.0, hair_angle=0):
        """Draw a human with Rastafari hair"""
        # Head
        head_r = int(30 * scale)
        draw.ellipse([x - head_r, y - head_r, x + head_r, y + head_r],
                     fill=(255, 220, 177), outline=(200, 170, 130), width=2)
        
        # Eyes
        eye_y = y - 10
        draw.ellipse([x - 15, eye_y - 5, x - 8, eye_y + 2], fill=(255, 255, 255))
        draw.ellipse([x + 8, eye_y - 5, x + 15, eye_y + 2], fill=(255, 255, 255))
        draw.ellipse([x - 13, eye_y - 3, x - 10, eye_y], fill=(0, 0, 0))
        draw.ellipse([x + 10, eye_y - 3, x + 13, eye_y], fill=(0, 0, 0))
        
        # Mouth
        draw.arc([x - 15, y + 5, x + 15, y + 20], 0, 180, fill=(0, 0, 0), width=2)
        
        # Rastafari hair (dreads)
        colors = [(255, 0, 0), (255, 255, 0), (0, 255, 0)]  # Red, Yellow, Green
        num_dreads = 12
        for i in range(num_dreads):
            angle = (i / num_dreads) * 360 + hair_angle
            # Hair length varies with tornado effect
            hair_len = 60 + int(30 * abs(np.sin(np.radians(hair_angle * 2))))
            start_angle = np.radians(angle)
            end_x = x + int(head_r * np.cos(start_angle))
            end_y = y - head_r + int(head_r * np.sin(start_angle))
            
            # Dread end position (affected by tornado)
            tornado_offset_x = int(hair_len * np.sin(np.radians(hair_angle + i * 30)))
            tornado_offset_y = int(hair_len * np.cos(np.radians(hair_angle + i * 30)))
            
            dread_end_x = end_x + tornado_offset_x
            dread_end_y = end_y + tornado_offset_y
            
            color = colors[i % len(colors)]
            draw.line([end_x, end_y, dread_end_x, dread_end_y], 
                     fill=color, width=6)
        
        # Body
        body_y = y + head_r
        draw.rectangle([x - 40, body_y, x + 40, body_y + 80],
                      fill=(200, 0, 0), outline=(150, 0, 0), width=2)
        
        # Arms
        draw.line([x - 40, body_y + 20, x - 70, body_y + 50], 
                 fill=(255, 220, 177), width=15)
        draw.line([x + 40, body_y + 20, x + 70, body_y + 50], 
                 fill=(255, 220, 177), width=15)
        
        # Legs
        draw.line([x - 20, body_y + 80, x - 20, body_y + 140], 
                 fill=(0, 0, 200), width=15)
        draw.line([x + 20, body_y + 80, x + 20, body_y + 140], 
                 fill=(0, 0, 200), width=15)
    
    def draw_cheeseburger(self, draw, x, y, scale=1.0):
        """Draw a cheeseburger"""
        size = int(50 * scale)
        
        # Top bun
        draw.ellipse([x - size, y - size//2, x + size, y + size//2],
                     fill=(210, 180, 140), outline=(180, 150, 110), width=2)
        
        # Sesame seeds
        for i in range(8):
            seed_x = x + int((size - 20) * np.cos(i * np.pi / 4))
            seed_y = y + int((size//2 - 10) * np.sin(i * np.pi / 4))
            draw.ellipse([seed_x - 3, seed_y - 3, seed_x + 3, seed_y + 3],
                        fill=(255, 255, 200))
        
        # Cheese
        draw.rectangle([x - size + 10, y + size//2, x + size - 10, y + size//2 + 15],
                      fill=(255, 200, 0), outline=(200, 150, 0), width=2)
        
        # Patty
        draw.ellipse([x - size + 5, y + size//2 + 15, x + size - 5, y + size//2 + 35],
                     fill=(139, 69, 19), outline=(100, 50, 15), width=2)
        
        # Lettuce
        draw.arc([x - size + 10, y + size//2 + 35, x + size - 10, y + size//2 + 50],
                0, 180, fill=(0, 255, 0), width=8)
        
        # Bottom bun
        draw.ellipse([x - size, y + size//2 + 50, x + size, y + size + 50],
                     fill=(210, 180, 140), outline=(180, 150, 110), width=2)
    
    def draw_burp_effect(self, draw, x, y, radius, alpha=128):
        """Draw burp cloud effect"""
        # Draw expanding circles for burp effect
        for i in range(5):
            r = radius + i * 20
            outline_alpha = max(0, alpha - i * 25)
            draw.ellipse([x - r, y - r, x + r, y + r],
                        outline=(150, 255, 150, outline_alpha), width=3)
    
    def draw_fart_effect(self, draw, x, y, size, alpha=128):
        """Draw fart cloud effect"""
        # Draw wavy greenish clouds
        for i in range(3):
            offset_y = i * 30
            fill_alpha = max(0, alpha//2 - i * 20)
            outline_alpha = max(0, alpha - i * 40)
            draw.ellipse([x - size - i*10, y + offset_y - size//2, 
                         x + size + i*10, y + offset_y + size//2],
                        fill=(200, 255, 150, fill_alpha),
                        outline=(150, 200, 100, outline_alpha), width=2)
    
    def make_frame(self, t):
        """Generate a single frame at time t"""
        # Create image with RGBA mode to support alpha transparency
        img = Image.new('RGBA', (self.width, self.height), color=(135, 206, 235, 255))
        draw = ImageDraw.Draw(img)
        
        # Ground
        draw.rectangle([0, self.height - 200, self.width, self.height],
                      fill=(34, 139, 34))
        
        # Determine which scene we're in
        if t < SCENE_1_DURATION:
            # Scene 1: Alien drinking in saucer (hovering in upper left)
            saucer_x = 400
            saucer_y = 300
            alien_pos = self.draw_flying_saucer(draw, saucer_x, saucer_y)
            self.draw_alien(draw, alien_pos[0], alien_pos[1], drinking=True)
            self.draw_beer_bottles(draw, alien_pos[0], alien_pos[1] + 100, count=3)
            
        elif t < SCENE_1_DURATION + SCENE_2_DURATION:
            # Scene 2: Human walks in from right, alien still drinking
            saucer_x = 400
            saucer_y = 300
            alien_pos = self.draw_flying_saucer(draw, saucer_x, saucer_y)
            self.draw_alien(draw, alien_pos[0], alien_pos[1], drinking=True)
            self.draw_beer_bottles(draw, alien_pos[0], alien_pos[1] + 100, count=3)
            
            # Human walking
            progress = (t - SCENE_1_DURATION) / SCENE_2_DURATION
            human_x = self.width - int(progress * (self.width - 800))
            human_y = self.height - 350
            self.draw_human(draw, human_x, human_y)
            self.draw_cheeseburger(draw, human_x + 80, human_y - 50)
            
        elif t < SCENE_1_DURATION + SCENE_2_DURATION + SCENE_3_DURATION:
            # Scene 3: Saucer moves invisibly in front of human
            progress = (t - SCENE_1_DURATION - SCENE_2_DURATION) / SCENE_3_DURATION
            
            human_x = 800
            human_y = self.height - 350
            self.draw_human(draw, human_x, human_y)
            self.draw_cheeseburger(draw, human_x + 80, human_y - 50)
            
            # Saucer moves to position in front of human (but still visible to viewer)
            saucer_x = 400 + int(progress * (human_x - 400))
            saucer_y = 300 + int(progress * (human_y - 200 - 300))
            alien_pos = self.draw_flying_saucer(draw, saucer_x, saucer_y)
            self.draw_alien(draw, alien_pos[0], alien_pos[1], drinking=False)
            
        elif t < SCENE_1_DURATION + SCENE_2_DURATION + SCENE_3_DURATION + SCENE_4_DURATION:
            # Scene 4: Burp effect - cheeseburger flies away
            progress = (t - SCENE_1_DURATION - SCENE_2_DURATION - SCENE_3_DURATION) / SCENE_4_DURATION
            
            human_x = 800
            human_y = self.height - 350
            saucer_x = human_x
            saucer_y = human_y - 200
            
            # Draw human
            self.draw_human(draw, human_x, human_y)
            
            # Burp effect
            burp_radius = int(50 + progress * 150)
            burp_alpha = int(255 * (1 - progress))
            self.draw_burp_effect(draw, saucer_x, saucer_y + 50, burp_radius, burp_alpha)
            
            # Cheeseburger flies away (30 meters = ~900 pixels)
            burger_x = human_x + 80 + int(progress * 900)
            burger_y = human_y - 50 - int(progress * 200 * np.sin(progress * np.pi))
            burger_scale = 1 - progress * 0.7  # Gets smaller as it flies away
            self.draw_cheeseburger(draw, burger_x, burger_y, scale=burger_scale)
            
            # Draw saucer and alien
            alien_pos = self.draw_flying_saucer(draw, saucer_x, saucer_y)
            self.draw_alien(draw, alien_pos[0], alien_pos[1], drinking=False)
            
        else:
            # Scene 5: Fart effect - hair tornado
            progress = (t - SCENE_1_DURATION - SCENE_2_DURATION - SCENE_3_DURATION - SCENE_4_DURATION) / SCENE_5_DURATION
            
            human_x = 800
            human_y = self.height - 350
            saucer_x = human_x
            saucer_y = human_y - 200
            
            # Hair tornado effect
            hair_angle = progress * 720  # Two full rotations
            self.draw_human(draw, human_x, human_y, hair_angle=hair_angle)
            
            # Fart effect
            fart_size = int(40 + progress * 100)
            fart_alpha = int(200 * (1 - progress * 0.5))
            self.draw_fart_effect(draw, saucer_x, saucer_y + 100, fart_size, fart_alpha)
            
            # Draw saucer and alien
            alien_pos = self.draw_flying_saucer(draw, saucer_x, saucer_y)
            self.draw_alien(draw, alien_pos[0], alien_pos[1], drinking=False)
        
        # Convert RGBA to RGB for video output
        # Create RGB image and composite the RGBA image onto it
        rgb_img = Image.new('RGB', img.size, (135, 206, 235))
        rgb_img.paste(img, (0, 0), img)  # img acts as both source and mask
        
        return np.array(rgb_img)

def main():
    """Generate the animation"""
    # Import moviepy here so it's only needed when actually generating
    try:
        from moviepy import VideoClip
    except ImportError:
        print("Error: moviepy not installed. Install with: pip install moviepy")
        sys.exit(1)
    
    print("Generating alien animation...")
    print(f"Resolution: {WIDTH}x{HEIGHT}")
    print(f"FPS: {FPS}")
    print(f"Duration: {DURATION} seconds")
    
    # Create animation
    animation = AlienAnimation(WIDTH, HEIGHT, FPS)
    
    # Generate video
    clip = VideoClip(animation.make_frame, duration=DURATION)
    
    # Output file
    output_file = "alien_animation.mp4"
    
    print(f"Writing to {output_file}...")
    clip.write_videofile(output_file, fps=FPS, codec='libx264')
    
    print(f"Animation saved to {output_file}")
    print(f"Total frames: {DURATION * FPS}")

if __name__ == "__main__":
    main()
