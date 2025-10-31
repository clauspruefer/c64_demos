#!/usr/bin/env python3
"""
ASCII Visualization of the Halloween Demo
Shows a text-based preview of what the C64 demo would display
"""

import time
import os
import math

# ANSI color codes for terminal
class Colors:
    BLACK = '\033[40m'
    ORANGE = '\033[48;5;208m'
    PURPLE = '\033[48;5;93m'
    GREEN = '\033[48;5;28m'
    RED = '\033[41m'
    RESET = '\033[0m'
    ORANGE_TEXT = '\033[38;5;208m'
    BOLD = '\033[1m'

def clear_screen():
    """Clear the terminal screen"""
    os.system('clear' if os.name != 'nt' else 'cls')

def draw_pumpkin(x, y, frame_buffer):
    """Draw a simple ASCII pumpkin at position (x, y)"""
    pumpkin = [
        "  _|_  ",
        " /___\\ ",
        "|o   o|",
        "|  ^  |",
        "| \\_/ |",
        " \\___/ "
    ]
    
    for i, line in enumerate(pumpkin):
        row = y + i
        if 0 <= row < len(frame_buffer) and 0 <= x < 80:
            for j, char in enumerate(line):
                col = x + j
                if 0 <= col < 80 and char != ' ':
                    frame_buffer[row][col] = char

def draw_raster_bar(row, color_index, frame_buffer):
    """Draw a colored raster bar at the given row"""
    # Halloween color sequence
    colors = [Colors.BLACK, Colors.ORANGE, Colors.RED, Colors.ORANGE, 
              Colors.BLACK, Colors.PURPLE, Colors.GREEN, Colors.PURPLE]
    
    color = colors[color_index % len(colors)]
    for col in range(80):
        frame_buffer[row][col] = (color, ' ')

def main():
    """Main visualization loop"""
    print(f"{Colors.BOLD}{Colors.ORANGE_TEXT}")
    print("=" * 80)
    print("C64 HALLOWEEN DEMO VISUALIZATION".center(80))
    print("Cubism Part 4 - ASCII Preview".center(80))
    print("=" * 80)
    print(f"{Colors.RESET}\n")
    
    time.sleep(2)
    
    frame = 0
    try:
        while True:
            clear_screen()
            
            # Create frame buffer (25 rows, 80 cols for C64-like dimensions)
            frame_buffer = [[' ' for _ in range(80)] for _ in range(25)]
            
            # Draw title
            title = "HAPPY HALLOWEEN 2025!"
            start_col = (80 - len(title)) // 2
            for i, char in enumerate(title):
                frame_buffer[5][start_col + i] = (Colors.ORANGE_TEXT, char)
            
            subtitle = "SPOOKY DEMO BY TWF"
            start_col = (80 - len(subtitle)) // 2
            for i, char in enumerate(subtitle):
                frame_buffer[7][start_col + i] = (Colors.ORANGE_TEXT, char)
            
            # Calculate pumpkin positions using sine waves
            # Four pumpkins with different phase offsets
            positions = []
            for i in range(4):
                phase = frame + (i * 64)
                x = int(35 + 30 * math.sin(phase * 0.02))
                y = int(12 + 3 * math.sin(phase * 0.04))
                positions.append((x, y))
            
            # Draw pumpkins
            for x, y in positions:
                draw_pumpkin(x, y, frame_buffer)
            
            # Draw raster bars in background
            for row in range(10, 20):
                color_idx = (frame // 2 + row) % 8
                draw_raster_bar(row, color_idx, frame_buffer)
            
            # Render frame buffer
            print(f"{Colors.BLACK}")
            for row in frame_buffer:
                for cell in row:
                    if isinstance(cell, tuple):
                        color, char = cell
                        print(f"{color}{char}{Colors.RESET}", end='')
                    else:
                        print(f"{Colors.ORANGE_TEXT}{cell}{Colors.RESET}", end='')
                print()
            
            # Status line
            print(f"\n{Colors.BOLD}Frame: {frame:4d}  |  Press Ctrl+C to exit{Colors.RESET}")
            
            time.sleep(0.1)
            frame += 1
            
    except KeyboardInterrupt:
        print(f"\n\n{Colors.ORANGE_TEXT}Happy Halloween! 🎃{Colors.RESET}\n")

if __name__ == '__main__':
    main()
