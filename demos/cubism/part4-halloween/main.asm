; ============================================================================
; C64 Demo - Cubism Part 4: Halloween Special
; ============================================================================
; A spooky Halloween-themed demo featuring:
; - Floating jack-o'-lantern pumpkin sprites
; - Halloween color raster bars (orange, black, purple, green)
; - Spooky scrolling message
; - Eerie sine wave effects
; ============================================================================

!to "cubism-part4-halloween.prg", cbm
*=$0801
!BASIC

jmp main

*=$2400

main

; ============================================================================
; Initialize Screen
; ============================================================================

; clear screen with spaces
  lda #$20
  ldx #0
clrloop
  sta $0400, x
  sta $0500, x
  sta $0600, x
  sta $0700, x
  dex
  bne clrloop

; set border and background to black (spooky!)
  lda #$00
  sta $d020
  sta $d021

; set character colors to orange (Halloween!)
  lda #$08
  ldx #0
colorloop
  sta $d800, x
  sta $d900, x
  sta $da00, x
  sta $db00, x
  dex
  bne colorloop

; ============================================================================
; Display Halloween Message
; ============================================================================

; "HAPPY HALLOWEEN 2025" centered on screen
  ldx #0
msgloop
  lda halloween_msg, x
  beq msg_done
  sta $0400+10*40+8, x  ; Row 10, centered
  inx
  cpx #20
  bne msgloop

msg_done

; "SPOOKY DEMO BY TWF" 
  ldx #0
msg2loop
  lda halloween_msg2, x
  beq msg2_done
  sta $0400+12*40+10, x  ; Row 12
  inx
  cpx #18
  bne msg2loop

msg2_done

; ============================================================================
; Setup Interrupts and Sprites
; ============================================================================

  sei

; disable signals from CIA-1 (maskable IRQ)
  lda #%01111111
  sta $dc0d

; clear VIC raster register
  and $d011
  sta $d011

; ack pending irqs CIA-1 and CIA-2
  sta $dc0d
  sta $dd0d

; set interrupt on raster line 50
  lda #50
  sta $d012

; set interrupt address
  lda #<irq
  ldx #>irq
  sta $0314
  stx $0315

; ============================================================================
; Configure Sprites (Pumpkins!)
; ============================================================================

; set global sprite 1x height pixels
  lda #%00000000
  sta $d017

; set global sprite 2x width pixels for bigger pumpkins
  lda #%00001111
  sta $d01d

; set global sprite multicolor mode off
  lda #%00000000
  sta $d01c

; set sprite colors to orange (pumpkin body)
  lda #$08  ; orange
  sta $d027
  sta $d028
  sta $d029
  sta $d02a

; set sprite start y position(s)
  lda #100
  sta $d001
  sta $d003
  lda #140
  sta $d005
  sta $d007

; set sprite start x positions
  lda #50
  sta $d000
  lda #120
  sta $d002
  lda #190
  sta $d004
  lda #240
  sta $d006

; set x-pos hi bit (all sprites in low 256 range initially)
  lda #%00000000
  sta $d010

; sprite text front / back pos (in front)
  lda #%00000000
  sta $d01b

; enable sprite display 1-4 (four pumpkins)
  lda #%00001111
  sta $d015

; set all sprite data pointers to pumpkin sprite at $2000
  lda #$80
  sta $07f8
  sta $07f9
  sta $07fa
  sta $07fb

; enable raster irq mode
  lda #%00000001
  sta $d01a

; clear interrupts and start
  cli

; ============================================================================
; Main Loop
; ============================================================================

endless
  jmp endless

; ============================================================================
; IRQ Interrupt Handler
; ============================================================================

!align 255,0

irq

; ============================================================================
; Raster Bar Effects with Halloween Colors
; ============================================================================

; start at raster line 50
  lda #50
  sta current_raster

rasterbar_loop
  lda current_raster
  cmp #200  ; stop at raster line 200
  bcs rasterbar_done

  ; wait for raster line (simple busy-wait, safe for demo use)
  ; current_raster value is already in accumulator from above
raster_wait
  cmp $d012
  bne raster_wait

  ; get color from table
  ldx raster_index
  lda halloween_color_table, x
  sta $d020
  sta $d021

  ; increment counters
  inc raster_index

  inc current_raster
  inc current_raster  ; skip every other line for speed
  jmp rasterbar_loop

rasterbar_done

; reset background to black
  lda #$00
  sta $d020
  sta $d021

; ============================================================================
; Animate Pumpkin Sprites with Sine Movement
; Using self-modifying code (classic C64 demo technique) to update sprite
; positions by incrementing the index into the sine wave lookup table
; ============================================================================

; pumpkin 1 - horizontal sine wave
sprite1_update
  ldx #0
  lda pumpkin_sine_x, x
  sta $d000
  inc sprite1_update+1

; pumpkin 2 - horizontal sine wave (offset)
sprite2_update
  ldx #64
  lda pumpkin_sine_x, x
  sta $d002
  inc sprite2_update+1

; pumpkin 3 - horizontal sine wave (different offset)
sprite3_update
  ldx #128
  lda pumpkin_sine_x, x
  sta $d004
  inc sprite3_update+1

; pumpkin 4 - horizontal sine wave (another offset)
sprite4_update
  ldx #192
  lda pumpkin_sine_x, x
  sta $d006
  inc sprite4_update+1

; vertical wobble for pumpkin 1 and 3
sprite_y1_update
  ldx #0
  lda pumpkin_sine_y, x
  sta $d001
  inc sprite_y1_update+1

sprite_y3_update
  ldx #128
  lda pumpkin_sine_y, x
  sta $d005
  inc sprite_y3_update+1

; ============================================================================
; Color Cycling Effect
; ============================================================================

; cycle the color table offset for animated raster bars
  inc color_offset
  lda color_offset
  sta raster_index

; ack interrupt / clear VIC interrupt flag
  asl $d019

; finish interrupt (ea81 = skip keyboard, cursor handling)
  jmp $ea81

; ============================================================================
; Data Section
; ============================================================================

halloween_msg
  !scr "happy halloween 2025!"
  !byte 0

halloween_msg2
  !scr "spooky demo by twf"
  !byte 0

; Variables
current_raster
  !byte 0

raster_index
  !byte 0

color_offset
  !byte 0

; ============================================================================
; Sprite and Color Data
; ============================================================================

* = $2000
!src "pumpkin-sprite.i"

* = $2100
pumpkin_sine_x
!src "pumpkin-sine-x.i"

* = $2200
pumpkin_sine_y
!src "pumpkin-sine-y.i"

* = $2300
halloween_color_table
!src "halloween-colors.i"
