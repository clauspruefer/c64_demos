!to "demo1.prg", cbm
*=$0801
!BASIC

main

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

; set interrupt on raster line 58 @top visible screen
  lda #58
  sta $d012

; set interrupt address
  lda #<irq
  ldx #>irq
  sta $0314
  stx $0315

; enable raster irq mode from VIC
  lda #%00000001
  sta $d01a

  cli

endless
  jmp endless

irq

; "wait" for left beam position
 
  nop
  nop
  nop
  nop
  nop
  nop
  
; set border and main raster color line white

  lda #$1
  sta $d021
  sta $d020

; "wait" for next line beam position

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

; set border and main raster color line black

  lda #$0
  sta $d020
  sta $d021

; ack interrupt / clear VIC interrupt flag
  
  asl $d019

; finish interrupt (ea81 == ommit keyboard, cursor handling)
  jmp $ea81
