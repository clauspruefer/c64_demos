; Halloween themed raster bar color data
; Orange, black, purple, and green color scheme
; C64 colors: 0=black, 2=red, 4=purple, 5=green, 8=orange, 9=brown

; Pattern: Black -> Orange -> Red -> Orange -> Black -> Purple -> Green -> Purple -> Black (repeating)
!byte $00,$00,$00,$09,$09,$08,$08,$02  ; Black fade to orange to red
!byte $02,$08,$08,$09,$09,$00,$00,$00  ; Red back to orange to black
!byte $00,$04,$04,$04,$05,$05,$05,$04  ; Black to purple to green
!byte $04,$04,$00,$00,$00,$09,$09,$08  ; Purple back through black to orange
!byte $08,$02,$02,$08,$08,$09,$09,$00  ; Orange red pattern
!byte $00,$00,$00,$04,$04,$05,$05,$04  ; Black purple green
!byte $04,$00,$00,$00,$09,$09,$08,$08  ; Back to orange
!byte $02,$02,$08,$08,$09,$09,$00,$00  ; Orange fade
!byte $00,$00,$04,$04,$04,$05,$05,$05  ; Purple green
!byte $04,$04,$04,$00,$00,$00,$09,$09  ; Purple fade to orange
!byte $08,$08,$02,$02,$08,$08,$09,$09  ; Orange red
!byte $00,$00,$00,$00,$04,$04,$05,$05  ; Black purple green
!byte $05,$04,$04,$00,$00,$00,$09,$09  ; Green back to black orange
!byte $08,$08,$02,$02,$08,$08,$09,$09  ; Orange red
!byte $00,$00,$00,$00,$04,$04,$04,$05  ; Black purple green
!byte $05,$05,$04,$04,$04,$00,$00,$00  ; Green purple fade
!byte $09,$09,$08,$08,$02,$02,$08,$08  ; Orange red
!byte $09,$09,$00,$00,$00,$00,$04,$04  ; Orange to black purple
!byte $05,$05,$05,$04,$04,$04,$00,$00  ; Green purple black
!byte $00,$09,$09,$08,$08,$02,$02,$08  ; Black orange red
!byte $08,$09,$09,$00,$00,$00,$00,$04  ; Orange black purple
!byte $04,$04,$05,$05,$05,$04,$04,$04  ; Purple green
!byte $00,$00,$00,$09,$09,$08,$08,$02  ; Black orange red
!byte $02,$08,$08,$09,$09,$00,$00,$00  ; Red orange black
!byte $00,$04,$04,$04,$05,$05,$05,$04  ; Purple green pattern
!byte $04,$04,$00,$00,$00,$09,$09,$08  ; Purple to orange
!byte $08,$02,$02,$08,$08,$09,$09,$00  ; Orange red fade
!byte $00,$00,$00,$04,$04,$05,$05,$04  ; Black purple green
!byte $04,$00,$00,$00,$09,$09,$08,$08  ; Purple black orange
!byte $02,$02,$08,$08,$09,$09,$00,$00  ; Red orange black
!byte $00,$00,$04,$04,$04,$05,$05,$05  ; Black purple green
!byte $04,$04,$04,$00,$00,$00,$00,$00  ; Purple fade to black
