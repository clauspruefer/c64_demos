; Floodlight Animation for Commodore 64
; 3 moving and zooming circles with shadows
; 32x10 character display using color RAM

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
    
    ; Set screen to black background
    lda #$00
    sta $d020       ; Border color = black
    sta $d021       ; Background color = black
    
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
    
    ; Update color RAM with current frame
    jsr update_colors
    
    ; Advance to next frame
    inc frame_index
    lda frame_index
    cmp #64         ; Wrap at 64 frames
    bne skip_wrap
    lda #$00
    sta frame_index
skip_wrap:
    
    ; Return from interrupt
    jmp $ea81

; Update color RAM from animation data
update_colors:
    ; Calculate frame data address
    ; address = animation_data + (frame_index * 320)
    ; 320 = 256 + 64 = $100 + $40
    
    ; Get base address
    lda #<animation_data
    sta source_ptr
    lda #>animation_data
    sta source_ptr+1
    
    ; Add frame_index * 256
    lda frame_index
    clc
    adc source_ptr+1
    sta source_ptr+1
    
    ; Add frame_index * 64
    ; First calculate frame_index * 64
    lda frame_index
    lsr                 ; Divide by 4 gives us * 64 when we shift left 6 times
    lsr                 ; Actually easier to just shift left twice
    ; Let's use a different approach
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
    
    ; Set destination to color RAM
    lda #$00
    sta dest_ptr
    lda #$d8
    sta dest_ptr+1
    
    ; Copy 320 bytes (32x10)
    ; We'll do this in 4 chunks of 80 bytes to avoid page boundary issues
    ldx #$00
copy_loop1:
    ldy #$00
copy_inner1:
    lda (source_ptr),y
    sta (dest_ptr),y
    iny
    cpy #80
    bne copy_inner1
    
    ; Advance pointers
    lda source_ptr
    clc
    adc #80
    sta source_ptr
    lda source_ptr+1
    adc #$00
    sta source_ptr+1
    
    lda dest_ptr
    clc
    adc #80
    sta dest_ptr
    lda dest_ptr+1
    adc #$00
    sta dest_ptr+1
    
    inx
    cpx #$04        ; 4 chunks of 80 = 320 bytes
    bne copy_loop1
    
    rts

; Variables
frame_index:
    !byte $00

; Include animation data
!source "floodlights-data.i"
