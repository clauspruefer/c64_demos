; Ninja Eyes Demo - Last Ninja Inspired Animation
; Morphing eye pupils into 3D rotating cubes
;
; Memory Map:
; $0801-$080c : BASIC stub
; $080e+      : Main program
; $2000+      : Cube rotation frame data

!to "ninja-eyes.prg", cbm
!cpu 6510

* = $0801

; BASIC stub: 10 SYS 2062
; 2062 decimal = $080e hex (start of main program)
!byte $0c, $08  ; Link address
!byte $0a, $00  ; Line number 10
!byte $9e       ; SYS token
!byte $20       ; Space
!byte $32, $30, $36, $32  ; "2062" in ASCII
!byte $00       ; End of line
!byte $00, $00  ; End of program

* = $080e

main:
    ; Disable interrupts during setup
    sei

    ; Disable CIA interrupts
    lda #$7f
    sta $dc0d
    sta $dd0d

    ; Clear interrupt flags
    lda $dc0d
    lda $dd0d

    ; Clear screen
    jsr clear_screen
    
    ; Draw title
    jsr draw_title
    
    ; Setup character set for eyes
    jsr setup_charset
    
    ; Draw the two ninja eyes
    jsr draw_eyes
    
    ; Setup raster interrupt
    jsr setup_irq
    
    cli

main_loop:
    ; Infinite loop - all work done in IRQ
    jmp main_loop

;=============================================================================
; Clear screen
;=============================================================================
clear_screen:
    lda #$20  ; Space character
    ldx #0
clear_loop:
    ; Clear 4 pages of screen memory (1000 bytes total)
    ; Loop runs 256 times: 256 x 4 = 1024 bytes (covers entire screen)
    sta $0400,x
    sta $0500,x
    sta $0600,x
    sta $0700,x
    dex
    bne clear_loop
    
    ; Set screen colors
    lda #$00  ; Black
    sta $d020  ; Border
    sta $d021  ; Background
    
    rts

;=============================================================================
; Draw title text
;=============================================================================
draw_title:
    ; Draw title at row 5, centered
    ldx #0
title_loop:
    lda title_text,x
    beq title_done
    sta $0400 + 5*40 + 7,x
    lda #$0e  ; Light blue
    sta $d800 + 5*40 + 7,x
    inx
    bne title_loop
title_done:
    rts

title_text:
    !text "ninja eyes - morphing pupils"
    !byte 0

info_text:
    !text "  inspired by last ninja  "
    !byte 0

;=============================================================================
; Setup character set
;=============================================================================
setup_charset:
    ; We'll use the default charset for now, but point to custom area later
    ; For this demo, we'll use text mode and draw with characters
    
    lda #$15  ; Enable text mode, 25 rows
    sta $d011
    
    lda #$08  ; 40 columns
    sta $d016
    
    ; Draw info text at bottom
    ldx #0
info_loop:
    lda info_text,x
    beq info_done
    sta $0400 + 22*40 + 7,x
    lda #$0f  ; Light gray
    sta $d800 + 22*40 + 7,x
    inx
    bne info_loop
info_done:
    
    rts

;=============================================================================
; Draw the ninja eyes on screen
;=============================================================================
draw_eyes:
    ; Left eye position: row 10, col 8
    ; Right eye position: row 10, col 26
    
    ; Draw left eye
    ldx #0
draw_left_eye:
    lda eye_chars,x
    sta $0400 + 10*40 + 8,x
    lda eye_colors,x
    sta $d800 + 10*40 + 8,x
    inx
    cpx #24  ; 3 rows x 8 cols = 24 chars
    bne draw_left_eye
    
    ; Draw right eye
    ldx #0
draw_right_eye:
    lda eye_chars,x
    sta $0400 + 10*40 + 26,x
    lda eye_colors,x
    sta $d800 + 10*40 + 26,x
    inx
    cpx #24
    bne draw_right_eye
    
    rts

; Include generated eye data
!source "eye-data.i"

;=============================================================================
; Setup raster interrupt
;=============================================================================
setup_irq:
    ; Set raster line
    lda #$80
    sta $d012
    
    ; Set interrupt handler
    lda #<irq_handler
    sta $0314
    lda #>irq_handler
    sta $0315
    
    ; Enable raster interrupt
    lda #$01
    sta $d01a
    
    ; Clear high bit of raster compare
    lda $d011
    and #$7f
    sta $d011
    
    rts

;=============================================================================
; Animation state
;=============================================================================
anim_phase:     !byte 0    ; 0=eyes, 1=morphing, 2=cube, 3=unmorphing
anim_counter:   !byte 0    ; Frame counter
cube_frame:     !byte 0    ; Current cube rotation frame
morph_timer:    !byte 0    ; Delay between morph stages

