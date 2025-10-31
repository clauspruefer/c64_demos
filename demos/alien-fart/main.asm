; Alien Fart Animation for Commodore 64
; An alien in a flying saucer eating donuts, a man passes by eating hamburger
; The alien flies by and farts, sending the hamburger flying!

*=$0801
!byte $0b,$08,$0a,$00,$9e,$32,$30,$36,$31,$00,$00,$00  ; BASIC: 10 SYS 2061

; === Zero Page Variables ===
frame_counter = $fb
animation_state = $fc
alien_x = $fd
man_x = $fe
hamburger_x = $50
hamburger_y = $51
fart_frame = $52

main:
    ; Clear screen
    lda #$20
    ldx #0
clear_loop:
    sta $0400, x
    sta $0500, x
    sta $0600, x
    sta $0700, x
    dex
    bne clear_loop

    ; Set background and border colors
    lda #$06        ; Blue background (sky)
    sta $d021
    lda #$0e        ; Light blue border
    sta $d020

    ; Initialize animation variables
    lda #0
    sta frame_counter
    sta animation_state
    sta fart_frame
    
    lda #100
    sta alien_x
    lda #24         ; Start man off-screen left
    sta man_x
    lda #180
    sta hamburger_x
    lda #200
    sta hamburger_y

    ; Setup sprites
    jsr setup_sprites

    ; Setup interrupt
    sei

    ; Disable CIA interrupts
    lda #%01111111
    sta $dc0d
    sta $dd0d

    ; Clear VIC raster register
    lda $d011
    and #%01111111
    sta $d011

    ; Acknowledge pending interrupts
    lda $dc0d
    lda $dd0d

    ; Set interrupt on raster line 250 (bottom of screen for timing)
    lda #250
    sta $d012

    ; Set interrupt address
    lda #<irq
    ldx #>irq
    sta $0314
    stx $0315

    ; Enable raster IRQ
    lda #%00000001
    sta $d01a

    cli

endless:
    jmp endless

