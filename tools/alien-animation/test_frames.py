#!/usr/bin/env python3
"""
Test script for alien animation - generates sample frames as PNG images
This allows quick verification without generating the full video
"""

from generate_alien_animation import AlienAnimation
from PIL import Image
import os

def main():
    """Generate test frames"""
    print("Generating test frames...")
    
    # Create output directory
    os.makedirs("test_frames", exist_ok=True)
    
    # Create animation
    anim = AlienAnimation()
    
    # Generate frames at key moments
    test_times = [
        (0.0, "scene1_alien_drinking.png"),
        (4.0, "scene2_human_walking.png"),
        (5.5, "scene3_saucer_moving.png"),
        (7.0, "scene4_burp_effect.png"),
        (9.0, "scene4_burger_flying.png"),
        (12.0, "scene5_fart_tornado.png"),
        (14.0, "scene5_hair_spinning.png"),
    ]
    
    for t, filename in test_times:
        print(f"Generating frame at t={t}s: {filename}")
        frame_array = anim.make_frame(t)
        
        # Convert numpy array to PIL Image
        img = Image.fromarray(frame_array)
        
        # Save
        filepath = os.path.join("test_frames", filename)
        img.save(filepath)
        print(f"  Saved to {filepath}")
    
    print(f"\nTest frames saved to test_frames/")
    print("You can view these images to verify the animation looks correct.")

if __name__ == "__main__":
    main()
