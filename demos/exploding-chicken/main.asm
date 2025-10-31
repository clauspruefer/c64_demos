; Exploding Chicken Animation for Commodore 64
; Enhanced Color Mode (ECM) with hires display
; Using Colodore palette
; 8x8 character display (64 characters total)

* = $0801

; BASIC stub: SYS 2061
!byte $0b, $08, $0a, $00, $9e, $32, $30, $36, $31, $00, $00, $00

* = $080d

; Zero page variables for indirect addressing
source_ptr = $fb
dest_ptr = $fd

main:
    ; Disable interrupts during setup
    sei
    
    ; Clear screen with spaces
    ldx #$00
clear_screen:
    lda #$20        ; Space character
    sta $0400,x
    sta $0500,x
    sta $0600,x
    sta $0700,x
    dex
    bne clear_screen
    
    ; Set screen colors for Enhanced Color Mode
    ; Enhanced Color Mode uses background colors from $d021-$d024
    lda #$00        ; Background color 0 = black
    sta $d021
    lda #$0b        ; Background color 1 = dark grey  
    sta $d022
    lda #$0c        ; Background color 2 = medium grey
    sta $d023
    lda #$0f        ; Background color 3 = light grey
    sta $d024
    
    lda #$00
    sta $d020       ; Border color = black
    
    ; Enable Enhanced Color Mode (ECM)
    ; Set bit 6 of $d011 for ECM, bit 5 for bitmap mode (we want text mode)
    lda $d011
    ora #%01000000  ; Enable ECM (bit 6)
    and #%11011111  ; Disable bitmap mode (bit 5 clear)
    sta $d011
    
    ; Initialize frame counter
    lda #$00
    sta frame_index
    
    ; Setup raster interrupt
    lda #$7f
    sta $dc0d       ; Disable CIA interrupts
    sta $dd0d
    
    lda $dc0d       ; Acknowledge any pending interrupts
    lda $dd0d
    
    lda #$01
    sta $d01a       ; Enable raster interrupts
    
    lda #$00        ; Set raster line to 0
    sta $d012
    
    lda $d011       ; Clear high bit of raster line
    and #$7f
    sta $d011
    
    ; Set interrupt vector
    lda #<irq
    sta $0314
    lda #>irq
    sta $0315
    
    cli
    
    ; Main loop - just wait
main_loop:
    jmp main_loop

; Raster interrupt handler
irq:
    ; Acknowledge interrupt
    dec $d019
    
    ; Update display with current frame
    jsr update_display
    
    ; Slow down animation (update every 4th frame for ~15fps)
    inc anim_delay
    lda anim_delay
    cmp #$04
    bne skip_frame_advance
    
    ; Reset delay counter
    lda #$00
    sta anim_delay
    
    ; Advance to next frame
    inc frame_index
    lda frame_index
    cmp #16         ; Wrap at 16 frames
    bne skip_frame_advance
    lda #$00
    sta frame_index
    
skip_frame_advance:
    ; Return from interrupt
    jmp $ea81

; Update display with current animation frame
update_display:
    ; Calculate frame data address
    ; Each frame is 64 bytes (8x8 characters)
    ; address = animation_data + (frame_index * 64)
    
    ; Get base address
    lda #<animation_data
    sta source_ptr
    lda #>animation_data
    sta source_ptr+1
    
    ; Add frame_index * 64
    ; 64 = $40, so shift left 6 times
    lda frame_index
    asl                 ; * 2
    asl                 ; * 4  
    asl                 ; * 8
    asl                 ; * 16
    asl                 ; * 32
    asl                 ; * 64
    clc
    adc source_ptr
    sta source_ptr
    lda source_ptr+1
    adc #$00
    sta source_ptr+1
    
    ; Set destination to screen RAM center position
    ; Screen is 40x25, we want to center 8x8 display
    ; Center position: (40-8)/2 = 16 chars from left, (25-8)/2 = 8.5 ~ 8 rows from top
    ; Screen offset = 8 * 40 + 16 = 336 = $150
    lda #<($0400 + $150)
    sta dest_ptr
    lda #>($0400 + $150)
    sta dest_ptr+1
    
    ; Copy 8 rows of 8 characters
    ldx #$00        ; Row counter
copy_row_loop:
    ldy #$00        ; Column counter
copy_col_loop:
    lda (source_ptr),y
    sta (dest_ptr),y
    iny
    cpy #$08        ; 8 characters per row
    bne copy_col_loop
    
    ; Advance source pointer by 8 bytes
    lda source_ptr
    clc
    adc #$08
    sta source_ptr
    lda source_ptr+1
    adc #$00
    sta source_ptr+1
    
    ; Advance destination pointer to next screen row (40 chars)
    lda dest_ptr
    clc
    adc #$28        ; 40 = $28
    sta dest_ptr
    lda dest_ptr+1
    adc #$00
    sta dest_ptr+1
    
    inx
    cpx #$08        ; 8 rows
    bne copy_row_loop
    
    rts

; Update color RAM with animation colors
; In ECM, colors are determined by bits 6-7 of character code
; and the foreground color from color RAM
update_colors:
    ; Set color RAM to yellow for chicken
    ; Center position in color RAM: $d800 + $150
    lda #<($d800 + $150)
    sta dest_ptr
    lda #>($d800 + $150)
    sta dest_ptr+1
    
    ; Set 8x8 area to yellow
    ldx #$00
color_row_loop:
    ldy #$00
color_col_loop:
    lda #$07        ; Yellow
    sta (dest_ptr),y
    iny
    cpy #$08
    bne color_col_loop
    
    ; Next row
    lda dest_ptr
    clc
    adc #$28        ; 40 chars
    sta dest_ptr
    lda dest_ptr+1
    adc #$00
    sta dest_ptr+1
    
    inx
    cpx #$08
    bne color_row_loop
    
    rts

; Variables
frame_index:
    !byte $00

anim_delay:
    !byte $00

; Include animation data
!source "chicken-data.i"
