# Demo Part "Alpine Winter 2026"

Part #4 is called "Alpine Winter 2026" - a winter-themed demo featuring snowfall over an Alpine landscape.

## Theme

This demo celebrates winter 2026 with a serene Alpine scene, representing the beauty of Central European winters where the C64 demo scene has strong roots (Austria, Germany, Switzerland).

## Layout

The screen is divided into three main sections:

1. **Top Sky** - Dark winter sky with falling snowflakes using sprite multiplexing
2. **Middle Mountains** - Alpine mountain silhouette using character graphics
3. **Bottom Valley** - Snow-covered valley floor with animated snowdrift effect

## Visual Effects

### Snowflakes (Sprites)
- Uses all 8 hardware sprites multiplexed across multiple Y positions
- Each snowflake has individual X and Y sine movement for natural falling motion
- Sprites use multicolor mode for detailed snowflake patterns
- Different fall speeds create depth perception

### Mountain Landscape (Characters)
- Custom character set for mountain silhouettes
- Multicolor character mode for gradients
- Static background representing alpine peaks
- Uses colors: dark blue (sky), white (snow caps), grey (rock faces)

### Color Palette
- Background: Dark blue (#06 - Blue)
- Snowflakes: White (#01 - White) and light blue (#0E - Light Blue)
- Mountains: Grey (#0B - Dark Grey), white (#01), dark blue
- Valley: White with light blue shading

## Technical Implementation

### Sprite Multiplexing
Similar to part1's technique, but with 8 sprites multiplexed across 3-4 positions:
- 8 sprites × 4 positions = 32 snowflakes on screen
- IRQ-based sprite positioning for smooth multiplexing
- Each sprite has independent sine-based X/Y movement

### Sine Movement Data
- Pre-calculated sine tables for smooth snowflake motion
- X-position sine: Creates gentle horizontal drift
- Y-position offset: Different speeds for depth effect
- Combined movement creates realistic falling snow

### Memory Layout
```
$0801       - BASIC start
$2000       - Sprite data (snowflake patterns)
$2400       - Main program code
$2800       - Character set (mountain graphics)
$3000       - Sine table data (X/Y movement)
$3400       - Color data
```

## Winter 2026 Country Theme

The Alpine setting represents:
- **Austria**: Home of famous demo groups and scene gatherings
- **Switzerland**: Alpine beauty and winter sports
- **Germany**: Strong C64 community and events

The demo evokes the peaceful winter atmosphere of these regions during the 2026 winter season.

## Timing and Synchronization

- Stable raster IRQ for flicker-free sprite multiplexing
- Double buffering for smooth animation
- 50Hz PAL timing (standard for European C64)
- Synchronized color cycling for aurora effect in upper sky (optional)

## Assembly Generation

The demo uses Python scripts to generate:
1. Sine wave tables for snowflake movement
2. Sprite data for snowflake patterns
3. Character set data for mountain landscape
4. Optimized assembly code for sprite positioning

### Generated Data Files

The Python scripts generate several data files:
- **sinus-snowflake-x-data.i** - Used: X-position sine table (30px amplitude)
- **sinus-snowflake-x2-data.i** - Reserved: Alternative sine (50px amplitude) for future variety
- **snowflake-y-speed-data.i** - Reserved: Variable fall speeds for future depth effect
- **snowflake-initial-y-data.i** - Used: Staggered initial Y positions
- **snowflake-sprite-data.i** - Used: Three snowflake sprite patterns

Files marked "Reserved" are generated for potential future enhancements.

## Future Enhancements

Potential additions for the final version:
- Aurora borealis effect in upper sky using color cycling
- Music integration (winter-themed SID tune)
- Smooth scrolling text scroller with greeting message
- Particle effect for wind gusts moving snow
- Variable snowflake fall speeds using snowflake-y-speed-data.i
- Wider horizontal movement using sinus-snowflake-x2-data.i
