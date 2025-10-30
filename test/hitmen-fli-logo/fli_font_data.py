"""
FLI font character data for HITMEN logo
Based on C64 FLI logo style with horizontal stripes in each character
Each character is 8x8 pixels, with horizontal lines to enable FLI color changes
"""

# FLI font - each character has horizontal stripes
# Format: 8 bytes per character, each byte is one row (scanline)
# 1 = foreground, 0 = background
# Designed to show multiple colors per character with FLI

FLI_FONT = {
    'H': [
        0b11111111,  # ████████
        0b11000011,  # ██    ██
        0b11000011,  # ██    ██
        0b11111111,  # ████████
        0b11111111,  # ████████
        0b11000011,  # ██    ██
        0b11000011,  # ██    ██
        0b11111111,  # ████████
    ],
    'I': [
        0b11111111,  # ████████
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b11111111,  # ████████
    ],
    'T': [
        0b11111111,  # ████████
        0b11111111,  # ████████
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
        0b00011000,  #    ██
    ],
    'M': [
        0b11111111,  # ████████
        0b11011011,  # ██ ██ ██
        0b11111111,  # ████████
        0b11111111,  # ████████
        0b11010011,  # ██ █  ██
        0b11000011,  # ██    ██
        0b11000011,  # ██    ██
        0b11111111,  # ████████
    ],
    'E': [
        0b11111111,  # ████████
        0b11111111,  # ████████
        0b11000000,  # ██
        0b11111100,  # ██████
        0b11111100,  # ██████
        0b11000000,  # ██
        0b11111111,  # ████████
        0b11111111,  # ████████
    ],
    'N': [
        0b11111111,  # ████████
        0b11100011,  # ███   ██
        0b11110011,  # ████  ██
        0b11111011,  # █████ ██
        0b11011111,  # ██ █████
        0b11001111,  # ██  ████
        0b11000111,  # ██   ███
        0b11111111,  # ████████
    ],
}