; === Setup Sprites ===
setup_sprites:
    ; Enable all sprites (0-7, we'll use 0-4)
    lda #%00011111
    sta $d015

    ; Set sprite colors
    lda #$0a        ; Red for alien
    sta $d027
    lda #$0c        ; Gray for flying saucer
    sta $d028
    lda #$07        ; Yellow for donut
    sta $d029
    lda #$0f        ; Light gray for man
    sta $d02a
    lda #$08        ; Orange for hamburger
    sta $d02b

    ; Set sprite pointers
    lda #$40        ; Alien sprite data at $1000
    sta $07f8
    lda #$41        ; Saucer sprite data at $1040
    sta $07f9
    lda #$42        ; Donut sprite data at $1080
    sta $07fa
    lda #$43        ; Man sprite data at $10c0
    sta $07fb
    lda #$44        ; Hamburger sprite data at $1100
    sta $07fc

    ; Initial sprite positions
    ; Alien (sprite 0) - in saucer eating donuts
    lda alien_x
    sta $d000       ; X position
    lda #100
    sta $d001       ; Y position

    ; Saucer (sprite 1) - same position as alien
    lda alien_x
    sta $d002       ; X position
    lda #100
    sta $d003       ; Y position

    ; Donut (sprite 2) - near alien's mouth
    lda alien_x
    clc
    adc #10
    sta $d004       ; X position
    lda #90
    sta $d005       ; Y position

    ; Man (sprite 3) - walking from left
    lda man_x
    sta $d006       ; X position
    lda #200
    sta $d007       ; Y position (ground level)

    ; Hamburger (sprite 4) - in man's hand initially
    lda man_x
    clc
    adc #5
    sta $d008       ; X position
    lda #190
    sta $d009       ; Y position

    ; No sprite X MSB initially
    lda #0
    sta $d010

    ; Sprites in front of background
    lda #0
    sta $d01b

    rts

; === Interrupt Handler ===
irq:
    ; Acknowledge interrupt
    asl $d019

    ; Update animation
    jsr update_animation

    ; Increment frame counter
    inc frame_counter

    ; Return from interrupt
    jmp $ea81

; === Animation Update ===
update_animation:
    lda animation_state
    cmp #0
    bne check_state_1
    jmp state_0_alien_eating
check_state_1:
    cmp #1
    bne check_state_2
    jmp state_1_man_walking
check_state_2:
    cmp #2
    bne check_state_3
    jmp state_2_alien_flies
check_state_3:
    cmp #3
    bne update_done
    jmp state_3_hamburger_flying
update_done:
    rts

; State 0: Alien eating donuts (60 frames)
state_0_alien_eating:
    ; Animate donut moving towards alien
    lda frame_counter
    and #$08        ; Simple bounce animation
    beq donut_near
    lda #10
    jmp donut_pos
donut_near:
    lda #5
donut_pos:
    clc
    adc alien_x
    sta $d004       ; Donut X

    ; After 60 frames, move to next state
    lda frame_counter
    cmp #60
    bcc done_state_0
    lda #0
    sta frame_counter
    inc animation_state
done_state_0:
    rts

; State 1: Man walking by eating hamburger (120 frames)
state_1_man_walking:
    ; Move man from left to right
    inc man_x
    lda man_x
    sta $d006       ; Man X position
    
    ; Update hamburger position to follow man
    lda man_x
    clc
    adc #5
    sta $d008       ; Hamburger X
    
    ; Check if man has walked far enough
    lda frame_counter
    cmp #120
    bcc done_state_1
    lda #0
    sta frame_counter
    inc animation_state
done_state_1:
    rts

; State 2: Alien flies by man's face (30 frames)
state_2_alien_flies:
    ; Move alien and saucer down and to the right
    lda frame_counter
    lsr
    lsr             ; Divide by 4 for smoother movement
    clc
    adc #100
    sta $d000       ; Alien X
    sta $d002       ; Saucer X
    
    lda frame_counter
    lsl
    clc
    adc #100
    sta $d001       ; Alien Y
    sta $d003       ; Saucer Y
    
    ; Move donut with alien
    lda $d000
    clc
    adc #10
    sta $d004       ; Donut X
    lda $d001
    sec
    sbc #10
    sta $d005       ; Donut Y
    
    ; At frame 20, trigger fart
    lda frame_counter
    cmp #20
    bne no_fart
    lda #1
    sta fart_frame
no_fart:
    
    ; Visual fart effect (change border color briefly)
    lda fart_frame
    beq no_fart_effect
    cmp #5
    bcs fart_done
    inc fart_frame
    lda #$05        ; Green for fart cloud
    sta $d020
    jmp check_state_2_end
fart_done:
    lda #0
    sta fart_frame
    lda #$0e        ; Reset border
    sta $d020
no_fart_effect:

check_state_2_end:
    lda frame_counter
    cmp #30
    bcc done_state_2
    lda #0
    sta frame_counter
    inc animation_state
    
    ; Store hamburger starting position for physics
    lda $d008
    sta hamburger_x
    lda $d009
    sta hamburger_y
done_state_2:
    rts

; State 3: Hamburger flying away (60 frames)
state_3_hamburger_flying:
    ; Hamburger flies to the right and up, then falls
    lda frame_counter
    lsr             ; Divide by 2 for speed
    clc
    adc hamburger_x
    sta $d008       ; Hamburger X moves right
    
    ; Parabolic motion for Y (up then down)
    lda frame_counter
    cmp #30
    bcs falling
    ; Going up
    lda hamburger_y
    sec
    sbc #3
    sta hamburger_y
    jmp update_burger_y
falling:
    ; Going down
    lda hamburger_y
    clc
    adc #3
    sta hamburger_y
update_burger_y:
    lda hamburger_y
    sta $d009
    
    ; After 60 frames, reset to state 0
    lda frame_counter
    cmp #60
    bcc done_state_3
    lda #0
    sta frame_counter
    sta animation_state
    
    ; Reset positions
    lda #100
    sta alien_x
    lda #24
    sta man_x
    
    ; Reset sprite positions
    jsr setup_sprites
done_state_3:
    rts

; === Sprite Data ===
; Each sprite is 63 bytes (21 rows of 3 bytes)

* = $1000
; Sprite 0: Alien (simple head with eyes and antennae)
alien_sprite:
!byte %00000000, %01100000, %00000000  ; Row 1: Antennae
!byte %00000000, %11110000, %00000000  ; Row 2
!byte %00000001, %11111000, %00000000  ; Row 3: Head top
!byte %00000011, %11111100, %00000000  ; Row 4
!byte %00000111, %11111110, %00000000  ; Row 5
!byte %00000111, %11111110, %00000000  ; Row 6
!byte %00001111, %11111111, %00000000  ; Row 7
!byte %00001111, %11111111, %00000000  ; Row 8: Main head
!byte %00011100, %00000111, %00000000  ; Row 9: Eyes
!byte %00011100, %00000111, %00000000  ; Row 10: Eyes
!byte %00001111, %11111111, %00000000  ; Row 11
!byte %00001111, %11111111, %00000000  ; Row 12
!byte %00000111, %11111110, %00000000  ; Row 13
!byte %00000111, %11111110, %00000000  ; Row 14: Chin
!byte %00000011, %11111100, %00000000  ; Row 15
!byte %00000001, %11111000, %00000000  ; Row 16
!byte %00000000, %11110000, %00000000  ; Row 17
!byte %00000000, %01100000, %00000000  ; Row 18
!byte %00000000, %00000000, %00000000  ; Row 19
!byte %00000000, %00000000, %00000000  ; Row 20
!byte %00000000, %00000000, %00000000  ; Row 21

* = $1040
; Sprite 1: Flying Saucer
saucer_sprite:
!byte %00000000, %00000000, %00000000  ; Row 1
!byte %00000000, %00000000, %00000000  ; Row 2
!byte %00000001, %11111000, %00000000  ; Row 3: Dome top
!byte %00000011, %11111100, %00000000  ; Row 4
!byte %00000111, %11111110, %00000000  ; Row 5
!byte %00001111, %11111111, %00000000  ; Row 6: Dome
!byte %00011111, %11111111, %10000000  ; Row 7
!byte %00111111, %11111111, %11000000  ; Row 8: Main disk top
!byte %01111111, %11111111, %11100000  ; Row 9
!byte %11111111, %11111111, %11110000  ; Row 10: Widest part
!byte %11111111, %11111111, %11110000  ; Row 11
!byte %01111111, %11111111, %11100000  ; Row 12
!byte %00111111, %11111111, %11000000  ; Row 13: Disk bottom
!byte %00011111, %11111111, %10000000  ; Row 14
!byte %00000111, %11111110, %00000000  ; Row 15
!byte %00000000, %00000000, %00000000  ; Row 16
!byte %00000000, %00000000, %00000000  ; Row 17
!byte %00000000, %00000000, %00000000  ; Row 18
!byte %00000000, %00000000, %00000000  ; Row 19
!byte %00000000, %00000000, %00000000  ; Row 20
!byte %00000000, %00000000, %00000000  ; Row 21

* = $1080
; Sprite 2: Donut
donut_sprite:
!byte %00000000, %00000000, %00000000  ; Row 1
!byte %00000000, %00000000, %00000000  ; Row 2
!byte %00000000, %00000000, %00000000  ; Row 3
!byte %00000001, %11111000, %00000000  ; Row 4: Top
!byte %00000011, %11111100, %00000000  ; Row 5
!byte %00000111, %11111110, %00000000  ; Row 6
!byte %00001111, %00001111, %00000000  ; Row 7: Hole starts
!byte %00001110, %00000111, %00000000  ; Row 8
!byte %00001110, %00000111, %00000000  ; Row 9
!byte %00001110, %00000111, %00000000  ; Row 10: Hole middle
!byte %00001110, %00000111, %00000000  ; Row 11
!byte %00001110, %00000111, %00000000  ; Row 12
!byte %00001111, %00001111, %00000000  ; Row 13: Hole ends
!byte %00000111, %11111110, %00000000  ; Row 14
!byte %00000011, %11111100, %00000000  ; Row 15
!byte %00000001, %11111000, %00000000  ; Row 16: Bottom
!byte %00000000, %00000000, %00000000  ; Row 17
!byte %00000000, %00000000, %00000000  ; Row 18
!byte %00000000, %00000000, %00000000  ; Row 19
!byte %00000000, %00000000, %00000000  ; Row 20
!byte %00000000, %00000000, %00000000  ; Row 21

* = $10c0
; Sprite 3: Man (stick figure with head)
man_sprite:
!byte %00000000, %00000000, %00000000  ; Row 1
!byte %00000001, %11111000, %00000000  ; Row 2: Head top
!byte %00000011, %11111100, %00000000  ; Row 3
!byte %00000011, %11111100, %00000000  ; Row 4
!byte %00000001, %11111000, %00000000  ; Row 5: Head bottom
!byte %00000001, %11111000, %00000000  ; Row 6: Neck
!byte %00000001, %11111000, %00000000  ; Row 7
!byte %00001111, %11111111, %00000000  ; Row 8: Arms
!byte %00001111, %11111111, %00000000  ; Row 9
!byte %00000001, %11111000, %00000000  ; Row 10: Body
!byte %00000001, %11111000, %00000000  ; Row 11
!byte %00000001, %11111000, %00000000  ; Row 12
!byte %00000001, %11111000, %00000000  ; Row 13
!byte %00000001, %11111000, %00000000  ; Row 14
!byte %00000001, %11111000, %00000000  ; Row 15
!byte %00000011, %10011100, %00000000  ; Row 16: Legs
!byte %00000011, %10011100, %00000000  ; Row 17
!byte %00000011, %10011100, %00000000  ; Row 18
!byte %00000011, %00001100, %00000000  ; Row 19
!byte %00000011, %00001100, %00000000  ; Row 20
!byte %00000011, %00001100, %00000000  ; Row 21

* = $1100
; Sprite 4: Hamburger
hamburger_sprite:
!byte %00000000, %00000000, %00000000  ; Row 1
!byte %00000000, %00000000, %00000000  ; Row 2
!byte %00000000, %00000000, %00000000  ; Row 3
!byte %00000111, %11111110, %00000000  ; Row 4: Top bun
!byte %00001111, %11111111, %00000000  ; Row 5
!byte %00011111, %11111111, %10000000  ; Row 6
!byte %00011100, %00000001, %10000000  ; Row 7: Lettuce
!byte %00011010, %10101011, %10000000  ; Row 8
!byte %00011111, %11111111, %10000000  ; Row 9: Patty
!byte %00011111, %11111111, %10000000  ; Row 10
!byte %00011111, %11111111, %10000000  ; Row 11
!byte %00011100, %11100111, %10000000  ; Row 12: Cheese
!byte %00011111, %11111111, %10000000  ; Row 13
!byte %00011111, %11111111, %10000000  ; Row 14: Bottom bun
!byte %00001111, %11111111, %00000000  ; Row 15
!byte %00000111, %11111110, %00000000  ; Row 16
!byte %00000000, %00000000, %00000000  ; Row 17
!byte %00000000, %00000000, %00000000  ; Row 18
!byte %00000000, %00000000, %00000000  ; Row 19
!byte %00000000, %00000000, %00000000  ; Row 20
!byte %00000000, %00000000, %00000000  ; Row 21
