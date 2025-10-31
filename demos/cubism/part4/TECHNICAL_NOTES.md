# Technical Notes - Alpine Winter 2026

## Demo Scene Context

This demo part is created for the **Winter 2026** demoscene season, celebrating the rich history and ongoing activity of the C64 demoscene in Central Europe.

## Why Alpine/Country Theme?

### Historical Significance
The Central European countries (Austria, Germany, Switzerland) have been at the heart of the C64 demoscene since the 1980s:

- **Austria**: Host of numerous demoscene parties and home to legendary groups
- **Germany**: Largest European C64 community, numerous active coders
- **Switzerland**: Alpine setting, demo parties, and active scene members

### Winter 2026 Timing
Creating a winter-themed demo for the 2026 season allows it to be shown at:
- Winter demo parties
- Online demo compilations released in Q1 2026
- New Year celebrations in the demoscene
- Retrospective compilations

## Technical Innovations

While staying true to classic C64 demo techniques, this part includes:

### Sprite Multiplexing Refinement
- 4-level multiplexing for 32 on-screen snowflakes
- IRQ-based repositioning with stable raster timing
- Minimal flicker through careful Y-position management

### Sine-Based Natural Movement
- Pre-calculated sine tables for smooth motion
- Horizontal drift creates realistic wind effect
- Constant falling speed with sine-modulated X position

### Memory Efficiency
- Shared sprite patterns (3 designs for variety)
- Data generation via Python (not hand-coded)
- Optimized IRQ handlers with minimal register usage

## Assembly Workflow

### Development Process
```
1. Design concept (winter theme, country focus)
   ↓
2. Create Python generators for data
   ↓
3. Generate sine tables and sprite patterns
   ↓
4. Write main assembly with ACME syntax
   ↓
5. Include generated data files
   ↓
6. Compile with ACME assembler
   ↓
7. Test in VICE emulator or real C64
```

### Data Generation Philosophy
Following the pattern from part1 and part2:
- Python scripts generate complex data
- Assembly focuses on runtime logic
- Separation allows rapid iteration

## Future Enhancements

If time and cartridge space permit:

### Sound Integration
- Winter-themed SID music (composed specially)
- Sound effects for wind or tinkling bells
- Music timing synchronized to visual beats

### Additional Visual Effects
- Aurora borealis (color cycling in upper sky)
- Sparkle effect on snow (cycling bright pixels)
- Smooth horizontal scrolling text scroller
- "Alpine Winter 2026" logo/greeting

### Optimization
- Use of DMA timing for tighter sprite positioning
- Double-buffered animation for ultra-smooth motion
- Compressed sprite data if space is tight

## Compatibility

### Hardware Support
- **PAL C64**: Primary target (50Hz timing)
- **NTSC C64**: May need timing adjustments
- **SID**: No music yet, so any SID revision works
- **Memory**: Fits in standard 64KB RAM

### Emulator Support
- **VICE**: Full compatibility expected
- **CCS64**: Should work with proper timing
- **Real Hardware**: Target platform

## Building

### Requirements
- ACME Cross-Assembler (https://sourceforge.net/projects/acme-crossass/)
- Python 3.x (for data generation)
- VICE Emulator (for testing)

### Build Steps
```bash
cd demos/cubism/part4/python
python3 gen-snowflake-sine.py
python3 gen-snowflake-sprites.py
cd ..
acme -f cbm -o cubism-part4-alpine-winter.prg main.asm
```

### Testing
```bash
x64 cubism-part4-alpine-winter.prg
```

## Credits

- **Concept**: Winter 2026 country-themed demo part
- **Code**: Assembly with ACME syntax
- **Data Generation**: Python 3
- **Target**: C64 demoscene (Austria, Germany, Switzerland)
- **Year**: 2026 (Winter season)

## License

Follows the main repository license (see root LICENSE file).

## Contact

For demo scene context and C64 programming discussions:
- CSDb (C64 Scene Database): https://csdb.dk/
- Lemon64 Forums: https://www.lemon64.com/forum/
- Pouët: https://www.pouet.net/

---

**Note**: This demo part celebrates the timeless appeal of C64 demo programming and the enduring community in Central Europe. Winter 2026 represents both a specific time period and the ongoing passion for this classic platform.
