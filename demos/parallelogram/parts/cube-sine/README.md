# Demo Part "Cube Sine"

Part #1 is called "Cube with Sprite-Pixel Sine Overlay".

# Layout

The screen will be divided into 3 parts.

1. Top "VectorObject, SpritesOverlay Scroller"
2. 2x2 character "Standard-Scroller"
3. Arcanoid Game "Dummy"

# Sprites

- Sprite pixel size Normal Size X ($d017)
- Sprite pixel size Double Size Y ($d01d)

We build two sprite rows containing 8 sprites in sum.

```bash
      x:1 x:2 x:3 x:4
y:1   s1  s2  s3  s4
------ Multipplex ----
y:2   s1  s2  s3  s4
```

## Mutiplexing

After Raster Line at Sprite1 end (21 pixel) is reached, the sprites will be multiplexed.

```assembly
; multiplex y-sprites

  lda #50+(2*21)
  sta $d001
  sta $d003
  sta $d005
  sta $d007
```

## Data Pointer

All sprites data pointer is set to $2000 where a single sprite resides:

```bash
Row1  "*           "
Row2  "*           "
Row3  "*           "
Row4  "*           "
Row5  "*           "
Row6  "*           "
Row7  "*           "
Row8  "*           "
Row9  "*           "
Row10 "*           "
Row11 "*           "
Row12 "*           "
Row13 "*           "
Row14 "*           "
Row15 "*           "
Row16 "*           "
Row17 "*           "
Row18 "*           "
Row19 "*           "
Row20 "*           "
Row21 "*           "
```

# Rastersplit

So, to achieve having 4 individual "moving" pixel per raster line, we need to
"switch" 4 sprites X-Position ($d000, $d002, $d004 and $d006).

"Impossible!" someone could say regarding the "Bad-Raster-Line" problem sucking
up clock cycles.

Ha, just do it every 2 raster-lines! And do it exactly in the position between
the sprite hi-bit and next raster line starting area. So we are also able to 
also set $d020 **and** $d021 dynamic background colors.

Noone will realize if we will only use it as transient switch between cycling
vector-objects :)

The next graph exactly shows what we have achieved so far.

```bash
#Line1   ||||     ||||         ||||      ||||   \ == 2 Height Pixel
#Line2   ||||     ||||         ||||      ||||   / independ. movable
-------------------------------------------------------------------
#Line3    ||||      ||||     ||||      ||||     \ == 2 Height Pixel
#Line4    ||||      ||||     ||||      ||||     / independ. movable
-------------------------------------------------------------------
#Line5     ||||     ||||     ||||      ||||     \ == 2 Height Pixel
#Line6     ||||     ||||     ||||      ||||     / independ. movable
-------------------------------------------------------------------
#Line7    ||||       ||||   ||||     ||||       \ == 2 Height Pixel
#Line8    ||||       ||||   ||||     ||||       / independ. movable
```

> Each 2 Lines 4 Pixels that can be positioned (moved) individually.
> Predestined for moving Sine-Curves in front of multicolor text mode.

# Setting up Stable Raster IRQ

To set up a "stable" raster irq (non-wobbling 100% fixed y-position) multiple
solution approaches exist.

The most simple one is setting it up by correct *sprite-positioning*.

Simple to say: enable / disable the correct sprites regarding default
sprite positions before setting up interrupt handling (cli).

If sprites reside (even non-enabled) at the **wrong** position the VIC
ends up with unstable timing problems. Also you should only enable 2,4
or 8 sprites not 1,3,5,6 or 7.

# Timing Calculation

The timing cal culation can be very time consuming. One nop or opcode at
the wrong position will also make the VIC unstable again.

For this i have set up a 8 lines Assembler "Template". This template will
be used to generate assembler code from.

Currently it is somehow a bit hand-corrected, maybe we will fix this in the
future.

# Dynamic Data Optimization

To minimize cpu cycles for reading / writing $d000 ... data we will 
directly set each 2 raster lines. ACME labels will help changing this data
from "outside".

```assembly
; sprite x position data

lbl_sprite1_1
  ldx #0
  ldy #0

  stx $d000
  sty $d002

lbl_sprite1_2
  ldx #0
  ldy #0

  stx $d004
  sty $d006
```

Changing the values (data at the last line) from "oustside" with position data
(256 bytes) at $2100 now is possible by:

```assembly
sprite1_sin
  ldx #0
  lda $2100,x
  sta lbl_sprite39_1+1
  ;inc sprite1_sin+1
  inc sprite1_sin+1
```
