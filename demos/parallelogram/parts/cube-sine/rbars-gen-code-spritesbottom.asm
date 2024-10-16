; adjusted timing

nop
nop
nop
nop
nop

; sprites x position #1 -  template start

lbl_barcol21
  lda #0

lbl_sprite21_1
  ldx #50
  ldy #54

  stx $d000
  sty $d002

lbl_sprite21_2
  ldx #58
  ldy #62

  stx $d004
  sty $d006

  sta $d020
  sta $d021

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

lbl_sprite22_1
  ldx #80
  ldy #85

  stx $d000
  sty $d002

lbl_sprite22_2
  ldx #90
  ldy #95

  stx $d004
  sty $d006

; raster col line #2

lbl_barcol22
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

; sprites x position #3

lbl_sprite23_1
  ldx #120
  ldy #130

  stx $d000
  sty $d002

lbl_sprite23_2
  ldx #140
  ldy #150

  stx $d004
  sty $d006

; raster col line #3

lbl_barcol23
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

; sprites x position #4

lbl_sprite24_1
  ldx #80
  ldy #84

  stx $d004
  sty $d006

lbl_sprite24_2
  ldx #88
  ldy #92

  stx $d000
  sty $d002

; raster col line #4

lbl_barcol24
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
; sprites x position #1 -  template start

lbl_barcol25
  lda #0

lbl_sprite25_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite25_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

  sta $d021
  sta $d020

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

lbl_sprite26_1
  ldx #50
  ldy #60

  stx $d000
  sty $d002

lbl_sprite26_2
  ldx #70
  ldy #80

  stx $d004
  sty $d006

; raster col line #2

lbl_barcol26
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

; sprites x position #3

lbl_sprite27_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite27_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

; raster col line #3

lbl_barcol27
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

; sprites x position #4

lbl_sprite28_1
  ldx #50
  ldy #60

  stx $d004
  sty $d006

lbl_sprite28_2
  ldx #70
  ldy #80

  stx $d000
  sty $d002

; raster col line #4

lbl_barcol28
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
; sprites x position #1 -  template start

lbl_barcol29
  lda #0

lbl_sprite29_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite29_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

  sta $d021
  sta $d020

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

lbl_sprite30_1
  ldx #50
  ldy #60

  stx $d000
  sty $d002

lbl_sprite30_2
  ldx #70
  ldy #80

  stx $d004
  sty $d006

; raster col line #2

lbl_barcol30
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

; sprites x position #3

lbl_sprite31_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite31_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

; raster col line #3

lbl_barcol31
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

; sprites x position #4

lbl_sprite32_1
  ldx #50
  ldy #60

  stx $d004
  sty $d006

lbl_sprite32_2
  ldx #70
  ldy #80

  stx $d000
  sty $d002

; raster col line #4

lbl_barcol32
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
; sprites x position #1 -  template start

lbl_barcol33
  lda #0

lbl_sprite33_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite33_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

  sta $d021
  sta $d020

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

lbl_sprite34_1
  ldx #50
  ldy #60

  stx $d000
  sty $d002

lbl_sprite34_2
  ldx #70
  ldy #80

  stx $d004
  sty $d006

; raster col line #2

lbl_barcol34
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

; sprites x position #3

lbl_sprite35_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite35_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

; raster col line #3

lbl_barcol35
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

; sprites x position #4

lbl_sprite36_1
  ldx #50
  ldy #60

  stx $d004
  sty $d006

lbl_sprite36_2
  ldx #70
  ldy #80

  stx $d000
  sty $d002

; raster col line #4

lbl_barcol36
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
; sprites x position #1 -  template start

lbl_barcol37
  lda #0

lbl_sprite37_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite37_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

  sta $d021
  sta $d020

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

lbl_sprite38_1
  ldx #50
  ldy #60

  stx $d000
  sty $d002

lbl_sprite38_2
  ldx #70
  ldy #80

  stx $d004
  sty $d006

; raster col line #2

lbl_barcol38
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

; sprites x position #3

lbl_sprite39_1
  ldx #100
  ldy #120

  stx $d000
  sty $d002

lbl_sprite39_2
  ldx #140
  ldy #160

  stx $d004
  sty $d006

; raster col line #3

lbl_barcol39
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

; sprites x position #4

lbl_sprite40_1
  ldx #0
  ldy #0

  stx $d004
  sty $d006

lbl_sprite40_2
  ldx #0
  ldy #0

  stx $d000
  sty $d002

; raster col line #4

lbl_barcol40
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
