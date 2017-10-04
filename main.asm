; main.s
; Runs on any Cortex M processor
; A very simple first project implementing a random number generator
; Daniel Valvano
; May 4, 2012

;  This example accompanies the book
;  "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
;  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2012
;  Section 3.3.10, Program 3.12
;
;Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
;   You may use, edit, run or distribute this file
;   as long as the above copyright notice remains
;THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
;OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
;MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
;VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
;OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;For more information about my classes, my research, and my books, see
;http://users.ece.utexas.edu/~valvano/


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
	mov32 r0, port_E
	ldr R1, [R0, #0x30] ;get current buttons pressed
	eor r1, #0xc
	orr r2, r1 ;refresh ready states of players
	cmp r2, #0x0c ;p1 and p2 are ready, might be #0xc
	beq START_GAME ;start game
	
	mov32 r0, port_B
	ldr r1, [r0, #0xc0] ;load current values of led 4,5
	mvn r1, r1
	mvn r3, r2
	and r1, r3, lsl #2
	str r1, [r0, #0xc0] ;set led 4,5 to inverted value
	

	mov32 r3, #0x2625a00 ; 0.5 Seconds
	bl TIMER
;	mov32 r0, sysTick
;	mov r1, #4
;	str r1, [r0, #0x10] ;stop sysTick, use system clock
;	mov32 r1, #0x7a1200 ;7a1200
;	str r1, [r0, #0x14] ;set count value
;	ldr r1, [r0, #0x10]
;	orr r1, #0x1
;	str r1, [r0, #0x10] ;enable clock
;	mov r3, #0x1
	mov32 r0, sysTick
BLINK_DELAY
	ldr r1, [r0, #0x10]
	cmp r3, r1, lsr #16
	bne BLINK_DELAY
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
	mov32 r3, #0x4c4b400 ;*****TODO make random 1-2, currently 1 sec**********
	bx lr
	
START_GAME
	mov32 r0, port_B
	mov r1, #0xcf ; cf=1100 1111, turn on center led
	str r1, [r0, #0xc0] ;load current values of led 4,5
	
Loop
	bl RND12 ;get random value 1-2 and store in r3
	bl TIMER ;use value of r3 from rnd12
WHILE2
	mov32 r0, port_E
	ldr R1, [R0, #0x30] ;get current buttons pressed
	eor r1, #0xc
	cmp r1, #0x0000
	bne Loop ;player pressed button early
	
	mov32 r0, sysTick
	ldr r1, [r0, #0x10] 
	lsr r1, #16
	cmp r3, r1 ;is timer done
	bne WHILE2
	
	lsl r11, #1 ;move p1 left 1
	lsr r12, #1 ;move p2 right 1
	bl UPDATE_LED
	
GET_FIRST_TURN	
	mov32 r0, port_E
	ldr R1, [R0, #0x30] ;get current buttons pressed
	eor r1, #0xc
	cmp r1, #0x8
	bne CHECK_P2
	bl PLAYER1_FIRST
CHECK_P2
	cmp r1, #0x4
	bne GET_FIRST_TURN
	bl PLAYER2_FIRST
	
	
	
	
	
	
PLAYER1_FIRST
	push{r0}
	push{r1}
	
	bx lr

PLAYER2_FIRST
	bx lr
;********************OLD CODE*********************	
delay
	ldr R1, [R7, #0x3FC]
	mvn R1, R1
	AND R1, #0x10
	cmp R1, #0x10
	beq stop
start
	ldr R1, [R2, #0x3FC]
	and R1, #0x04
	cmp R1, #0x04
	beq reset
	
	sub R5, #0x1
	cmp R5, #0x0
	bgt delay
	
	
	add R3, #0x1
	mov32 R6, #0x3FF
	cmp R3, R6
	beq reset
	b Loop

stop
	ldr R1, [R2, #0x3FC]
	and R1, #0x04
	cmp R1, #0x04
	beq reset
	ldr R1, [R7, #0x3FC]
	mvn R1, R1
	AND R1, #0x01
	cmp R1, #0x01
	beq start
	b stop


reset
	mov R3, #0x000
	b Loop

   ALIGN      
   END  
           