; Frankenstein Animation for Commodore 64
; Classic Frankenstein monster face in black and white
; 40x25 character display using color RAM

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
    
    ; Advance to next frame (slower animation speed)
    inc frame_counter
    lda frame_counter
    cmp #$02        ; Update every 2 frames (slower)
    bne skip_frame_advance
    
    lda #$00
    sta frame_counter
    
    inc frame_index
    lda frame_index
    cmp #FRAME_COUNT    ; Wrap at total frame count
    bne skip_frame_advance
    lda #$00
    sta frame_index
skip_frame_advance:
    
    ; Return from interrupt
    jmp $ea81

; Update color RAM from animation data
update_colors:
    ; Calculate frame data address
    ; address = animation_data + (frame_index * FRAME_SIZE)
    ; FRAME_SIZE = 1000 = $03e8
    
    ; Get base address
    lda #<animation_data
    sta source_ptr
    lda #>animation_data
    sta source_ptr+1
    
    ; Multiply frame_index by 1000 ($03e8)
    ; We'll do this as frame_index * 256 * 3 + frame_index * 232
    ; Actually, simpler: frame_index * (768 + 232) = frame_index * 1000
    ; Let's use frame_index * 256 * 4 - frame_index * 24
    ; Actually simplest: add in loop or use lookup
    
    ; For frame_index * 1000, we use:
    ; 1000 = 256*3 + 232 = 768 + 232
    
    ; Add frame_index * 256 * 3 (high byte contribution)
    lda frame_index
    sta temp_index
    
    ; Add 3 * frame_index to high byte
    asl temp_index  ; * 2
    lda temp_index
    clc
    adc frame_index ; * 3
    clc
    adc source_ptr+1
    sta source_ptr+1
    
    ; Add 232 * frame_index to address
    ; 232 = 256 - 24
    ; So we subtract 24 * frame_index
    lda frame_index
    asl             ; * 2
    asl             ; * 4
    asl             ; * 8
    sta temp_val
    asl             ; * 16
    clc
    adc temp_val    ; * 24
    
    ; Subtract from source_ptr
    sta temp_val
    lda source_ptr
    sec
    sbc temp_val
    sta source_ptr
    lda source_ptr+1
    sbc #$00
    sta source_ptr+1
    
    ; Set destination to color RAM
    lda #$00
    sta dest_ptr
    lda #$d8
    sta dest_ptr+1
    
    ; Copy 1000 bytes (40x25) in chunks
    ; We'll do 10 chunks of 100 bytes
    ldx #$00
copy_loop:
    ldy #$00
copy_inner:
    lda (source_ptr),y
    sta (dest_ptr),y
    iny
    cpy #100
    bne copy_inner
    
    ; Advance pointers by 100 bytes
    lda source_ptr
    clc
    adc #100
    sta source_ptr
    lda source_ptr+1
    adc #$00
    sta source_ptr+1
    
    lda dest_ptr
    clc
    adc #100
    sta dest_ptr
    lda dest_ptr+1
    adc #$00
    sta dest_ptr+1
    
    inx
    cpx #$0a        ; 10 chunks of 100 = 1000 bytes
    bne copy_loop
    
    rts

; Variables
frame_index:
    !byte $00
    
frame_counter:
    !byte $00
    
temp_index:
    !byte $00
    
temp_val:
    !byte $00

; Include animation data
!source "frankenstein-data.i"