;=============================================================================
; IRQ Handler - Animate the cube rotation
;=============================================================================
irq_handler:
    ; Save registers
    pha
    txa
    pha
    tya
    pha
    
    ; Increment animation counter
    inc anim_counter
    lda anim_counter
    and #$03  ; Slow down animation (every 4 frames)
    bne irq_done
    
    ; Update cube rotation
    inc cube_frame
    lda cube_frame
    and #$3f  ; 64 frames loop
    sta cube_frame
    
    ; Render cube in pupil positions
    jsr render_cube_pupils
    
irq_done:
    ; Acknowledge interrupt
    asl $d019
    
    ; Restore registers
    pla
    tay
    pla
    tax
    pla
    
    ; Jump to standard IRQ handler
    jmp $ea81

;=============================================================================
; Render cube in both pupil positions
;=============================================================================
render_cube_pupils:
    ; Get current cube frame data
    ldx cube_frame
    
    ; Calculate offset in cube data (16 bytes per frame)
    ; Multiply by 16: shift left 4 times
    txa
    asl
    asl
    asl
    asl
    tax
    
    ; Draw left pupil cube
    jsr draw_cube_left
    
    ; Draw right pupil cube  
    jsr draw_cube_right
    
    rts

;=============================================================================
; Draw cube in left pupil area (row 11, col 10)
;=============================================================================
draw_cube_left:
    ; Base screen position for left pupil (screen addr $0400 + 11*40 + 10 = $05ba)
    lda #$ba
    sta screen_ptr
    lda #$05
    sta screen_ptr + 1
    
    ; Draw the cube using character blocks
    jsr draw_cube_at_position
    
    rts

;=============================================================================
; Draw cube in right pupil area (row 11, col 28)
;=============================================================================
draw_cube_right:
    ; Base screen position for right pupil (screen addr $0400 + 11*40 + 28 = $05d0)
    lda #$d0
    sta screen_ptr
    lda #$05
    sta screen_ptr + 1
    
    ; Draw the cube using character blocks
    jsr draw_cube_at_position
    
    rts

;=============================================================================
; Draw cube at current screen position
; Uses the actual 3D vertex data to render cube wireframe
;=============================================================================
draw_cube_at_position:
    ; Get current cube frame data address
    lda cube_frame
    and #$3f  ; Keep in range 0-63
    
    ; Multiply by 16 (shift left 4 times) to get frame offset
    ; Store in a temp location for calculation
    sta temp_frame
    asl
    asl
    asl
    asl
    tax  ; X now has the offset (0, 16, 32, etc.)
    
    ; Clear pupil area first (3x3 characters for pupil)
    lda #$20  ; Space character
    ldy #0
clear_pupil:
    sta (screen_ptr),y
    iny
    sta (screen_ptr),y
    iny
    sta (screen_ptr),y
    
    ; Move to next row
    tya
    clc
    adc #40-3  ; Add 40 for next row, minus 3 we already wrote
    tay
    
    cpy #120  ; Done 3 rows?
    bcc clear_pupil
    
    ; Now draw cube vertices as character blocks
    ; Get first 4 vertices (back face of cube)
    ldy #0
    
draw_vertex_loop:
    ; Get X coordinate
    lda cube_data,x
    inx
    ; Convert to screen offset (0-23 -> 0-2 for our 3-char wide area)
    lsr
    lsr
    lsr  ; Divide by 8 to get column (0-2)
    sta vertex_col
    
    ; Get Y coordinate  
    lda cube_data,x
    inx
    ; Convert to screen offset (0-7 -> 0-2 for our 3-char tall area)
    lsr
    lsr  ; Divide by 4 to get row (0-1, with some in row 2)
    sta vertex_row
    
    ; Calculate screen offset: row*40 + col
    lda vertex_row
    cmp #2
    bcs skip_vertex  ; Skip if beyond our drawing area
    
    ; Multiply row by 40
    asl  ; row * 2
    sta temp_row
    asl  ; row * 4
    asl  ; row * 8
    clc
    adc temp_row  ; row * 10
    asl  ; row * 20
    asl  ; row * 40
    
    ; Add column
    clc
    adc vertex_col
    tay
    
    ; Draw the vertex
    lda #$a0  ; Block character
    sta (screen_ptr),y
    
skip_vertex:
    ; Check if we've drawn enough vertices
    ; We process 8 vertices (16 bytes)
    txa
    and #$0f
    bne draw_vertex_loop
    
    rts

; Temporary variables for cube drawing
temp_frame: !byte 0
vertex_col: !byte 0
vertex_row: !byte 0
temp_row:   !byte 0

;=============================================================================
; Variables - Zero page addresses for pointers
;=============================================================================
screen_ptr = $fb  ; Use zero page locations $fb-$fc for screen pointer

;=============================================================================
; Include cube rotation data
;=============================================================================
* = $2000
cube_data:
!source "cube-rotation-data.i"
