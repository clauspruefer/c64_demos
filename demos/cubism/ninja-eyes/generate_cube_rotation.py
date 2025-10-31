#!/usr/bin/env python3
"""
Generate 3D cube rotation data for C64 demo.
Creates rotation frames for morphing eye pupils into rotating cubes.
"""

import math

# Constants
EPSILON = 0.01  # Small value to avoid division by zero

# Cube vertices (centered at origin)
cube_vertices = [
    [-1, -1, -1], [1, -1, -1], [1, 1, -1], [-1, 1, -1],  # back face
    [-1, -1, 1], [1, -1, 1], [1, 1, 1], [-1, 1, 1]       # front face
]

# Cube edges (pairs of vertex indices)
cube_edges = [
    [0, 1], [1, 2], [2, 3], [3, 0],  # back face
    [4, 5], [5, 6], [6, 7], [7, 4],  # front face
    [0, 4], [1, 5], [2, 6], [3, 7]   # connecting edges
]

def rotate_x(point, angle):
    """Rotate point around X axis"""
    y = point[1] * math.cos(angle) - point[2] * math.sin(angle)
    z = point[1] * math.sin(angle) + point[2] * math.cos(angle)
    return [point[0], y, z]

def rotate_y(point, angle):
    """Rotate point around Y axis"""
    x = point[0] * math.cos(angle) + point[2] * math.sin(angle)
    z = -point[0] * math.sin(angle) + point[2] * math.cos(angle)
    return [x, point[1], z]

def rotate_z(point, angle):
    """Rotate point around Z axis"""
    x = point[0] * math.cos(angle) - point[1] * math.sin(angle)
    y = point[0] * math.sin(angle) + point[1] * math.cos(angle)
    return [x, y, point[2]]

def project_3d_to_2d(point, distance=4):
    """Simple perspective projection"""
    if point[2] + distance == 0:
        point[2] = EPSILON
    factor = distance / (distance + point[2])
    x = int(point[0] * factor * 3 + 12)  # scale and center (12x8 area for pupil)
    y = int(point[1] * factor * 3 + 4)
    return [x, y]

def generate_cube_frame(angle_x, angle_y, angle_z):
    """Generate one frame of cube rotation"""
    rotated_vertices = []
    for vertex in cube_vertices:
        point = rotate_x(vertex, angle_x)
        point = rotate_y(point, angle_y)
        point = rotate_z(point, angle_z)
        rotated_vertices.append(point)
    
    # Project to 2D
    projected = [project_3d_to_2d(v) for v in rotated_vertices]
    return projected

def vertex_to_byte_data(vertices, edges):
    """Convert vertices and edges to byte data for C64"""
    data = []
    
    # Store vertices (x, y pairs)
    for v in vertices:
        # Clamp to reasonable ranges (0-23 for x, 0-7 for y for pupil area)
        x = max(0, min(23, v[0]))
        y = max(0, min(7, v[1]))
        data.append(x)
        data.append(y)
    
    return data

# Generate rotation frames
num_frames = 64
frames_data = []

print("; Cube rotation frame data - 64 frames")
print("; Each frame: 8 vertices * 2 bytes (x,y) = 16 bytes per frame")
print()

for i in range(num_frames):
    angle = (i / num_frames) * 2 * math.pi
    # Rotate around multiple axes for interesting effect
    vertices = generate_cube_frame(angle * 0.7, angle, angle * 0.5)
    frame_data = vertex_to_byte_data(vertices, cube_edges)
    frames_data.extend(frame_data)

# Write to include file
with open('cube-rotation-data.i', 'w') as f:
    f.write('; Cube rotation data - 64 frames, 16 bytes each\n')
    f.write('; Format: x0,y0, x1,y1, ..., x7,y7 for each frame\n')
    f.write('\n')
    
    for frame_idx in range(num_frames):
        f.write(f'; Frame {frame_idx}\n')
        f.write('!byte ')
        frame_start = frame_idx * 16
        frame_bytes = frames_data[frame_start:frame_start + 16]
        f.write(','.join([f'${b:02x}' for b in frame_bytes]))
        f.write('\n')

print(f"Generated {num_frames} rotation frames")
print("Output written to: cube-rotation-data.i")
