# Cubism Part 4: Halloween Special 🎃👻

A spooky Halloween-themed demo part for the Commodore 64!

## Description

This demo part celebrates Halloween with classic spooky effects:
- **Floating Jack-O'-Lantern Pumpkins**: Four animated pumpkin sprites moving in sine wave patterns
- **Halloween Color Scheme**: Orange, black, purple, and green raster bars
- **Spooky Messages**: Halloween greetings displayed on screen
- **Eerie Animations**: Smooth sine wave movements creating a floating, haunting effect

## Technical Details

### Screen Layout

The demo creates a Halloween atmosphere using:
1. Black border and background (spooky darkness!)
2. Orange text color for the Halloween messages
3. Animated color raster bars cycling through Halloween colors
4. Four pumpkin sprites floating across the screen

### Sprites

- **Sprite Type**: Custom jack-o'-lantern pumpkin design (24x21 pixels)
- **Sprite Colors**: Orange (#$08) for authentic pumpkin appearance
- **Sprite Count**: 4 pumpkins displayed simultaneously
- **Animation**: Horizontal and vertical sine wave movement for floating effect
- **Size**: 2x width for larger, more visible pumpkins

### Color Scheme

Classic Halloween colors from the C64 palette:
- **Orange** (#$08): Main pumpkin color and primary text
- **Black** (#$00): Background and darkness
- **Purple** (#$04): Ghostly, mysterious atmosphere
- **Green** (#$05): Spooky highlights
- **Red** (#$02): Additional Halloween accent
- **Brown** (#$09): Earth tones for variety

### Raster Effects

The demo uses raster bar effects to create animated color bands:
- Starts at raster line 50
- Animates through to raster line 200
- Color data cycles through the Halloween palette
- Creates a dynamic, eerie background atmosphere

### Sine Wave Movement

Two types of sine wave data control the pumpkin animation:
1. **Horizontal Movement** (`pumpkin-sine-x.i`):
   - Amplitude: 80 pixels
   - Frequency: 1 cycle per 256 frames
   - Offset: 120 pixels (centered)
   
2. **Vertical Wobble** (`pumpkin-sine-y.i`):
   - Amplitude: 20 pixels (subtle)
   - Frequency: 2 cycles per 256 frames
   - Offset: 100 pixels
   - Creates a floating, ethereal effect

Each pumpkin sprite reads from a different offset in the sine table, creating varied but synchronized movement patterns.

## Files

- `main.asm` - Main demo assembly code
- `pumpkin-sprite.i` - Jack-o'-lantern sprite data (63 bytes)
- `pumpkin-sine-x.i` - Horizontal sine wave movement data (256 bytes)
- `pumpkin-sine-y.i` - Vertical sine wave movement data (256 bytes)
- `halloween-colors.i` - Color raster bar data (256 bytes)
- `python/gen-halloween-sin.py` - Python script to generate sine wave data

## Building

To assemble this demo, use the ACME cross-assembler:

```bash
acme main.asm
```

This will create `cubism-part4-halloween.prg` which can be loaded into a C64 emulator like VICE.

## Running

### In VICE Emulator:
```bash
x64 cubism-part4-halloween.prg
```

### On Real C64 Hardware:
Load the `.prg` file from disk and run:
```
LOAD"CUBISM-PART4-HALLOWEEN.PRG",8,1
RUN
```

## Memory Map

```
$0801-$0804  : BASIC starter
$0bfe        : (Reserved for future music/SID)
$2000-$203f  : Pumpkin sprite data (63 bytes)
$2100-$21ff  : Horizontal sine wave data (256 bytes)
$2200-$22ff  : Vertical sine wave data (256 bytes)
$2300-$23ff  : Halloween color raster data (256 bytes)
$2400-$????  : Main program code
```

## Visual Effect Breakdown

### Sprite Animation
Each of the 4 pumpkins has:
- Independent X position from offset sine table
- Shared or independent Y wobble
- Continuous cycling through sine wave
- Orange coloring (#$08)
- 2x width for better visibility

### Raster Bars
- Dynamic color changes on each raster line
- Halloween-themed color palette cycling
- Smooth transitions between colors
- Covers most of visible screen (lines 50-200)

### Messages
- "HAPPY HALLOWEEN 2025!" - Main greeting
- "SPOOKY DEMO BY TWF" - Credits
- Displayed in orange text on black background

## Credits

- **Code**: TWF (The Worst Felon)
- **Demo Series**: Cubism (Hitmen)
- **Theme**: Halloween 2025 Special Edition
- **Platform**: Commodore 64

## Notes

This is a standalone demo part that can be integrated into the larger Cubism demo compilation. The Halloween theme makes it perfect for seasonal events and demo parties held around October 31st.

The demo uses stable raster interrupts and careful timing to achieve smooth animations without flicker. All effects run in real-time on the C64's 1MHz 6510 CPU.

## Future Enhancements

Potential additions for future versions:
- SID music (spooky Halloween tune)
- Additional sprite types (ghosts, bats, witches)
- More complex scrolling text
- FLI graphics mode for hi-res pumpkin background
- Character set animation for flickering effects

---

Happy Halloween! 🎃👻🦇
