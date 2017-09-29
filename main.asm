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
	mov r2, #0 ;ready states of p1[0] and p2[1]
	mov32 r0, port_B
	mov r1, #0x30
	str r1, [r0, #0x3fc] ;turns on center leds 

standby
	mov32 r0, port_E
	ldr R1, [R0, #0x30]
	eor r1, #0xc
	orr r2, r1 ;refresh ready states of players
	cmp r2, #0x0c ;p1 and p2 are ready, might be #0xc
	beq Loop ;start game
	
	mov32 r0, port_B
	ldr r1, [r0, #0xc0] ;load current values of led 4,5
	mvn r1, r1
	str r1, [r0, #0xc0] ;set led 4,5 to inverted value
	
	mov32 r0, sysTick
	mov r1, #4
	str r1, [r0, #0x10] ;stop sysTick, use system clock
	mov32 r1, #0x7a1200 ;7a1200
	str r1, [r0, #0x14] ;set count value
	ldr r1, [r0, #0x10]
	orr r1, #0x1
	str r1, [r0, #0x10] ;enable clock
	mov r3, #0x1
wait1
	ldr r1, [r0, #0x10]
	cmp r3, r1, lsr #16
	bne wait1
	b standby
;******start main loop******	
Loop
	mov32 R5, #0x6cd06 ;delay counter
	mvn R3, R3
	
	str R3, [R0, #0x3FC]
	lsr R4, R3, #8
	str R4, [R2, #0x3FC]
	
	mvn R3, R3
	
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
           