#!/usr/bin/env python3
"""
Generate HITMEN FLI multicolor logo for C64
Creates both FLI and non-FLI versions to demonstrate the FLI advantage
Uses colodore palette (Pepto's accurate C64 colors)

FLI (Flexible Line Interpretation) allows changing color RAM every scanline,
enabling more colors on screen compared to standard multicolor mode.
"""

import math
import os
from PIL import Image, ImageDraw, ImageFont

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


def create_hitmen_text_image():
    """
    Create a high-resolution image with HITMEN text
    Returns PIL Image with white text on black background
    """
    # Create a larger canvas for better text rendering
    scale = 4
    img_width = SCREEN_WIDTH_PIXELS * scale
    img_height = SCREEN_HEIGHT_PIXELS * scale
    
    img = Image.new('L', (img_width, img_height), color=0)  # Grayscale
    draw = ImageDraw.Draw(img)
    
    # Draw HITMEN text in the center
    # Using basic PIL text rendering
    text = "HITMEN"
    
    # Try to use a bold font, fallback to default
    try:
        # Try common monospace/bold fonts
        font_size = 120 * scale
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", font_size)
    except (OSError, IOError):
        try:
            font = ImageFont.truetype("/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf", 120 * scale)
        except (OSError, IOError):
            # Fallback to default font
            font = ImageFont.load_default()
    
    # Get text bounding box and center it
    bbox = draw.textbbox((0, 0), text, font=font)
    text_width = bbox[2] - bbox[0]
    text_height = bbox[3] - bbox[1]
    
    x = (img_width - text_width) // 2
    y = (img_height - text_height) // 2
    
    # Draw white text
    draw.text((x, y), text, fill=255, font=font)
    
    # Scale down to C64 resolution
    img_scaled = img.resize((SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS), Image.LANCZOS)
    
    return img_scaled


def generate_fli_logo():
    """
    Generate FLI version - uses different colors for each scanline
    This demonstrates the FLI effect with maximum color variation
    """
    # Get base text image
    text_img = create_hitmen_text_image()
    pixels = text_img.load()
    
    # Create output image
    fli_img = Image.new('RGB', (SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS))
    fli_pixels = fli_img.load()
    
    # Process each scanline (8 pixels = 1 char row)
    # In FLI mode, we can change colors every scanline
    for y in range(SCREEN_HEIGHT_PIXELS):
        # Calculate which color from gradient to use based on vertical position
        # This creates a smooth gradient from top to bottom
        color_index = int((y / SCREEN_HEIGHT_PIXELS) * (len(COLOR_GRADIENT) - 1))
        color_index = min(color_index, len(COLOR_GRADIENT) - 1)
        
        # Get the C64 color for this scanline
        c64_color = COLOR_GRADIENT[color_index]
        rgb_color = COLODORE_PALETTE_RGB[c64_color]
        
        # For each pixel in this scanline
        for x in range(SCREEN_WIDTH_PIXELS):
            # Get brightness from source image
            brightness = pixels[x, y]
            
            # If bright enough, use the gradient color, otherwise black
            if brightness > 128:
                fli_pixels[x, y] = rgb_color
            else:
                fli_pixels[x, y] = COLODORE_PALETTE_RGB[0x00]  # Black
    
    return fli_img


