            	AREA	main, CODE, READONLY, ALIGN=2
            	THUMB
            	EXPORT Start


message     	DCB     "Hello Students!",0     	; message stored readonly
 
            	ALIGN ;pg149
Start       	LDR 	R0, =message            	; load address of the message
            	
            	MOV 	R1, SP		            	; load memory location to store
           	
load        	LDR 	R2,[R0]                 	; load a word of the message
            	ADD 	R0, R0, #4					; adds 4 to the pointer  
                MOV 	R3, #4 				       	; used as a counter
 
            	SUB 	R3, R3, #1              	; decrements counter
            	STR 	R2,[R1,#4]             		; store the word to memory, inc R1 by 4
            	
            	CMP 	R3, #0						; Check for null
            	BNE 	load                    	; repeat if not null terminated
            	
loop        	B   	loop
 
            	ALIGN
            	END