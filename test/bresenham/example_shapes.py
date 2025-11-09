#!/usr/bin/env python3
"""
Example: Drawing custom shapes with Bresenham lines
This demonstrates how to use the LineDrawer class to create various shapes
"""

from bresenham_line import LineDrawer


def draw_triangle(drawer, x1, y1, x2, y2, x3, y3, color=(255, 255, 255)):
    """Draw a triangle given three points"""
    drawer.draw_line(x1, y1, x2, y2, color)
    drawer.draw_line(x2, y2, x3, y3, color)
    drawer.draw_line(x3, y3, x1, y1, color)


def draw_rectangle(drawer, x, y, width, height, color=(255, 255, 255)):
    """Draw a rectangle"""
    drawer.draw_line(x, y, x + width, y, color)
    drawer.draw_line(x + width, y, x + width, y + height, color)
    drawer.draw_line(x + width, y + height, x, y + height, color)
    drawer.draw_line(x, y + height, x, y, color)


def draw_star(drawer, cx, cy, radius, color=(255, 255, 255)):
    """Draw a star pattern from center point"""
    import math
    
    # Draw 8 lines radiating from center
    for i in range(8):
        angle = i * math.pi / 4
        x = int(cx + radius * math.cos(angle))
        y = int(cy + radius * math.sin(angle))
        drawer.draw_line(cx, cy, x, y, color)


if __name__ == '__main__':
    # Create a 400x300 canvas
    drawer = LineDrawer(width=400, height=300, background_color=(0, 0, 32))
    
    print("Drawing custom shapes...")
    
    # Draw a triangle
    draw_triangle(drawer, 50, 50, 150, 50, 100, 150, color=(255, 0, 0))
    
    # Draw a rectangle
    draw_rectangle(drawer, 200, 50, 150, 100, color=(0, 255, 0))
    
    # Draw a star
    draw_star(drawer, 100, 225, 60, color=(255, 255, 0))
    
    # Draw another star
    draw_star(drawer, 300, 225, 50, color=(255, 0, 255))
    
    # Save the result
    drawer.save('custom_shapes.png')
    print("Done! Saved to custom_shapes.png")
