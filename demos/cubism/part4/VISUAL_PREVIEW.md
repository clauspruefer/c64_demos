# Alpine Winter 2026 - Visual Preview

This document provides a text-based visualization of how the demo will appear on a C64 screen.

## Screen Layout (40x25 characters)

```
┌────────────────────────────────────────┐ Row 0
│    *     *         *       *           │ Sky with
│         *     *         *     *        │ falling
│  *         *      *           *        │ snowflakes
│      *        *         *        *     │ (sprites)
│  *       *        *   *      *         │
│       *     *        *    *       *    │
│   *      *      *        *         *   │
│      *       *     *        *      *   │ Row 7
│  *      *       *      *        *      │
│      *     *       *       *      *    │
│   *      *    *       *       *        │
│      *       *    *       *        *   │
│  *      *       *    *        *        │
│      *     *       *     *        *    │ Row 14
├────────────────────────────────────────┤ Mountains
│          ▄▄▄▄▄▄                ▄▄▄▄    │ start
│        ▄▄▓▓▓▓▓▓▄▄            ▄▄▓▓▓▓▄▄  │ Row 15-17
│      ▄▄▓▓▓▓▓▓▓▓▓▓▄▄        ▄▄▓▓▓▓▓▓▓▓▄ │ (peaks)
│    ▄▄▓▓▓▓▓▓▓▓▓▓▓▓▓▓▄▄    ▄▄▓▓▓▓▓▓▓▓▓▓▓ │
│  ▄▄▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▄▄▄▄▓▓▓▓▓▓▓▓▓▓▓▓▓ │ Row 18-20
│▄▄▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │ (slopes)
├────────────────────────────────────────┤
│▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒│ Row 21-24
│▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒│ Snow-covered
│▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒│ valley floor
│▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒│ (white)
└────────────────────────────────────────┘ Row 24
```

## Color Scheme

- **Sky (Rows 0-14)**: Dark Blue (#06)
- **Snowflakes**: White (#01) and Light Blue (#0E) alternating
- **Mountain Peaks**: White (#01) for snow caps
- **Mountain Slopes**: Grey (#0B/#0C) for rocky areas
- **Valley Floor**: White (#01) with light shading

## Animation Details

### Snowflakes
- 32 snowflakes total (8 hardware sprites × 4 multiplex positions)
- Each snowflake follows a sine wave for horizontal drift
- Vertical falling speed: 2 pixels per frame
- When reaching bottom, wraps to top for continuous effect
- Different sprites use different patterns (3 snowflake designs)

### Movement Pattern
```
Frame 1:  *         Frame 2:   *        Frame 3:    *
         falling →         falling →         falling
         + sine drift      + sine drift      + sine drift
```

## Technical Breakdown

### Memory Map
```
$0801 - BASIC start
$2000 - Sprite data (3 snowflake patterns × 64 bytes)
$2400 - Main program code
      - Screen setup
      - Sprite initialization
      - IRQ handlers
$3xxx - Sine tables and position data
```

### IRQ Structure
```
IRQ at line 50  → Position sprites (level 1, Y=0-49)
IRQ at line 100 → Reposition sprites (level 2, Y=50-99)
IRQ at line 150 → Reposition sprites (level 3, Y=100-149)
IRQ at line 200 → Reposition sprites (level 4, Y=150-199)
  ↓
Back to line 50
```

## Country Theme

The Alpine landscape represents the Central European winter scene, honoring the strong C64 demo scene in:

- 🇦🇹 **Austria** - Home of famous demoscene events
- 🇩🇪 **Germany** - Large C64 community and gatherings
- 🇨🇭 **Switzerland** - Alpine beauty and winter sports

This celebrates **Winter 2026** in these regions where the C64 scene has flourished for decades.

## Expected Visual Effect

When running, the viewer sees:
1. Peaceful snowfall against a dark winter evening sky
2. Majestic Alpine mountain peaks with snow-covered summits
3. Smooth, natural snowflake motion (sine-based horizontal drift)
4. A serene valley floor covered in fresh snow
5. Classic C64 color palette evoking winter atmosphere

The effect creates a meditative, calming winter scene that would fit perfectly in a winter-themed demo compilation for 2026.
