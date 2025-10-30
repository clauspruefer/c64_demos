# HITMEN FLI Multicolor Logo Generator

This tool generates a "HITMEN" logo for the Commodore 64 demonstrating the **TRUE** FLI (Flexible Line Interpretation) graphics mode effect.

## What is TRUE FLI?

FLI (Flexible Line Interpretation) is a C64 graphics technique that allows changing color registers **on every scanline** (not just every 8 pixels):

- **Non-FLI mode**: Can only change colors every 8 pixels (per character row) - colors are aligned to character boundaries
- **FLI mode**: Can change colors on **EVERY scanline** (200 individual scanlines), creating smooth color bands that **cut through the middle of characters**

This is the **key difference**: FLI creates horizontal color stripes that slice through characters, which is **IMPOSSIBLE** with standard $d800 color RAM.

## Why the Previous Version Was Wrong

The earlier version applied colors per character row (every 8 pixels), which can be done with standard $d800 color RAM - no FLI needed! That was LAME.

The new version creates **scanline-level color changes** that cut through characters at pixel precision - this **requires FLI** and makes the effect clearly visible!

## Features

- **Uses Colodore Palette**: Hardware-measured C64 colors with gamma correction
- **16 Color Gradient**: Smooth gradient using all 16 C64 colors
- **Scanline-Level FLI**: Color changes at every scanline, cutting through characters
- **Side-by-Side Comparison**: Shows blocky non-FLI vs smooth FLI color bands
- **C64 Assembly Data**: Outputs assembly include file with scanline color data

## Generated Files

- `hitmen_fli_logo.png` - FLI version with scanline-level color changes
- `hitmen_non_fli_logo.png` - Non-FLI version with 8-pixel character-row bands
- `hitmen_comparison.png` - Side-by-side comparison showing the TRUE FLI advantage
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

### TRUE FLI Implementation
The FLI effect is achieved by:
1. **200 scanline color entries** - one color value for each of the 200 scanlines
2. **Scanline-level precision** - colors change every pixel line, not every 8 pixels
3. **Bands cut through characters** - horizontal color stripes slice through the middle of characters

### Non-FLI Limitation
Non-FLI mode is limited to:
1. **One color per character row** - all 8 scanlines in a character must use the same color
2. **Blocky bands** - color changes align to 8-pixel character boundaries
3. **Cannot cut through characters** - impossible to have different colors on different scanlines within the same character

## Comparison

![HITMEN Logo Comparison](hitmen_comparison.png)

The comparison image **clearly shows** the FLI advantage:
- **Left**: Non-FLI with blocky 8-pixel bands aligned to character boundaries
- **Right**: FLI with smooth scanline-level bands that cut through characters

The FLI version's horizontal stripes **slice through the letters** at any vertical position, while the non-FLI version's bands are **locked to 8-pixel increments**. This is the true power of FLI!
