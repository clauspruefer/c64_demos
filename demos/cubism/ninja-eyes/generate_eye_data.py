#!/usr/bin/env python3
"""
Generate eye character and color data for ninja eyes demo.
Creates detailed eye graphics with pupils.
"""

# C64 Color constants
WHITE = 0x01
DARK_GRAY = 0x0b

# Eye is 8 characters wide x 3 characters tall
# Using C64 PETSCII characters to draw the eye

def generate_eye_data():
    """Generate eye character patterns"""
    
    # Eye shape using PETSCII characters
    # Top row - curved top of eye
    eye_top = [
        0x20,  # Space
        0x55,  # Upper-left curve ╔
        0x40,  # Horizontal line ─
        0x40,  # Horizontal line ─
        0x40,  # Horizontal line ─
        0x40,  # Horizontal line ─
        0x49,  # Upper-right curve ╗
        0x20,  # Space
    ]
    
    # Middle row - eye white with pupil in center
    eye_middle = [
        0x55,  # Left edge │
        0x20,  # White space
        0xa0,  # Block ▌ (pupil left)
        0xa0,  # Block █ (pupil center)
        0xa0,  # Block ▐ (pupil right)
        0x20,  # White space
        0x49,  # Right edge │
        0x20,  # Space
    ]
    
    # Bottom row - curved bottom of eye
    eye_bottom = [
        0x20,  # Space
        0x4a,  # Lower-left curve ╚
        0x40,  # Horizontal line ─
        0x40,  # Horizontal line ─
        0x40,  # Horizontal line ─
        0x40,  # Horizontal line ─
        0x4b,  # Lower-right curve ╝
        0x20,  # Space
    ]
    
    # Combine all rows
    eye_chars = eye_top + eye_middle + eye_bottom
    
    # Color data - white for eye, dark gray for pupil
    color_top = [WHITE] * 8
    color_middle = [WHITE, WHITE, DARK_GRAY, DARK_GRAY, DARK_GRAY, WHITE, WHITE, WHITE]
    color_bottom = [WHITE] * 8
    
    eye_colors = color_top + color_middle + color_bottom
    
    return eye_chars, eye_colors

# Generate and write to include file
chars, colors = generate_eye_data()

with open('eye-data.i', 'w') as f:
    f.write('; Eye character data - 3 rows x 8 columns per eye\n')
    f.write('; Top row, middle row (with pupils), bottom row\n')
    f.write('eye_chars:\n')
    
    # Write in groups of 8
    f.write('    ; Top row\n')
    f.write('    !byte ')
    f.write(','.join([f'${c:02x}' for c in chars[0:8]]))
    f.write('\n')
    
    f.write('    ; Middle row (with pupil markers)\n')
    f.write('    !byte ')
    f.write(','.join([f'${c:02x}' for c in chars[8:16]]))
    f.write('\n')
    
    f.write('    ; Bottom row\n')
    f.write('    !byte ')
    f.write(','.join([f'${c:02x}' for c in chars[16:24]]))
    f.write('\n\n')
    
    f.write('eye_colors:\n')
    f.write('    ; Colors: white for eye white, dark gray for pupil area\n')
    f.write('    !byte ')
    f.write(','.join([f'${c:02x}' for c in colors[0:8]]))
    f.write('\n')
    f.write('    !byte ')
    f.write(','.join([f'${c:02x}' for c in colors[8:16]]))
    f.write('\n')
    f.write('    !byte ')
    f.write(','.join([f'${c:02x}' for c in colors[16:24]]))
    f.write('\n')

print("Generated eye data in eye-data.i")
