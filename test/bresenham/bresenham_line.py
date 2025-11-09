#!/usr/bin/env python3
"""
Bresenham 2D Line Drawing Algorithm
Simple implementation of Bresenham's line algorithm for drawing lines
Lines are drawn on a canvas and exported as PNG image
"""

from PIL import Image


class LineDrawer:
    """Simple line drawer using Bresenham's algorithm"""
    
    def __init__(self, width=320, height=200, background_color=(0, 0, 0)):
        """
        Initialize the line drawer with a canvas
        
        Args:
            width: Canvas width in pixels
            height: Canvas height in pixels
            background_color: RGB tuple for background color
        """
        self.width = width
        self.height = height
        self.background_color = background_color
        self.image = Image.new('RGB', (width, height), background_color)
        self.pixels = self.image.load()
    
    def draw_line(self, x0, y0, x1, y1, color=(255, 255, 255)):
        """
        Draw a line from (x0, y0) to (x1, y1) using Bresenham's algorithm
        
        Args:
            x0: Starting x coordinate
            y0: Starting y coordinate
            x1: Ending x coordinate
            y1: Ending y coordinate
            color: RGB tuple for line color
        """
        # Convert to integers
        x0, y0, x1, y1 = int(x0), int(y0), int(x1), int(y1)
        
        # Calculate differences
        dx = abs(x1 - x0)
        dy = abs(y1 - y0)
        
        # Determine direction
        sx = 1 if x0 < x1 else -1
        sy = 1 if y0 < y1 else -1
        
        # Initialize error
        err = dx - dy
        
        # Current position
        x, y = x0, y0
        
        # Draw line
        while True:
            # Set pixel if within bounds
            if 0 <= x < self.width and 0 <= y < self.height:
                self.pixels[x, y] = color
            
            # Check if we've reached the end
            if x == x1 and y == y1:
                break
            
            # Calculate error and adjust coordinates
            e2 = 2 * err
            
            if e2 > -dy:
                err -= dy
                x += sx
            
            if e2 < dx:
                err += dx
                y += sy
    
    def save(self, filename):
        """
        Save the canvas as a PNG file
        
        Args:
            filename: Output filename (should end with .png)
        """
        self.image.save(filename)
        print(f"Image saved to: {filename}")
    
    def clear(self):
        """Clear the canvas to background color"""
        self.image = Image.new('RGB', (self.width, self.height), self.background_color)
        self.pixels = self.image.load()


def main():
    """Demo: Draw several lines using Bresenham's algorithm"""
    
    # Create a 320x200 canvas (C64 resolution)
    drawer = LineDrawer(width=320, height=200, background_color=(0, 0, 0))
    
    print("Drawing lines using Bresenham's algorithm...")
    
    # Draw a box
    drawer.draw_line(50, 50, 270, 50, color=(255, 0, 0))      # Top - Red
    drawer.draw_line(270, 50, 270, 150, color=(0, 255, 0))    # Right - Green
    drawer.draw_line(270, 150, 50, 150, color=(0, 0, 255))    # Bottom - Blue
    drawer.draw_line(50, 150, 50, 50, color=(255, 255, 0))    # Left - Yellow
    
    # Draw diagonals
    drawer.draw_line(50, 50, 270, 150, color=(255, 0, 255))   # Diagonal 1 - Magenta
    drawer.draw_line(270, 50, 50, 150, color=(0, 255, 255))   # Diagonal 2 - Cyan
    
    # Draw a star pattern from center
    center_x, center_y = 160, 100
    drawer.draw_line(center_x, center_y, 160, 20, color=(255, 255, 255))    # Top
    drawer.draw_line(center_x, center_y, 160, 180, color=(255, 255, 255))   # Bottom
    drawer.draw_line(center_x, center_y, 30, 100, color=(255, 255, 255))    # Left
    drawer.draw_line(center_x, center_y, 290, 100, color=(255, 255, 255))   # Right
    
    # Save the result
    drawer.save('bresenham_demo.png')
    print("Done! Created demonstration with multiple lines.")


if __name__ == '__main__':
    main()
