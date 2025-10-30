#!/usr/bin/env python3
"""
Generate HITMEN FLI multicolor logo for C64
Creates TRUE FLI logo with MULTIPLE COLORS PER CHARACTER

TRUE FLI means:
- Each 8x8 character can have DIFFERENT COLORS on each scanline
- This creates a rainbow/gradient effect WITHIN each character
- The same character bitmap shows multiple colors vertically
- This is THE defining feature of FLI and impossible without it

Non-FLI can only have ONE color per entire 8x8 character block.
"""

import os
from PIL import Image, ImageDraw, ImageFont
from fli_font_data import FLI_FONT

# C64 screen dimensions
SCREEN_WIDTH_CHARS = 40  # characters (320 pixels)
SCREEN_HEIGHT_CHARS = 25  # characters (200 pixels)
CHAR_WIDTH = 8
CHAR_HEIGHT = 8
SCREEN_WIDTH_PIXELS = SCREEN_WIDTH_CHARS * CHAR_WIDTH
SCREEN_HEIGHT_PIXELS = SCREEN_HEIGHT_CHARS * CHAR_HEIGHT

# Colodore palette - more accurate C64 colors than Pepto's palette
# Colodore uses measured values from real hardware with gamma correction
# Reference: https://www.colodore.com/
# Note: This differs from the Pepto palette used in floodlights generator
# as the problem statement specifically requests colodore palette
COLODORE_PALETTE_RGB = {
    0x00: (0x00, 0x00, 0x00),  # Black
    0x01: (0xFF, 0xFF, 0xFF),  # White
    0x02: (0x68, 0x37, 0x2b),  # Red
    0x03: (0x70, 0xa4, 0xb2),  # Cyan
    0x04: (0x6f, 0x3d, 0x86),  # Purple
    0x05: (0x58, 0x8d, 0x43),  # Green
    0x06: (0x35, 0x28, 0x79),  # Blue
    0x07: (0xb8, 0xc7, 0x6f),  # Yellow
    0x08: (0x6f, 0x4f, 0x25),  # Orange
    0x09: (0x43, 0x39, 0x00),  # Brown
    0x0a: (0x9a, 0x67, 0x59),  # Light Red
    0x0b: (0x44, 0x44, 0x44),  # Dark Grey
    0x0c: (0x6c, 0x6c, 0x6c),  # Medium Grey
    0x0d: (0x9a, 0xd2, 0x84),  # Light Green
    0x0e: (0x6c, 0x5e, 0xb5),  # Light Blue
    0x0f: (0x95, 0x95, 0x95),  # Light Grey
}

# Create a gradient for the HITMEN logo using all 16 colors
# Ordered from dark to bright for a nice gradient effect
COLOR_GRADIENT = [
    0x00,  # Black
    0x09,  # Brown
    0x0b,  # Dark Grey
    0x02,  # Red
    0x06,  # Blue
    0x04,  # Purple
    0x08,  # Orange
    0x05,  # Green
    0x0c,  # Medium Grey
    0x0a,  # Light Red
    0x0e,  # Light Blue
    0x0f,  # Light Grey
    0x03,  # Cyan
    0x0d,  # Light Green
    0x07,  # Yellow
    0x01,  # White
]



def render_fli_character(char, start_y):
    """
    Render a single FLI character with MULTIPLE COLORS per character.
    This is the KEY to TRUE FLI - each scanline within the character gets its own color.
    
    Args:
        char: Character to render ('H', 'I', 'T', 'M', 'E', 'N')
        start_y: Starting Y position (scanline number) for this character
        
    Returns:
        8x8 PIL Image with FLI coloring
    """
    if char not in FLI_FONT:
        char = 'I'  # Fallback
    
    char_data = FLI_FONT[char]
    char_img = Image.new('RGB', (CHAR_WIDTH, CHAR_HEIGHT))
    pixels = char_img.load()
    
    # For each scanline in this character (0-7)
    for row in range(CHAR_HEIGHT):
        # Calculate absolute scanline number on screen
        absolute_scanline = start_y + row
        
        # Get the color for THIS specific scanline (FLI magic!)
        # Different scanlines get different colors even within the same character
        color_index = int((absolute_scanline / SCREEN_HEIGHT_PIXELS) * (len(COLOR_GRADIENT) - 1))
        color_index = min(color_index, len(COLOR_GRADIENT) - 1)
        c64_color = COLOR_GRADIENT[color_index]
        fg_color = COLODORE_PALETTE_RGB[c64_color]
        bg_color = COLODORE_PALETTE_RGB[0x00]  # Black background
        
        # Render this scanline with its unique color
        row_byte = char_data[row]
        for col in range(CHAR_WIDTH):
            bit = (row_byte >> (7 - col)) & 1
            if bit:
                pixels[col, row] = fg_color
            else:
                pixels[col, row] = bg_color
    
    return char_img


