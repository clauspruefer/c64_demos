#!/usr/bin/env python3
"""
Generate cylinder coordinates for OpenGL demo.
Creates a virtual rope wrapping around a cylinder with sine wave modulation.
"""

import math

def generate_cylinder_coordinates(num_points=108, radius=8.0, height=30.0):
    """
    Generate coordinates for a virtual rope wrapping around a cylinder.
    The rope goes from top to bottom, wrapping around the cylinder.
    We'll add sine modulations to x, y, z to make it look less static.
    
    Args:
        num_points: Number of vertices to generate
        radius: Base radius of the cylinder
        height: Total height of the cylinder
    
    Returns:
        List of (x, y, z) coordinate tuples
    """
    coordinates = []
    
    for i in range(num_points):
        # Progress from 0 to 1 as we go from top to bottom
        t = i / (num_points - 1)
        
        # Y coordinate goes from top to bottom with larger range
        # Center at 0, range from -15 to 15
        y = height/2 - height * t
        y += 0.5 * math.sin(t * 8 * math.pi)  # Vertical sine wave
        
        # Angle around the cylinder - the rope wraps around
        # Multiple wraps as we go down (e.g., 4-5 full rotations)
        wraps = 4.5
        angle = t * wraps * 2 * math.pi
        
        # Add sine modulation to the angle for less static appearance
        angle += 0.3 * math.sin(t * 5 * math.pi)
        
        # X and Z form a circle, but with sine modulations
        # Add varying radius for organic look
        radius_mod = radius + 1.0 * math.sin(t * 7 * math.pi)
        
        x = radius_mod * math.cos(angle)
        z = radius_mod * math.sin(angle)
        
        # Add additional sine perturbations to x and z
        x += 0.5 * math.sin(t * 6 * math.pi + angle)
        z += 0.5 * math.cos(t * 6 * math.pi + angle)
        
        coordinates.append((int(round(x)), int(round(y)), int(round(z))))
    
    return coordinates


def print_cpp_array(coords, name="vertices"):
    """Print coordinates in C++ array format."""
    print(f"const int NUM_VERTICES = {len(coords)};")
    print()
    print(f"GLint {name}[NUM_VERTICES][3] = {{")
    for i, (x, y, z) in enumerate(coords):
        if i % 4 == 0:
            print("  ", end="")
        print(f"{{{x:3d}, {y:3d}, {z:3d}}}", end="")
        if i < len(coords) - 1:
            print(", ", end="")
            if (i + 1) % 4 == 0:
                print()
        else:
            print()
    print("};")
    print()
    print(f"// Generated {len(coords)} vertices for cylinder with sine wrapping")
    print(f"// Radius: ~{radius} units, Height: {height} units")


if __name__ == "__main__":
    # Default parameters for the cylinder
    num_vertices = 108
    radius = 8.0
    height = 30.0
    
    # Generate coordinates
    coords = generate_cylinder_coordinates(num_vertices, radius, height)
    
    # Print in C++ format
    print_cpp_array(coords)
    
    # Print statistics
    print(f"// Average Y-spacing: {height/(num_vertices-1):.3f} units")
