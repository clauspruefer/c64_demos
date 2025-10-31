!to "cubism-part4-alpine-winter.prg", cbm
*=$0801
!BASIC

; ============================================================================
; Alpine Winter 2026 - C64 Demo Part
; ============================================================================
; A winter-themed demo featuring falling snowflakes over an Alpine landscape
; Represents the winter 2026 season in Central European demo scene countries
; (Austria, Germany, Switzerland)
; ============================================================================

jmp main

; ============================================================================
; Constants and Definitions
; ============================================================================

; Screen and sprite constants
SCREEN_RAM      = $0400
COLOR_RAM       = $d800
SPRITE_POINTERS = $07f8

; VIC-II registers
VIC_SPRITE_ENABLE    = $d015
VIC_SPRITE_X         = $d000
VIC_SPRITE_Y         = $d001
VIC_SPRITE_XMSB      = $d010
VIC_SPRITE_COLOR     = $d027
VIC_SPRITE_MULTICOLOR= $d01c
VIC_SPRITE_MC0       = $d025
VIC_SPRITE_MC1       = $d026
VIC_BORDER           = $d020
VIC_BACKGROUND       = $d021
VIC_RASTER           = $d012
VIC_CONTROL1         = $d011

; Sprite data location ($2000 / 64 = $80)
SPRITE_DATA_BLOCK    = $80

; Number of snowflakes (8 sprites x 4 multiplex positions)
NUM_SPRITES          = 8
MULTIPLEX_LEVELS     = 4
TOTAL_SNOWFLAKES     = 32

; Colors for winter theme
COLOR_SKY            = $06  ; Blue
COLOR_SNOW_WHITE     = $01  ; White
COLOR_SNOW_LIGHT     = $0e  ; Light blue
COLOR_MOUNTAIN_DARK  = $0b  ; Dark grey
COLOR_MOUNTAIN_MID   = $0c  ; Medium grey

; ============================================================================
; Main Program Entry
; ============================================================================

*=$2400

main

; Clear screen with winter sky color
  lda #$20         ; Space character
  ldx #0
clrloop
  sta SCREEN_RAM, x
  sta SCREEN_RAM+$100, x
  sta SCREEN_RAM+$200, x
  sta SCREEN_RAM+$2e8, x
  lda #COLOR_SKY
  sta COLOR_RAM, x
  sta COLOR_RAM+$100, x
  sta COLOR_RAM+$200, x
  sta COLOR_RAM+$2e8, x
  lda #$20
  dex
  bne clrloop

; Set screen and border colors (winter theme)
  lda #COLOR_SKY
  sta VIC_BORDER
  sta VIC_BACKGROUND

; Draw Alpine mountain landscape (bottom half of screen)
  jsr draw_mountains

; Initialize sprites for snowflakes
  jsr init_sprites

; Setup interrupt system
  sei

; Disable CIA-1 interrupts
  lda #%01111111
  sta $dc0d

; Clear VIC raster register high bit
  and VIC_CONTROL1
  sta VIC_CONTROL1

; Acknowledge pending interrupts
  sta $dc0d
  sta $dd0d

; Set interrupt on raster line 50 (top of screen)
  lda #50
  sta VIC_RASTER

; Set interrupt vector
  lda #<irq
  sta $0314
  lda #>irq
  sta $0315

; Enable raster interrupts
  lda #%00000001
  sta $d01a

  cli

; Main loop - just wait for interrupts
mainloop
  jmp mainloop

; ============================================================================
; Draw Alpine Mountain Landscape
; ============================================================================
; Draws a simple mountain silhouette in the lower portion of screen

draw_mountains

; Draw mountains using characters in rows 15-24 (bottom of screen)
; Using simple patterns with extended background color mode

  ldx #0