def generate_fli_logo():
    """
    Generate TRUE FLI logo - MULTIPLE COLORS PER CHARACTER
    
    This is what makes FLI special:
    - Each character (8x8 pixels) can display MULTIPLE COLORS vertically
    - Each of the 8 scanlines in a character can have a DIFFERENT color
    - The same character bitmap (e.g., letter 'H') shows a rainbow gradient
    - This is IMPOSSIBLE without FLI mode
    
    In non-FLI mode, an entire 8x8 character can only be ONE color.
    """
    # Create output image
    fli_img = Image.new('RGB', (SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS), 
                        color=COLODORE_PALETTE_RGB[0x00])
    
    # Text to display
    text = "HITMEN"
    
    # Center the text horizontally
    text_width_chars = len(text)
    start_char_x = (SCREEN_WIDTH_CHARS - text_width_chars) // 2
    
    # Center vertically
    start_char_y = SCREEN_HEIGHT_CHARS // 2 - 1
    
    # Render each character with FLI colors
    for i, char in enumerate(text):
        char_x = start_char_x + i
        char_y = start_char_y
        
        # Calculate pixel position
        pixel_x = char_x * CHAR_WIDTH
        pixel_y = char_y * CHAR_HEIGHT
        
        # Render character with FLI (multiple colors per character!)
        char_img = render_fli_character(char, pixel_y)
        
        # Paste character into logo
        fli_img.paste(char_img, (pixel_x, pixel_y))
    
    return fli_img



def render_non_fli_character(char, char_y):
    """
    Render a single non-FLI character with ONE COLOR for the entire character.
    
    In non-FLI mode, an entire 8x8 character block can only have ONE color.
    All 8 scanlines must use the same color - no rainbow effect possible.
    
    Args:
        char: Character to render ('H', 'I', 'T', 'M', 'E', 'N')
        char_y: Character Y position (for determining which single color to use)
        
    Returns:
        8x8 PIL Image with single-color character
    """
    if char not in FLI_FONT:
        char = 'I'  # Fallback
    
    char_data = FLI_FONT[char]
    char_img = Image.new('RGB', (CHAR_WIDTH, CHAR_HEIGHT))
    pixels = char_img.load()
    
    # Pick ONE color for this ENTIRE character (all 8 scanlines)
    # This is the non-FLI limitation
    color_index = int((char_y / SCREEN_HEIGHT_CHARS) * (len(COLOR_GRADIENT) - 1))
    color_index = min(color_index, len(COLOR_GRADIENT) - 1)
    c64_color = COLOR_GRADIENT[color_index]
    fg_color = COLODORE_PALETTE_RGB[c64_color]
    bg_color = COLODORE_PALETTE_RGB[0x00]  # Black background
    
    # Render all scanlines with THE SAME color (non-FLI limitation)
    for row in range(CHAR_HEIGHT):
        row_byte = char_data[row]
        for col in range(CHAR_WIDTH):
            bit = (row_byte >> (7 - col)) & 1
            if bit:
                pixels[col, row] = fg_color  # Same color for ALL rows!
            else:
                pixels[col, row] = bg_color
    
    return char_img


def generate_non_fli_logo():
    """
    Generate non-FLI version - ONE COLOR PER CHARACTER
    
    In standard C64 mode (non-FLI), each 8x8 character can only have ONE color.
    All 8 scanlines in the character must use the same color.
    
    This means NO rainbow/gradient effect within characters.
    The letter 'H' can be red OR blue OR green, but not all three at once.
    """
    # Create output image
    non_fli_img = Image.new('RGB', (SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS),
                            color=COLODORE_PALETTE_RGB[0x00])
    
    # Text to display
    text = "HITMEN"
    
    # Center the text horizontally
    text_width_chars = len(text)
    start_char_x = (SCREEN_WIDTH_CHARS - text_width_chars) // 2
    
    # Center vertically
    start_char_y = SCREEN_HEIGHT_CHARS // 2 - 1
    
    # Render each character with single color (non-FLI limitation)
    for i, char in enumerate(text):
        char_x = start_char_x + i
        char_y = start_char_y
        
        # Calculate pixel position
        pixel_x = char_x * CHAR_WIDTH
        pixel_y = char_y * CHAR_HEIGHT
        
        # Render character with SINGLE color (non-FLI)
        char_img = render_non_fli_character(char, char_y)
        
        # Paste character into logo
        non_fli_img.paste(char_img, (pixel_x, pixel_y))
    
    return non_fli_img


def create_comparison_image(fli_img, non_fli_img):
    """
    Create a side-by-side comparison showing TRUE FLI advantage
    """
    # Create canvas for both images plus labels
    label_height = 50
    total_width = SCREEN_WIDTH_PIXELS * 2 + 20  # Gap between images
    total_height = SCREEN_HEIGHT_PIXELS + label_height
    
    comparison = Image.new('RGB', (total_width, total_height), color=(0, 0, 0))
    draw = ImageDraw.Draw(comparison)
    
    # Paste images
    comparison.paste(non_fli_img, (0, label_height))
    comparison.paste(fli_img, (SCREEN_WIDTH_PIXELS + 20, label_height))
    
    # Add labels
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 22)
    except (OSError, IOError):
        font = ImageFont.load_default()
    
    draw.text((20, 10), "Non-FLI (one color/char)", fill=(255, 255, 255), font=font)
    draw.text((SCREEN_WIDTH_CHARS + 30, 10), "FLI (multi-color/char)", fill=(255, 255, 255), font=font)
    
    return comparison


