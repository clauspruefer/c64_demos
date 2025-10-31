# Technical Implementation Notes

## Memory Map

### Zero Page Variables
- `$FB` - frame_counter: Counts frames for animation timing
- `$FC` - animation_state: Current state (0-3)
- `$FD` - alien_x: Alien X position
- `$FE` - man_x: Man X position  
- `$50` - hamburger_x: Hamburger X position
- `$51` - hamburger_y: Hamburger Y position
- `$52` - fart_frame: Fart effect frame counter

### Sprite Data Locations
- `$1000-$103F` - Sprite 0: Alien (63 bytes)
- `$1040-$107F` - Sprite 1: Flying Saucer (63 bytes)
- `$1080-$10BF` - Sprite 2: Donut (63 bytes)
- `$10C0-$10FF` - Sprite 3: Man (63 bytes)
- `$1100-$113F` - Sprite 4: Hamburger (63 bytes)

### VIC-II Registers Used
- `$D000-$D00F` - Sprite X/Y positions
- `$D010` - Sprite X MSB (most significant bit)
- `$D012` - Raster line register
- `$D015` - Sprite enable register
- `$D019` - Interrupt status register
- `$D01A` - Interrupt enable register
- `$D01B` - Sprite priority register
- `$D020` - Border color
- `$D021` - Background color
- `$D027-$D02E` - Sprite colors

## Animation States

### State 0: Alien Eating (60 frames)
- Alien and saucer stationary in sky
- Donut bounces between positions to simulate eating
- Frame counter determines donut position

### State 1: Man Walking (120 frames)
- Man walks from left to right across screen
- Hamburger follows man's position
- Each frame increments man_x by 1 pixel

### State 2: Alien Attacks (30 frames)
- Alien and saucer move down and right toward man
- At frame 20, fart effect triggers
- Border flashes green for 5 frames to show fart cloud
- Donut moves with alien

### State 3: Hamburger Flies (60 frames)
- Hamburger trajectory follows parabolic arc
- First 30 frames: moves right and up
- Last 30 frames: continues right but falls down
- Simulates approximately 20 meters of travel

## Sprite Design

All sprites are 24x21 pixels, stored as 63 bytes:
- Each row is 3 bytes (24 bits)
- Total of 21 rows
- Monochrome sprites (single color + transparent)

### Sprite Colors
- Alien: Red ($0A)
- Saucer: Gray ($0C)
- Donut: Yellow ($07)
- Man: Light Gray ($0F)
- Hamburger: Orange ($08)

## Timing

The animation runs at 50Hz (PAL) or 60Hz (NTSC) depending on C64 system:
- Total loop time: 270 frames (5.4s at 50Hz, 4.5s at 60Hz)
- State 0: 60 frames (1.2s / 1.0s)
- State 1: 120 frames (2.4s / 2.0s)
- State 2: 30 frames (0.6s / 0.5s)
- State 3: 60 frames (1.2s / 1.0s)

## IRQ Handling

The demo uses a raster interrupt at line 250 (bottom of screen):
- Provides stable timing independent of main loop
- Updates sprite positions and animation state
- Plays position and timing calculations
- Properly acknowledges VIC-II interrupt
