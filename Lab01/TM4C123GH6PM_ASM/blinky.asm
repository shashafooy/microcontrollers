       THUMB
       AREA    |.text|, CODE, READONLY, ALIGN=2
       EXPORT  Start


GPIO_CLK	DCD 0x400FE018	;address for GPIO Clock
CLKVAL		DCD 0x0020	;value for enabling gpios clock 
UNLOCK		DCD 0x4C4F434B ; gpio unlock code. 
GPIOF		DCD 0x40025000  ;base address for one of the GPIOs
GPIOF_PINS	DCD 0xE	;value for configuring the gpio
			;LIST the values toggled by the setup value.
			;i.e. gpio enable, pull up register, etc.  
		ALIGN

Start
	mov32 R0, #0x400FE108 ; Enable GPIO Clock
	mov R1, #0x20
	str R1, [R0]
	
	mov32 R0, #0x40025000 ;GPIOF address
	
	;unlock GPIOF
	mov32 R1, #0x4C4F434B; GPIO Unlock code. 
	str R1, [R0,#0x520];
	
	mov R1, #0x1F
	str R1, [R0,#0x524];GPIOCR
	mov R1, #0x11
	str R1, [R0,#0x510]
	mov R1, #0x0F
	str R1, [R0,#0x400] ;GPIODIR
	mov R1, #0x1F
	str R1, [R0,#0x51C] ;digital enable
loop
	MOV32 R1, #0x02	;load value for turning on LED color Red
	STR R1, [R0,#0x38]	;write the above value to GPIOF ODR register.

	MOV R4, #0			;initial value for iteration loop
	MOV32 R5, #0x7FFFF	;number of iterations for delay loops
delay1
	ADD R4, #1			; add 1 to register 4
	CMP R4, R5			;check number of iterations
	BLE delay1			;continue if iterated less than 0xFFFFF + 1 times, otherwise repeat delay loop
	
	MOV32 R1, #0x04	;load value for turning on LED color blue
	STR R1, [R0, #0x38]	;write the above value to GPIOF ODR
	
	MOV R4, #0		;initial value for iteration loop
	;			; **** the tm4c123gh6pm has 16 MHz clock.
	;; 			 how long should the loop take with that clock?
	
delay2
	ADD R4, #1			;add 1 to register 4
	CMP R4, R5			;check number of iterations
	BLE delay2			;continue if iterated less than 0xFFFFF + 1 times, otherwise repeat delay loop

	MOV32 R1, #0x08	;load value for turning on LED Color Green
	STR R1, [R0, #0x38]	;write the above value to GPIOF ODR
	
	B loop	;do it all over again, forever
	ALIGN
	END
