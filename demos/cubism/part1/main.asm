!to "cubism-part2.prg", cbm
*=$0801
!BASIC

; copy logo chars / banks
  ldx #0
clrloop1
  lda $5400+3, x
  sta $4800, x
  lda $5400+3+40, x
  sta $4800+40, x
  lda $5400+3+80, x
  sta $4800+80, x
  lda $5400+3+120, x
  sta $4800+120, x
  lda $5400+3+160, x
  sta $4800+160, x
  lda $5400+3+200, x
  sta $4800+200, x
  lda $5400+3+240, x
  sta $4800+240, x
  lda $5400+3+280, x
  sta $4800+280, x
  lda $5400+3+320, x
  sta $4800+320, x
  inx
  cpx #37
  bne clrloop1

  ldx #0
clrloop2
  lda $5400+2, x
  sta $4c00, x
  lda $5400+2+40, x
  sta $4c00+40, x
  lda $5400+2+80, x
  sta $4c00+80, x
  lda $5400+2+120, x
  sta $4c00+120, x
  lda $5400+2+160, x
  sta $4c00+160, x
  lda $5400+2+200, x
  sta $4c00+200, x
  lda $5400+2+240, x
  sta $4c00+240, x
  lda $5400+2+280, x
  sta $4c00+280, x
  lda $5400+2+320, x
  sta $4c00+320, x
  inx
  cpx #37
  bne clrloop2

  ldx #0
clrloop3
  lda $5400+1, x
  sta $5000, x
  lda $5400+1+40, x
  sta $5000+40, x
  lda $5400+1+80, x
  sta $5000+80, x
  lda $5400+1+120, x
  sta $5000+120, x
  lda $5400+1+160, x
  sta $5000+160, x
  lda $5400+1+200, x
  sta $5000+200, x
  lda $5400+1+240, x
  sta $5000+240, x
  lda $5400+1+280, x
  sta $5000+280, x
  lda $5400+1+320, x
  sta $5000+320, x
  inx
  cpx #37
  bne clrloop3

; set character color gray
  lda #$0c
  ldx #0
clrloop4
  sta $d800, x
  sta $d900, x
  dex
  bne clrloop4
  
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

; set interrupt on raster line 50 @top visible screen
  lda #50
  sta $d012

; set interrupt address
  lda #<irq
  ldx #>irq
  sta $0314
  stx $0315

; enable raster irq mode from VIC
  lda #%00000001
  sta $d01a

; init sound
  lda #1
  jsr $0d2c

  cli

endless
  jmp endless

* = $2000
irq

; "set" vic bank to $4000

  lda #%00000010
  sta $dd00

  lda #$0c
  sta $d020
  sta $d021
  
  nop
  nop
  nop
  nop

!src "rasterline-opcodes-out.asm"
!src "copy-data.asm"

d016_sin
  ldx #0
  lda $3e00,x
  sta scroll_offset1+1
  inc d016_sin+1

d018_sin
  ldx #0
  lda $3f00,x
  ;sta charset_map_offset1+1
  inc d018_sin+1

  ;lda #64
  ;sta charset_map_offset1+1

; ack interrupt / clear VIC interrupt flag
  
  asl $d019

; play sound

  jsr $0d83
 
; finish interrupt (ea81 == ommit keyboard, cursor handling)
  jmp $ea81

* = $0c6a
!bin "Iron_Lord.sid"

* = $3e00
!src "sinus-d016-data.i"

* = $3f00
;!src "sinus-d018-data.i"

* = $4000
!bin "hitmenlogo-charset.bin"

* = $5400
!bin "hitmenlogo-charmap.bin"