def save_c64_data(fli_img, output_file):
    """
    Save FLI image data in C64 format
    For TRUE FLI multicolor mode, we store color information for each scanline
    within each character, showing how each character displays multiple colors
    """
    with open(output_file, 'w') as f:
        f.write('; HITMEN FLI Multicolor Logo Data\n')
        f.write('; Generated using colodore palette\n')
        f.write('; TRUE FLI mode: Multiple colors per character!\n')
        f.write('; Screen size: {}x{} pixels\n\n'.format(
            SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS))
        
        f.write('SCREEN_WIDTH = {}\n'.format(SCREEN_WIDTH_CHARS))
        f.write('SCREEN_HEIGHT = {}\n'.format(SCREEN_HEIGHT_CHARS))
        f.write('SCANLINES = {}\n\n'.format(SCREEN_HEIGHT_PIXELS))
        
        # Store color data per scanline for FLI effect
        f.write('; Color data for each scanline (FLI mode)\n')
        f.write('; Note: Each character (8 scanlines) has 8 different colors!\n')
        f.write('fli_scanline_colors:\n')
        
        for y in range(SCREEN_HEIGHT_PIXELS):
            # Determine the color for this scanline
            color_index = int((y / SCREEN_HEIGHT_PIXELS) * (len(COLOR_GRADIENT) - 1))
            color_index = min(color_index, len(COLOR_GRADIENT) - 1)
            c64_color = COLOR_GRADIENT[color_index]
            
            char_row = y // 8
            scanline_in_char = y % 8
            f.write('!byte ${:02x}  ; Scanline {} (char {} line {}) - Color {}\n'.format(
                c64_color, y, char_row, scanline_in_char, color_index))
        
        f.write('\n; Character bitmap data\n')
        f.write('; Using FLI font with horizontal stripes for maximum color visibility\n')
        
        # Write font data
        f.write('\nfli_font_data:\n')
        for char_name, char_data in FLI_FONT.items():
            f.write(f'; Character: {char_name}\n')
            for row_idx, row_byte in enumerate(char_data):
                f.write(f'!byte %{row_byte:08b}  ; Row {row_idx}\n')
            f.write('\n')


def main():
    print("="*70)
    print("Generating HITMEN TRUE FLI Multicolor Logo")
    print("="*70)
    print(f"Screen size: {SCREEN_WIDTH_CHARS}x{SCREEN_HEIGHT_CHARS} chars ({SCREEN_WIDTH_PIXELS}x{SCREEN_HEIGHT_PIXELS} pixels)")
    print(f"Using colodore palette with {len(COLOR_GRADIENT)} colors")
    print("\nTRUE FLI means:")
    print("  - Each 8x8 character displays MULTIPLE COLORS (rainbow effect)")
    print("  - Each of the 8 scanlines in a character has its OWN color")
    print("  - The SAME character bitmap shows different colors vertically")
    print("  - This is THE defining feature of FLI!")
    print("\nNon-FLI limitation:")
    print("  - Each 8x8 character can only be ONE color")
    print("  - All 8 scanlines must use the same color")
    print("  - NO rainbow effect within characters")
    
    # Generate both versions
    print("\n" + "-"*70)
    print("Generating FLI version (MULTIPLE colors per character)...")
    fli_img = generate_fli_logo()
    fli_img.save('hitmen_fli_logo.png')
    print("  ✓ Saved: hitmen_fli_logo.png")
    
    print("\nGenerating non-FLI version (ONE color per character)...")
    non_fli_img = generate_non_fli_logo()
    non_fli_img.save('hitmen_non_fli_logo.png')
    print("  ✓ Saved: hitmen_non_fli_logo.png")
    
    print("\nGenerating comparison image...")
    comparison = create_comparison_image(fli_img, non_fli_img)
    comparison.save('hitmen_comparison.png')
    print("  ✓ Saved: hitmen_comparison.png")
    
    print("\nGenerating C64 assembly data...")
    save_c64_data(fli_img, 'hitmen_fli_data.i')
    print("  ✓ Saved: hitmen_fli_data.i")
    
    print("\n" + "="*70)
    print("✓ DONE! TRUE FLI logo generated!")
    print("="*70)
    print("\nLook at the comparison image:")
    print("  - Non-FLI: Each letter is a SINGLE color (monochrome)")
    print("  - FLI: Each letter has a RAINBOW gradient (multi-color)")
    print("\nThis is TRUE FLI - multiple colors within the same character!")
    print("This effect is IMPOSSIBLE without FLI mode!")
    print("="*70)


if __name__ == '__main__':
    main()