mountain_loop
  ; Top of mountains (row 15-17) - peaks
  lda mountain_pattern_top, x
  sta SCREEN_RAM + (15*40), x
  lda #COLOR_SNOW_WHITE
  sta COLOR_RAM + (15*40), x
  
  ; Middle mountains (row 18-20) - slopes
  lda mountain_pattern_mid, x
  sta SCREEN_RAM + (18*40), x
  lda #COLOR_MOUNTAIN_MID
  sta COLOR_RAM + (18*40), x
  
  ; Valley floor (row 21-24) - snow
  lda #$20  ; Space
  sta SCREEN_RAM + (21*40), x
  sta SCREEN_RAM + (22*40), x
  sta SCREEN_RAM + (23*40), x
  sta SCREEN_RAM + (24*40), x
  lda #COLOR_SNOW_WHITE
  sta COLOR_RAM + (21*40), x
  sta COLOR_RAM + (22*40), x
  sta COLOR_RAM + (23*40), x
  sta COLOR_RAM + (24*40), x
  
  inx
  cpx #40
  bne mountain_loop
  
  rts

; Simple mountain pattern data (creates triangular peaks)
mountain_pattern_top
  !byte $20,$20,$20,$20,$a0,$a0,$a0,$20,$20,$20
  !byte $20,$20,$a0,$a0,$a0,$a0,$20,$20,$20,$20
  !byte $a0,$a0,$a0,$a0,$20,$20,$20,$20,$20,$a0
  !byte $a0,$a0,$a0,$20,$20,$20,$20,$20,$20,$20

mountain_pattern_mid
  !byte $a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0
  !byte $a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0
  !byte $a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0
  !byte $a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0,$a0

; ============================================================================
; Initialize Sprites (Snowflakes)
; ============================================================================

init_sprites

; Enable all 8 sprites
  lda #%11111111
  sta VIC_SPRITE_ENABLE

; Set sprite colors (white and light blue)
  lda #COLOR_SNOW_WHITE
  sta VIC_SPRITE_COLOR+0
  sta VIC_SPRITE_COLOR+2
  sta VIC_SPRITE_COLOR+4
  sta VIC_SPRITE_COLOR+6
  lda #COLOR_SNOW_LIGHT
  sta VIC_SPRITE_COLOR+1
  sta VIC_SPRITE_COLOR+3
  sta VIC_SPRITE_COLOR+5
  sta VIC_SPRITE_COLOR+7

; Set sprite data pointers (all point to first snowflake pattern)
  lda #SPRITE_DATA_BLOCK
  sta SPRITE_POINTERS+0
  sta SPRITE_POINTERS+1
  sta SPRITE_POINTERS+2
  sta SPRITE_POINTERS+3
  sta SPRITE_POINTERS+4
  sta SPRITE_POINTERS+5
  sta SPRITE_POINTERS+6
  sta SPRITE_POINTERS+7

; Initialize sprite positions from sine tables
  ldx #0
init_sprite_pos
  lda snowflake_init_y, x
  sta sprite_y_pos, x
  lda #0
  sta sprite_x_phase, x
  inx
  cpx #NUM_SPRITES
  bne init_sprite_pos

  rts

; ============================================================================
; IRQ Handler - Animate Snowflakes
; ============================================================================

irq
  ; Acknowledge interrupt
  lda VIC_RASTER
  sta $d019

  ; Update snowflake positions
  jsr update_snowflakes
  
  ; Position sprites for first multiplex level
  jsr position_sprites_level1

  ; Jump to next IRQ for sprite multiplexing
  lda #100
  sta VIC_RASTER
  lda #<irq_multiplex2
  sta $0314
  lda #>irq_multiplex2
  sta $0315

  jmp $ea81  ; Return via kernal

; ============================================================================
; Update Snowflake Positions
; ============================================================================

update_snowflakes

  ldx #0
update_loop

  ; Update Y position (falling)
  lda sprite_y_pos, x
  clc
  adc #2  ; Fall speed
  cmp #250  ; Bottom of screen?
  bcc no_wrap_y
  lda #0  ; Wrap to top
no_wrap_y
  sta sprite_y_pos, x

  ; Update X position using sine wave
  lda sprite_x_phase, x
  tay
  lda snowflake_x_sine, y
  sta sprite_x_pos, x
  
  ; Advance X phase
  lda sprite_x_phase, x
  clc
  adc #1
  sta sprite_x_phase, x

  inx
  cpx #NUM_SPRITES
  bne update_loop

  rts

