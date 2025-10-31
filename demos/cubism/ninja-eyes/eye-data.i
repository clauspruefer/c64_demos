; Eye character data - 3 rows x 8 columns per eye
; Top row, middle row (with pupils), bottom row
eye_chars:
    ; Top row
    !byte $20,$55,$40,$40,$40,$40,$49,$20
    ; Middle row (with pupil markers)
    !byte $55,$20,$a0,$a0,$a0,$20,$49,$20
    ; Bottom row
    !byte $20,$4a,$40,$40,$40,$40,$4b,$20

eye_colors:
    ; Colors: white for eye white, dark gray for pupil area
    !byte $01,$01,$01,$01,$01,$01,$01,$01
    !byte $01,$01,$0b,$0b,$0b,$01,$01,$01
    !byte $01,$01,$01,$01,$01,$01,$01,$01
