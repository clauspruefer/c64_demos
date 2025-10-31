; Pumpkin Jack-O'-Lantern Sprite Data
; 24x21 pixels sprite for Halloween demo
; Each sprite is 63 bytes (21 rows * 3 bytes each)

; Top of pumpkin (stem)
!byte $00,$18,$00  ; Row 1  - small stem
!byte $00,$3c,$00  ; Row 2  - stem base
!byte $00,$7e,$00  ; Row 3  - top curve
!byte $01,$ff,$80  ; Row 4  - pumpkin top
!byte $03,$ff,$c0  ; Row 5  - wider
!byte $07,$ff,$e0  ; Row 6  - wider still
!byte $0f,$ff,$f0  ; Row 7  - widest part
!byte $1f,$ff,$f8  ; Row 8  - full width
!byte $3f,$ff,$fc  ; Row 9  - full body
!byte $3e,$18,$7c  ; Row 10 - left eye (triangle)
!byte $3c,$3c,$3c  ; Row 11 - eyes filled
!byte $38,$7e,$1c  ; Row 12 - bottom of eyes
!byte $3f,$ff,$fc  ; Row 13 - between eyes and nose
!byte $3f,$81,$fc  ; Row 14 - small triangular nose
!byte $3f,$c3,$fc  ; Row 15 - nose wider
!byte $3f,$ff,$fc  ; Row 16 - between nose and mouth
!byte $3e,$00,$7c  ; Row 17 - mouth starts
!byte $3c,$81,$3c  ; Row 18 - jagged teeth
!byte $39,$c3,$9c  ; Row 19 - more teeth
!byte $1f,$ff,$f8  ; Row 20 - bottom curve
!byte $07,$ff,$e0  ; Row 21 - bottom of pumpkin
