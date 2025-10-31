# Cubism - C64 Demo

A multi-part demo for the Commodore 64, featuring various visual effects and techniques.

## Parts

### Part 1: Hitmen Logo with Sprite Sine Overlay
Features raster line effects, sprite multiplexing, and sine wave overlay animations.
See [part1/README.md](part1/README.md) for details.

### Part 2: Cube Sine
Rotating vector objects with sprite-pixel sine overlays and raster bar effects.
See [part2/README.md](part2/README.md) for details.

### Part 3: Evaluation Effects
Various experimental effects and prototypes.

### Part 4: Halloween Special 🎃
A spooky Halloween-themed demo featuring:
- Floating jack-o'-lantern pumpkin sprites
- Halloween color raster bars (orange, black, purple, green)
- Sine wave animations for eerie floating effects
- Seasonal greetings and spooky atmosphere

See [part4-halloween/README.md](part4-halloween/README.md) for details.

## Building

Each part can be assembled independently using ACME:

```bash
cd part1
acme main.asm

cd ../part2
acme main.asm

cd ../part4-halloween
acme main.asm
```

## Running

Load the generated `.prg` files in the VICE emulator or on real C64 hardware.

## Credits

Coded by TWF (The Worst Felon) for Hitmen