; ============================================================================
; Position Sprites (Level 1 - Top sprites)
; ============================================================================
; VIC sprite registers are at $d000-$d00f (X0,Y0,X1,Y1...X7,Y7)
; Using indexed addressing with base VIC_SPRITE_X ($d000)

position_sprites_level1

  ; Position first 8 sprites (level 1)
  ldy #0  ; Sprite number (0-7)
  ldx #0  ; VIC register offset (0-15)
pos_loop1
  ; Set X position at $d000+offset
  lda sprite_x_pos, y
  sta $d000, x  ; VIC_SPRITE_X + offset
  inx
  
  ; Set Y position at $d001+offset (next register)
  lda sprite_y_pos, y
  sta $d000, x  ; VIC_SPRITE_X + offset (reaches Y register)
  inx
  
  iny
  cpy #NUM_SPRITES
  bne pos_loop1

  rts

; ============================================================================
; Multiplex IRQ Handlers
; ============================================================================

irq_multiplex2
  lda VIC_RASTER
  sta $d019
  
  ; Reposition sprites for level 2 (Y positions only)
  ; Only need to update Y coords, X coords stay the same
  ldy #0  ; Sprite number (0-7)
  ldx #1  ; Start at first Y register ($d001)
pos_loop2
  lda sprite_y_pos, y
  clc
  adc #50  ; Offset for multiplex level 2
  sta $d000, x  ; Write to $d001, $d003, $d005, etc. (Y registers)
  inx
  inx  ; Skip to next Y register (skip over X register)
  iny
  cpy #NUM_SPRITES
  bne pos_loop2
  
  ; Setup next IRQ
  lda #150
  sta VIC_RASTER
  lda #<irq_multiplex3
  sta $0314
  lda #>irq_multiplex3
  sta $0315
  
  jmp $ea81

irq_multiplex3
  lda VIC_RASTER
  sta $d019
  
  ; Reposition sprites for level 3 (Y positions only)
  ldy #0  ; Sprite number (0-7)
  ldx #1  ; Start at first Y register ($d001)
pos_loop3
  lda sprite_y_pos, y
  clc
  adc #100  ; Offset for multiplex level 3
  sta $d000, x  ; Write to $d001, $d003, $d005, etc. (Y registers)
  inx
  inx  ; Skip to next Y register
  iny
  cpy #NUM_SPRITES
  bne pos_loop3
  
  ; Setup next IRQ
  lda #200
  sta VIC_RASTER
  lda #<irq_multiplex4
  sta $0314
  lda #>irq_multiplex4
  sta $0315
  
  jmp $ea81

irq_multiplex4
  lda VIC_RASTER
  sta $d019
  
  ; Reposition sprites for level 4 (Y positions only)
  ldy #0  ; Sprite number (0-7)
  ldx #1  ; Start at first Y register ($d001)
pos_loop4
  lda sprite_y_pos, y
  clc
  adc #150  ; Offset for multiplex level 4
  sta $d000, x  ; Write to $d001, $d003, $d005, etc. (Y registers)
  inx
  inx  ; Skip to next Y register
  iny
  cpy #NUM_SPRITES
  bne pos_loop4
  
  ; Return to main IRQ
  lda #50
  sta VIC_RASTER
  lda #<irq
  sta $0314
  lda #>irq
  sta $0315
  
  jmp $ea81

; ============================================================================
; Data Storage
; ============================================================================

; Sprite position tracking
sprite_x_pos      !fill NUM_SPRITES, 0
sprite_y_pos      !fill NUM_SPRITES, 0
sprite_x_phase    !fill NUM_SPRITES, 0

; ============================================================================
; Include Generated Data Files
; ============================================================================

; Snowflake movement sine tables
!src "sinus-snowflake-x-data.i"
!src "snowflake-initial-y-data.i"

; ============================================================================
; Sprite Data
; ============================================================================
; Snowflake sprite patterns at $2000 (block $80)

*=$2000
!src "snowflake-sprite-data.i"
