; sprites x position #1 -  template start

$lbl_barcol1
  lda #3

$lbl_sprite1_1
  ldx #100
  ldy #120

  stx $$d000
  sty $$d002

$lbl_sprite1_2
  ldx #140
  ldy #160

  stx $$d004
  sty $$d006

  sta $$d021
  sta $$d020

; raster col line #1

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

; sprites x position #2

$lbl_sprite2_1
  ldx #50
  ldy #60

  stx $$d000
  sty $$d002

$lbl_sprite2_2
  ldx #70
  ldy #80

  stx $$d004
  sty $$d006

; raster col line #2

$lbl_barcol2
  lda #0
  sta $$d020
  sta $$d021

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

; sprites x position #3

$lbl_sprite3_1
  ldx #100
  ldy #120

  stx $$d000
  sty $$d002

$lbl_sprite3_2
  ldx #140
  ldy #160

  stx $$d004
  sty $$d006

; raster col line #3

$lbl_barcol3
  lda #8
  sta $$d020
  sta $$d021

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

; sprites x position #4

$lbl_sprite4_1
  ldx #50
  ldy #60

  stx $$d004
  sty $$d006

$lbl_sprite4_2
  ldx #70
  ldy #80

  stx $$d000
  sty $$d002

; raster col line #4

$lbl_barcol4
  lda #0
  sta $$d020
  sta $$d021

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
