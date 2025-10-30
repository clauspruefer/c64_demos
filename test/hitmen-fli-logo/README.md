# HITMEN FLI Multicolor Logo Generator

This tool generates a "HITMEN" logo for the Commodore 64 demonstrating the FLI (Flexible Line Interpretation) graphics mode effect.

## What is FLI?

FLI (Flexible Line Interpretation) is a C64 graphics technique that allows changing color registers on every scanline, enabling more colors on screen compared to standard modes:

- **Non-FLI mode**: Limited to 4 colors per 8x8 character block
- **FLI mode**: Can use different colors for each scanline (200 scanlines), allowing all 16 C64 colors to be displayed

## Features

- **Uses Colodore Palette**: The most accurate C64 color representation (Pepto's palette)
- **16 Color Gradient**: Smooth gradient from dark to bright using all 16 C64 colors
- **Side-by-Side Comparison**: Generates both FLI and non-FLI versions to clearly demonstrate the FLI advantage
- **C64 Assembly Data**: Outputs assembly include file with scanline color data

## Generated Files

- `hitmen_fli_logo.png` - FLI version with 16 colors
- `hitmen_non_fli_logo.png` - Non-FLI version with only 4 colors
- `hitmen_comparison.png` - Side-by-side comparison showing the FLI advantage
- `hitmen_fli_data.i` - Assembly data file for C64 demos

## Usage

```bash
python3 generate_hitmen_fli_logo.py
```

## Requirements

- Python 3.6+
- Pillow (PIL) library

Install dependencies:
```bash
pip install Pillow
```

## Technical Details

### Screen Format
- Resolution: 320x200 pixels (40x25 characters)
- Character size: 8x8 pixels
- Mode: FLI Multicolor

### Color Gradient
The 16-color gradient progresses from dark to bright:
1. Black → Brown → Dark Grey → Red → Blue → Purple → Orange → Green
2. Medium Grey → Light Red → Light Blue → Light Grey → Cyan → Light Green → Yellow → White

### FLI Implementation
The FLI effect is achieved by storing different color values for each of the 200 scanlines, allowing the vertical gradient to smoothly transition through all 16 C64 colors.

## Comparison

![HITMEN Logo Comparison](hitmen_comparison.png)

The comparison image clearly shows the FLI advantage:
- **Left**: Non-FLI limited to 4 colors
- **Right**: FLI using all 16 colors in a smooth gradient

This demonstrates why FLI mode is valuable for C64 graphics - it allows much richer color variation!