def generate_non_fli_logo():
    """
    Generate non-FLI version - limited to 4 colors per 8x8 character block
    This is standard C64 multicolor mode limitation
    """
    # Get base text image
    text_img = create_hitmen_text_image()
    pixels = text_img.load()
    
    # Create output image
    non_fli_img = Image.new('RGB', (SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS))
    non_fli_pixels = non_fli_img.load()
    
    # In non-FLI mode, we can only use one set of colors for the entire screen
    # Let's use a middle section of the gradient to show the limitation
    # We'll pick 4 colors: background + 3 foreground colors
    base_colors = [
        COLOR_GRADIENT[0],   # Black (background)
        COLOR_GRADIENT[5],   # Purple
        COLOR_GRADIENT[10],  # Light Blue
        COLOR_GRADIENT[14],  # Yellow
    ]
    
    # Map each character block (8x8) to one of the 4 colors based on position
    for char_y in range(SCREEN_HEIGHT_CHARS):
        for char_x in range(SCREEN_WIDTH_CHARS):
            # Calculate which color to use based on vertical position
            # This gives us some variation but much less than FLI
            color_index = min(1 + (char_y * 2) // SCREEN_HEIGHT_CHARS, 3)
            c64_color = base_colors[color_index]
            rgb_color = COLODORE_PALETTE_RGB[c64_color]
            
            # Process all pixels in this character block
            for py in range(CHAR_HEIGHT):
                for px in range(CHAR_WIDTH):
                    pixel_x = char_x * CHAR_WIDTH + px
                    pixel_y = char_y * CHAR_HEIGHT + py
                    
                    # Get brightness from source image
                    brightness = pixels[pixel_x, pixel_y]
                    
                    # If bright enough, use the color, otherwise black
                    if brightness > 128:
                        non_fli_pixels[pixel_x, pixel_y] = rgb_color
                    else:
                        non_fli_pixels[pixel_x, pixel_y] = COLODORE_PALETTE_RGB[0x00]
    
    return non_fli_img


def create_comparison_image(fli_img, non_fli_img):
    """
    Create a side-by-side comparison image
    """
    # Create canvas for both images plus labels
    label_height = 30
    total_width = SCREEN_WIDTH_PIXELS * 2 + 20  # Gap between images
    total_height = SCREEN_HEIGHT_PIXELS + label_height
    
    comparison = Image.new('RGB', (total_width, total_height), color=(0, 0, 0))
    draw = ImageDraw.Draw(comparison)
    
    # Paste images
    comparison.paste(non_fli_img, (0, label_height))
    comparison.paste(fli_img, (SCREEN_WIDTH_PIXELS + 20, label_height))
    
    # Add labels
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 20)
    except (OSError, IOError):
        font = ImageFont.load_default()
    
    draw.text((80, 5), "Non-FLI (4 colors)", fill=(255, 255, 255), font=font)
    draw.text((SCREEN_WIDTH_PIXELS + 60, 5), "FLI (16 colors)", fill=(255, 255, 255), font=font)
    
    return comparison


def save_c64_data(fli_img, output_file):
    """
    Save FLI image data in C64 format
    For FLI multicolor mode, we need to store color information for each scanline
    """
    pixels = fli_img.load()
    
    with open(output_file, 'w') as f:
        f.write('; HITMEN FLI Multicolor Logo Data\n')
        f.write('; Generated using colodore palette\n')
        f.write('; FLI mode allows different colors per scanline\n')
        f.write('; Screen size: {}x{} pixels\n\n'.format(
            SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS))
        
        f.write('SCREEN_WIDTH = {}\n'.format(SCREEN_WIDTH_CHARS))
        f.write('SCREEN_HEIGHT = {}\n'.format(SCREEN_HEIGHT_CHARS))
        f.write('SCANLINES = {}\n\n'.format(SCREEN_HEIGHT_PIXELS))
        
        # Store color data per scanline for FLI effect
        f.write('; Color data for each scanline (FLI mode)\n')
        f.write('fli_scanline_colors:\n')
        
        for y in range(SCREEN_HEIGHT_PIXELS):
            # Determine the dominant color for this scanline
            color_index = int((y / SCREEN_HEIGHT_PIXELS) * (len(COLOR_GRADIENT) - 1))
            color_index = min(color_index, len(COLOR_GRADIENT) - 1)
            c64_color = COLOR_GRADIENT[color_index]
            
            f.write('!byte ${:02x}  ; Scanline {} - Color {}\n'.format(
                c64_color, y, color_index))
        
        f.write('\n; Character bitmap data would go here\n')
        f.write('; (Actual character data generation not implemented in this demo)\n')


def main():
    print("Generating HITMEN FLI Multicolor Logo...")
    print(f"Screen size: {SCREEN_WIDTH_CHARS}x{SCREEN_HEIGHT_CHARS} chars ({SCREEN_WIDTH_PIXELS}x{SCREEN_HEIGHT_PIXELS} pixels)")
    print(f"Using colodore palette with {len(COLOR_GRADIENT)} colors")
    
    # Generate both versions
    print("\nGenerating FLI version (16 colors)...")
    fli_img = generate_fli_logo()
    fli_img.save('hitmen_fli_logo.png')
    print("  Saved: hitmen_fli_logo.png")
    
    print("\nGenerating non-FLI version (4 colors)...")
    non_fli_img = generate_non_fli_logo()
    non_fli_img.save('hitmen_non_fli_logo.png')
    print("  Saved: hitmen_non_fli_logo.png")
    
    print("\nGenerating comparison image...")
    comparison = create_comparison_image(fli_img, non_fli_img)
    comparison.save('hitmen_comparison.png')
    print("  Saved: hitmen_comparison.png")
    
    print("\nGenerating C64 assembly data...")
    save_c64_data(fli_img, 'hitmen_fli_data.i')
    print("  Saved: hitmen_fli_data.i")
    
    print("\n✓ Done! FLI effect is clearly visible in the comparison image.")
    print("  The FLI version uses all 16 C64 colors in a smooth gradient")
    print("  The non-FLI version is limited to only 4 colors")


if __name__ == '__main__':
    main()
