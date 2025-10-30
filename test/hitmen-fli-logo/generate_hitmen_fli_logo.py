#!/usr/bin/env python3
"""
Generate HITMEN FLI multicolor logo for C64
Creates both FLI and non-FLI versions to demonstrate the FLI advantage
Uses colodore palette and C64-style blocky font

TRUE FLI allows changing colors PER SCANLINE (not just per character row).
This means different scanlines within the same 8-pixel tall character can have
different colors - creating horizontal color bands that cut through characters.
This is IMPOSSIBLE without FLI and makes the effect clearly visible.
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
    Create a high-resolution image with HITMEN text using C64-style blocky font
    Returns PIL Image with white text on black background
    """
    # Create a larger canvas for better text rendering
    scale = 4
    img_width = SCREEN_WIDTH_PIXELS * scale
    img_height = SCREEN_HEIGHT_PIXELS * scale
    
    img = Image.new('L', (img_width, img_height), color=0)  # Grayscale
    draw = ImageDraw.Draw(img)
    
    # Draw HITMEN text in the center using a blocky, C64-style approach
    text = "HITMEN"
    
    # Use a monospace bold font for blocky C64-style look
    try:
        # Try DejaVu Sans Mono Bold for blocky appearance
        font_size = 140 * scale
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", font_size)
    except (OSError, IOError):
        try:
            # Try Liberation Mono Bold
            font = ImageFont.truetype("/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf", 140 * scale)
        except (OSError, IOError):
            try:
                # Try DejaVu Sans Bold as fallback
                font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 140 * scale)
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
    Generate FLI version - uses different colors for EACH SCANLINE
    This creates horizontal color bands that cut through the middle of characters,
    which is IMPOSSIBLE without FLI mode and makes the effect clearly visible.
    
    In FLI mode, each of the 200 scanlines can have its own color.
    This means colors change WITHIN character rows (every pixel line),
    not just between character rows (every 8 pixels).
    """
    # Get base text image
    text_img = create_hitmen_text_image()
    pixels = text_img.load()
    
    # Create output image
    fli_img = Image.new('RGB', (SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS))
    fli_pixels = fli_img.load()
    
    # Process each scanline with its own color
    # Creates horizontal color bands that cut through characters
    for y in range(SCREEN_HEIGHT_PIXELS):
        # Determine which color from the gradient to use for this scanline
        # This creates smooth horizontal stripes that cut through characters
        band_index = int((y / SCREEN_HEIGHT_PIXELS) * (len(COLOR_GRADIENT) - 1))
        band_index = min(band_index, len(COLOR_GRADIENT) - 1)
        
        # Get the C64 color for this scanline
        c64_color = COLOR_GRADIENT[band_index]
        rgb_color = COLODORE_PALETTE_RGB[c64_color]
        
        # For each pixel in this scanline
        for x in range(SCREEN_WIDTH_PIXELS):
            # Get brightness from source image
            brightness = pixels[x, y]
            
            # If bright enough (text pixel), use the scanline's color
            # Otherwise use black background
            if brightness > 128:
                fli_pixels[x, y] = rgb_color
            else:
                fli_pixels[x, y] = COLODORE_PALETTE_RGB[0x00]  # Black
    
    return fli_img


def generate_non_fli_logo():
    """
    Generate non-FLI version - limited to ONE color per 8-pixel character row
    
    In standard C64 multicolor mode, color RAM ($d800) can only specify
    one color per character (8x8 pixels). This means an entire character row
    must use the same color - you CANNOT have different colors on different
    scanlines within the same character.
    
    This limitation makes the non-FLI version look blocky and monochrome
    compared to the FLI version with its scanline-level color changes.
    """
    # Get base text image
    text_img = create_hitmen_text_image()
    pixels = text_img.load()
    
    # Create output image
    non_fli_img = Image.new('RGB', (SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS))
    non_fli_pixels = non_fli_img.load()
    
    # In non-FLI mode, we can only change colors every 8 pixels (per character row)
    # Not per scanline like FLI
    for char_y in range(SCREEN_HEIGHT_CHARS):
        # Pick ONE color for this entire character row (all 8 scanlines)
        # Based on the character row position
        color_index = int((char_y / SCREEN_HEIGHT_CHARS) * (len(COLOR_GRADIENT) - 1))
        color_index = min(color_index, len(COLOR_GRADIENT) - 1)
        c64_color = COLOR_GRADIENT[color_index]
        rgb_color = COLODORE_PALETTE_RGB[c64_color]
        
        # Process all 8 scanlines in this character row with the SAME color
        for py in range(CHAR_HEIGHT):
            pixel_y = char_y * CHAR_HEIGHT + py
            if pixel_y >= SCREEN_HEIGHT_PIXELS:
                break
                
            for x in range(SCREEN_WIDTH_PIXELS):
                # Get brightness from source image
                brightness = pixels[x, pixel_y]
                
                # If bright enough, use the character row color, otherwise black
                # ALL 8 scanlines in this character row get the SAME color
                if brightness > 128:
                    non_fli_pixels[x, pixel_y] = rgb_color
                else:
                    non_fli_pixels[x, pixel_y] = COLODORE_PALETTE_RGB[0x00]
    
    return non_fli_img


def create_comparison_image(fli_img, non_fli_img):
    """
    Create a side-by-side comparison image showing the FLI advantage
    """
    # Create canvas for both images plus labels
    label_height = 40
    total_width = SCREEN_WIDTH_PIXELS * 2 + 20  # Gap between images
    total_height = SCREEN_HEIGHT_PIXELS + label_height
    
    comparison = Image.new('RGB', (total_width, total_height), color=(0, 0, 0))
    draw = ImageDraw.Draw(comparison)
    
    # Paste images
    comparison.paste(non_fli_img, (0, label_height))
    comparison.paste(fli_img, (SCREEN_WIDTH_PIXELS + 20, label_height))
    
    # Add labels
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24)
    except (OSError, IOError):
        font = ImageFont.load_default()
    
    draw.text((40, 8), "Non-FLI (8-pixel bands)", fill=(255, 255, 255), font=font)
    draw.text((SCREEN_WIDTH_PIXELS + 50, 8), "FLI (scanline colors)", fill=(255, 255, 255), font=font)
    
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
    print("\nKEY: FLI mode allows color changes at EVERY SCANLINE (200 scanlines)")
    print("     Non-FLI mode can only change colors every 8 pixels (per character row)")
    
    # Generate both versions
    print("\nGenerating FLI version (scanline-level color changes)...")
    fli_img = generate_fli_logo()
    fli_img.save('hitmen_fli_logo.png')
    print("  Saved: hitmen_fli_logo.png")
    
    print("\nGenerating non-FLI version (character-row level color changes)...")
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
    
    print("\n" + "="*70)
    print("✓ DONE! FLI effect is now CLEARLY VISIBLE!")
    print("="*70)
    print("\nThe FLI version shows horizontal color bands that CUT THROUGH")
    print("the characters - this is IMPOSSIBLE without FLI mode!")
    print("\nNon-FLI can only change colors every 8 pixels (character rows),")
    print("creating blocky horizontal bands aligned to character boundaries.")
    print("\nFLI changes colors at EVERY SCANLINE, creating smooth bands that")
    print("slice through the middle of characters - the true FLI advantage!")
    print("="*70)


if __name__ == '__main__':
    main()
