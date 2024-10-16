!to "parallelogram.prg", cbm
*=$0801
!BASIC

main

; clear screen
  lda #$20
  ldx #0
clrloop
  sta $0400, x
  sta $0500, x
  sta $0600, x
  sta $0700, x
  dex
  bne clrloop

; setup interrupt / timer
  sei

; disable signals from CIA-1 (maskable IRQ)
  lda #%011111111
  sta $dc0d

; clear VIC raster register
  and $d011
  sta $d011

; ack pending irqs CIA-1 and CIA-2
  sta $dc0d
  sta $dd0d

; set interrupt on raster line 48 @top visible screen
  lda #48
  sta $d012

; set interrupt address
  lda #<irq
  ldx #>irq
  sta $0314
  stx $0315

; set global sprite 2x height pixels
  lda #%11111111
  sta $d017

; set global sprite 1x width pixels
  lda #%00000000
  sta $d01d

; set global sprite multicolor
  sta $d01c

; set global sprite colors

  ldx #15
  stx $d027
  ldx #3
  stx $d028
  ldx #14
  stx $d029
  ldx #3
  stx $d02a

; set sprite start y position(s)
  lda #50
  sta $d001
  sta $d003
  sta $d005
  sta $d007

; set x-pos hi bit
  lda #%00000000
  sta $d010

; sprite text front / back pos

  lda #%00000000
  sta $d01b

; enable sprite display 1-4
  lda #%00001111
  sta $d015

; reset all sprite data pointer

  lda #$80
  sta $07f8
  sta $07f9
  sta $07fa
  sta $07fb
  sta $07fc
  sta $07fd
  sta $07fe
  sta $07ff

; reset all sprite x position

  lda #0
  sta $d000
  sta $d002
  sta $d004
  sta $d006
  sta $d008
  sta $d00a
  sta $d00c
  sta $d00e

; enable raster irq mode from VIC
  lda #%00000001
  sta $d01a

  cli

endless
  jmp endless

irq

; raster line 1 - sprites x pos

  lda #50
  sta $d001
  sta $d003
  sta $d005
  sta $d007

; timing

  nop
  nop

top_vector_scroll
  lda #20
  sta $d016

top_character_set
  lda #21
  sta $d018

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

; raster bar color (1 pixel)

  lda #1
  sta $d021
  sta $d020

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

; timing

  nop
  nop
  nop
  nop
  nop

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

; background raster col #2

lbl_barcol1
  lda #0

  sta $d020
  sta $d021

  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

; raster line 2 - sprites x pos

lbl_sprite2_1
  ldx #0
  ldy #0

  stx $d000
  sty $d002

lbl_sprite2_2
  ldx #0
  ldy #0

  stx $d004
  sty $d006

; background raster col #3

lbl_barcol2
  lda #0

  sta $d020
  sta $d021

; timing

  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

; raster line 3 - sprites x pos

lbl_sprite3_1
  ldx #0
  ldy #0

  stx $d000
  sty $d002

lbl_sprite3_2
  ldx #0
  ldy #0

  stx $d004
  sty $d006

; background raster col

lbl_barcol3
  lda #0

  sta $d020
  sta $d021

; timing

  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

; raster line 4 - sprites x pos

lbl_sprite4_1
  ldx #0
  ldy #0

  stx $d004
  sty $d006

lbl_sprite4_2
  ldx #0
  ldy #0

  stx $d000
  sty $d002

; background raster col #2

lbl_barcol4
  lda #0

  sta $d020
  sta $d021

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  nop
  nop
  nop

; include script pre-calculated code

!src "rbars-gen-code-spritestop.asm"

; multiplex y-sprites

  lda #50+(2*21)
  sta $d001
  sta $d003
  sta $d005
  sta $d007

!src "rbars-gen-code-spritesbottom.asm"

; reset background raster col to background col

  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop

  lda #1
  sta $d020
  sta $d021

; move sprites to border (non visible)

  ldx #0

  lda #0
  sta $d020
  sta $d021

  stx $d000
  stx $d002
  stx $d004
  stx $d006

; process sin test

!src "sin-processing.asm"

sprite1_sin
  ldx #0
  lda $3000,x
  sta lbl_sprite39_1+1
  ;inc sprite1_sin+1
  inc sprite1_sin+1

sprite2_sin
  ldx #2
  lda $3000,x
  sta lbl_sprite39_1+3
  ;inc sprite2_sin+1
  inc sprite2_sin+1

sprite3_sin
  ldx #4
  lda $3000,x
  sta lbl_sprite39_2+1
  inc sprite3_sin+1
  inc sprite3_sin+1

sprite4_sin
  ldx #6
  lda $3000,x
  sta lbl_sprite39_2+3
  inc sprite4_sin+1
  inc sprite4_sin+1

; ack interrupt / clear VIC interrupt flag

  asl $d019

; finish interrupt (ea81 == ommit keyboard, cursor handling)
  jmp $ea81

* = $2000
!src "sprite-data.i"

* = $3000
!src "sinus-sprite-pixel-data.i"
