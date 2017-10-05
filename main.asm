
       THUMB
       ;AREA    DATA, ALIGN=2   
       AREA    |.text|, CODE, READONLY, ALIGN=2
       EXPORT  Start
	   ALIGN 
	   ;unlock 0x4C4F434B
	   
	   ;PF4 is SW1
	   ;PF0 is SW2
	   ;PF1 is RGB Red
	   ;Enable Clock RCGCGPIO p338
	   ;Set direction 1 is out 0 is in. GPIODIR
	   ;DEN 
	   ; 0x3FC
	   
	   ;GPIO Port B (APB): 0x4000.5000
	   ;GPIO Port E (APB): 0x4002.4000
	   
	   
sysTick equ 0xe000e000

port_B equ 0x40005000 ;Port B
port_E equ 0x40024000
port_C equ 0x40006000	
	
Start  
	mov32 R0, #0x400FE108 ; Enable GPIO Clock
	mov R1, #0x32
	str R1, [R0]
	

		
	mov32 R0, port_B ;Port B
	;mov R1, #0x4C4F434B
	mov R1, #0x0
	str R1, [R0, #0x420] ;turn off alt function, may need unlock
	mov R1,#0xFF
	str R1,[R0,#0x400] ;Port B output 0-7
	mov R1,#0xFF
	str R1,[R0,#0x51C] ;Port B pin 0-7 on
	
	
	mov32 R0, port_E ;Port E
	mov R1, #0x0
	str R1, [R0, #0x420] ;turn off alt function, may need unlock
	mov R1,#0x03
	str R1,[R0,#0x400] ;Port E output 0-1, input 2-3
	mov R1, #0x0c
	str R1, [R0,#0x510] ;pull up
	mov R1,#0x0f
	str R1,[R0,#0x51C] ;Port E pin 0-3 on
	
	
	;mov32 R0, port_C
	;mov R1, #0x0
	;str R1, [R0, #0x420] ;turn off alt function	
	;mov R1, #0x0
	;str R1, [R0, #0x400] ;Port C input 4-7
	;mov R1, #0xf0
	;str R1, [R0,#0x510] ;pull up *****may be bug here******
	;mov R1, #0xf0
	;str R1, [R0, #0x51c] ;Port C pin 4-7 on
	
	
	
	mov R9, #0x0 ;draw counter
	mov R11, #0x20 ;player 1 position
	mov R12, #0x10 ;player 2 position
	mov r2, #0 ;ready states of p1[0] and p2[1]
	mov32 r0, port_B
	mov r1, #0x30
	mvn r1, r1
	str r1, [r0, #0x3fc] ;turns on center leds 
	mov32 r0, port_E
	mov r1, #0x3
	str r1, [r0, #0xc]

STANDBY

	mov32 r0, port_B
	ldr r1, [r0, #0xc0] ;load current values of led 4,5
	mvn r1, r1
	mvn r3, r2
	and r1, r3, lsl #2
	str r1, [r0, #0xc0] ;set led 4,5 to inverted value
	

	mov32 r3, #0x2625a00 ; 0.5 Seconds
	bl TIMER
	
blinkDelay
	mov32 r0, port_E
	ldr R1, [R0, #0x30] ;get current buttons pressed
	eor r1, #0xc
	orr r2, r1 ;refresh ready states of players
	cmp r2, #0x0c ;p1 and p2 are ready, might be #0xc
	beq START_GAME ;start game
    
    mov32 r0, sysTick
	ldr r1, [r0, #0x10]
	cmp r3, r1, lsr #16
	bne blinkDelay
	b STANDBY
	
TIMER
	push {r0}
	push {r1}
	
	mov32 r0, sysTick
	mov r1, #4
	str r1, [r0, #0x10] ;stop sysTick, use system clock
	;mov32 r1, #0x7a1200 ;7a1200
	str r3, [r0, #0x14] ;set reset value to r3
	str r3, [r0, #0x18] ;set current value to r3
	ldr r1, [r0, #0x10]
	orr r1, #0x1
	str r1, [r0, #0x10] ;enable clock
	mov r3, #0x1
	
	pop {r1}
	pop {r0}
	bx lr

UPDATE_LED
	push{r0}
	push{r1}
	push{r3}
	
	orr r3, r11, r12 ;which leds turn on
	mvn r3, r3
	mov32 r0, port_B
	str r3, [r0, #0x3fc]
	lsr r3, #8
	mov32 r0, port_E
	str r3, [r0, #0xc]
	
	pop{r3}
	pop{r1}
	pop{r0}
	bx lr

RND12
    push{r0}
    push{r1}
    push{r2}

    mov32 r0, sysTick
    ldr r3, [r0, #0x18] ;load current sysTick
    ldr r2, [r0, #0x14] ;load reset sysTick value
    udiv r3, r2 ;current/max = value 0-1
    
    mov32 r1, #0x4c4b400 ;1 second
    add r3, r1 ;add 1 second
    
    pop{r2}
    pop{r1}
    pop{r0}
	bx lr
	
START_GAME
	mov32 r0, port_B
	mov r1, #0xcf ; cf=1100 1111, turn on center led
	str r1, [r0, #0xc0] ;load current values of led 4,5
	
Loop
	bl RND12 ;get random value 1-2 and store in r3
	bl TIMER ;use value of r3 from rnd12
WHILE2 ;while(!timer.done)
	mov32 r0, port_E
	ldr R1, [R0, #0x30] ;get current buttons pressed
	eor r1, #0xc
	cmp r1, #0x0000
	bne Loop ;player pressed button early
	
	mov32 r0, sysTick
	ldr r1, [r0, #0x10] 
	lsr r1, #16
	cmp r3, r1 ;is timer done
	bne WHILE2 ;end while(!timer.done)
	
	lsl r11, #1 ;move p1 left 1
	lsr r12, #1 ;move p2 right 1
	bl UPDATE_LED
	
GET_FIRST_TURN	
	mov32 r0, port_E
	ldr R1, [R0, #0x30] ;get current buttons pressed
	eor r1, #0xc
	cmp r1, #0x8
	bne CHECK_P2 ;player 1 button not pressed
	bl PLAYER1_FIRST
    b WHILE3
CHECK_P2
	cmp r1, #0x4
	bne GET_FIRST_TURN ;recheck buttons, p2 button not pressed
	bl PLAYER2_FIRST
	
WHILE3 ;while(!timer.done)
	
	mov32 r0, port_E
	ldr r1, [r0, #0x20] ;get button p1
	eor r1, #0x8 ;inverse button, 1 on 0 off
	lsr r1, #3 ;move p1 bit to first position
    cmp r1, r10
    bne skipP1 
    ;if(p1.pressed && playerturn ==p1)
	lsr r11, #1 ;move p1 right
    bl UPDATE_LED
    add r9, #1 ;draw++
    b Loop
skipP1
    ldr r1, [r0, #0x10] ;get button p2
	eor r1, #0x4 ;inverse button, 1 on 0 off
	lsr r1, #1 ;move p2 bit to second position
    cmp r1, r10
    bne skipP2
    ;if(p2.pressed && playerturn ==p2)
    lsl r12, #1 ;move p2 left
    bl UPDATE_LED
    add r9, #1 ;draw++
    b Loop
skipP2 ;no buttons pressed

	mov32 r0, sysTick
	ldr r1, [r0, #0x10] 
	cmp r3, r1, lsr #16 ;is timer done
	bne WHILE3
    
    mov r9, #0 
;    mov32 r0, port_E
;    ldr r1, [r0, #0x10]
;    eor r1, #0x4
;    lsr r1, #2 
    cmp r10, #0x1
    bne p2Turn
    lsl r12, #1 ;move p2 left
    bl UPDATE_LED
    b checkWin
p2Turn
    lsr r11, #1 ;move p1 right 1
    bl UPDATE_LED
    b checkWin
    
checkWin
    cmp r11, #0x200 ;check if p1 on left edge
    beq BLINK
    cmp r12, #0x1 ;check if p2 on right edge
	beq BLINK
	b Loop
    
BLINK
    mov r4, r11
    mov r5, r12
blink1
    eor r11, r4 ;flip r11
    eor r12, r5 ;flip r12
    bl UPDATE_LED
    mov32 r3, #0x2625a00
    push{lr}
    bl TIMER ;delay of 0.5 sec
    pop{lr}
    mov32 r0, sysTick
blinkDelay1
	ldr r1, [r0, #0x10]
	cmp r3, r1, lsr #16
	bne blinkDelay1 ;delay of 0.5 sec
    b blink1 ;endlessly loop until reset
	
    
PLAYER1_FIRST
	
	lsr r11, #1 ;move p1 right 1
    push{lr}
    bl UPDATE_LED
    pop{lr}
	mov r10, #0x2 ;wait on p2
	;*****store switch pins 0,1 in r4**********
	mov r3, #0x0 ;dip switch value ****TODO, get did switch values
    push{lr}
	bl COMPUTE_PLAYER_DELAY
    pop{lr}
	mov r3, r5 ;store value r5 from COMPUTE_PLAYER_DELAY into R3 for timer 
    push{lr}
	bl TIMER
    pop{lr}
	bx lr

PLAYER2_FIRST

	lsl r12, #1 ;move p2 left 1
    push{lr}
    bl UPDATE_LED
    pop{lr}
	mov r10, #0x1 ;wait on p1
	mov r3, #0x0 ;dip switch value ****TODO, get did switch values
	;**********store switch pins 2,3 in r3, use lsr #2***********
    push{lr}
	bl COMPUTE_PLAYER_DELAY
    pop{lr}
	mov r3, r5 ;store value r5 from COMPUTE_PLAYER_DELAY into R3 for timer
    push{lr}
	bl TIMER
    pop{lr}
	bx lr
	
COMPUTE_PLAYER_DELAY
    push{r2}
	;r3 is the current switch value
	;r9 is current draw number
    mov r2, #0x50; 80
	mul r3, r2 ;p.sn*80
	mov r5, #0x140 ;320
	sub r5, r3 ;r5=320 - 80*p.sn
	
	mov r3, #0x4
	cmp r3, r9 ;if(4>=draws)
	ble continue1 ;else r3=4
	mov r3, r9 ; if(4>=draws) r3=num draws
continue1
	lsr r5, r3	;left shift r5 by min(4,draws)
	
    mov32 r2, #0x13880 ;80,000
    mul r5, r2 ;value of r5 * 80,000 to get time in ms
    pop{r2}
	bx lr ;END COMPUTE_PLAYER_DELAY, return value in r5

   ALIGN      
   END  
           