#!/usr/bin/env python3
"""
Generate double helix coordinates for OpenGL demo.
Creates two virtual ropes wrapping around a cylinder with 180° phase offset.
Reduced to 18 vertices total (9 per strand) for C64 demo performance.
"""

import math

def generate_double_helix_coordinates(num_points_per_strand=9, radius=8.0, height=30.0):
    """
    Generate coordinates for a double helix (two intertwined ropes).
    
    Args:
        num_points_per_strand: Number of vertices per strand (default: 9)
        radius: Base radius of the cylinder
        height: Total height of the cylinder
    
    Returns:
        Tuple of two lists of (x, y, z) coordinate tuples (strand1, strand2)
    """
    strand1 = []
    strand2 = []
    
    for i in range(num_points_per_strand):
        # Progress from 0 to 1 as we go from top to bottom
        t = i / (num_points_per_strand - 1)
        
        # Y coordinate goes from top to bottom
        # Center at 0, range from -15 to 15
        y = height/2 - height * t
        y += 0.5 * math.sin(t * 8 * math.pi)  # Vertical sine wave
        
        # Angle around the cylinder - the rope wraps around
        wraps = 4.5
        angle = t * wraps * 2 * math.pi
        
        # Add sine modulation to the angle for less static appearance
        angle += 0.3 * math.sin(t * 5 * math.pi)
        
        # X and Z form a circle, but with sine modulations
        # Add varying radius for organic look
        radius_mod = radius + 1.0 * math.sin(t * 7 * math.pi)
        
        # First strand
        x1 = radius_mod * math.cos(angle)
        z1 = radius_mod * math.sin(angle)
        x1 += 0.5 * math.sin(t * 6 * math.pi + angle)
        z1 += 0.5 * math.cos(t * 6 * math.pi + angle)
        
        # Second strand - 180° phase offset (π radians)
        x2 = radius_mod * math.cos(angle + math.pi)
        z2 = radius_mod * math.sin(angle + math.pi)
        x2 += 0.5 * math.sin(t * 6 * math.pi + angle + math.pi)
        z2 += 0.5 * math.cos(t * 6 * math.pi + angle + math.pi)
        
        strand1.append((int(round(x1)), int(round(y)), int(round(z1))))
        strand2.append((int(round(x2)), int(round(y)), int(round(z2))))
    
    return strand1, strand2


def print_cpp_arrays(strand1, strand2):
    """Print coordinates in C++ array format."""
    total = len(strand1) + len(strand2)
    print(f"const int NUM_VERTICES = {total};")
    print()
    print(f"GLint vertices[NUM_VERTICES][3] = {{")
    
    all_coords = []
    # Interleave the two strands (makes corresponding points easier to visualize)
    for i in range(len(strand1)):
        all_coords.append(strand1[i])
        all_coords.append(strand2[i])
    
    for i, (x, y, z) in enumerate(all_coords):
        if i % 2 == 0:
            print("  ", end="")
        print(f"{{{x:3d}, {y:3d}, {z:3d}}}", end="")
        if i < len(all_coords) - 1:
            print(", ", end="")
            if (i + 1) % 2 == 0:
                print()
        else:
            print()
    print("};")
    print()
    print(f"// Generated {total} vertices for double helix (2 strands x {len(strand1)} vertices)")
    print(f"// Strands are interleaved: strand1[0], strand2[0], strand1[1], strand2[1], ...")
    print(f"// Animation: 11.25 degrees per frame = 32 frames for complete 360° rotation")


if __name__ == "__main__":
    # Generate double helix with 9 vertices per strand (18 total)
    strand1, strand2 = generate_double_helix_coordinates(9, 8.0, 30.0)
    
    # Print in C++ format
    print_cpp_arrays(strand1, strand2)
