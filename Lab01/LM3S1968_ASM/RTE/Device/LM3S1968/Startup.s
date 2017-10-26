; <<< Use Configuration Wizard in Context Menu >>>
;******************************************************************************
;
; Startup.s - Startup code for Stellaris.
;
; Copyright (c) 2011 Texas Instruments Incorporated.  All rights reserved.
; Software License Agreement
; 
; Texas Instruments (TI) is supplying this software for use solely and
; exclusively on TI's microcontroller products. The software is owned by
; TI and/or its suppliers, and is protected under applicable copyright
; laws. You may not combine this software with "viral" open-source
; software in order to form a larger program.
; 
; THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
; NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
; NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
; CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
; DAMAGES, FOR ANY REASON WHATSOEVER.
; 
; This is part of revision 7860 of the Stellaris Peripheral Driver Library.
;
;******************************************************************************

;******************************************************************************
;
; <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;
;******************************************************************************
Stack   EQU     0x00000400

;******************************************************************************
;
; <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;
;******************************************************************************
Heap    EQU     0x00000000

;******************************************************************************
;
; Allocate space for the stack.
;
;******************************************************************************
        AREA    STACK, NOINIT, READWRITE, ALIGN=3
StackMem
        SPACE   Stack
__initial_sp

;******************************************************************************
;
; Allocate space for the heap.
;
;******************************************************************************
        AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
HeapMem
        SPACE   Heap
__heap_limit

;******************************************************************************
;
; Indicate that the code in this file preserves 8-byte alignment of the stack.
;
;******************************************************************************
        PRESERVE8

;******************************************************************************
;
; Place code into the reset code section.
;
;******************************************************************************
        AREA    RESET, CODE, READONLY
        THUMB

;******************************************************************************
;
; The vector table.
;
;******************************************************************************
        EXPORT  __Vectors
__Vectors
        DCD     StackMem + Stack            ; Top of Stack
        DCD     Reset_Handler               ; Reset Handler
        DCD     NMI_Handler                 ; NMI Handler
        DCD     HardFault_Handler           ; Hard Fault Handler
        DCD     MemManage_Handler           ; MPU Fault Handler
        DCD     BusFault_Handler            ; Bus Fault Handler
        DCD     UsageFault_Handler          ; Usage Fault Handler
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     DebugMon_Handler            ; Debug Monitor Handler
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; PendSV Handler
        DCD     SysTick_Handler             ; SysTick Handler
        DCD     GPIOPortA_Handler           ; GPIO Port A
        DCD     GPIOPortB_Handler           ; GPIO Port B
        DCD     GPIOPortC_Handler           ; GPIO Port C
        DCD     GPIOPortD_Handler           ; GPIO Port D
        DCD     GPIOPortE_Handler           ; GPIO Port E
        DCD     UART0_Handler               ; UART0
        DCD     UART1_Handler               ; UART1
        DCD     SSI0_Handler                ; SSI
        DCD     I2C0_Handler                ; I2C
        DCD     PWMFault_Handler            ; PWM Fault
        DCD     PWM0_Handler                ; PWM Generator 0
        DCD     PWM1_Handler                ; PWM Generator 1
        DCD     PWM2_Handler                ; PWM Generator 2
        DCD     Quadrature0_Handler         ; Quadrature Encoder
        DCD     ADC0_Handler                ; ADC Sequence 0
        DCD     ADC1_Handler                ; ADC Sequence 1
        DCD     ADC2_Handler                ; ADC Sequence 2
        DCD     ADC3_Handler                ; ADC Sequence 3
        DCD     WDT_Handler                 ; Watchdog
        DCD     Timer0A_Handler             ; Timer 0A
        DCD     Timer0B_Handler             ; Timer 0B
        DCD     Timer1A_Handler             ; Timer 1A
        DCD     Timer1B_Handler             ; Timer 1B
        DCD     Timer2A_Handler             ; Timer 2A
        DCD     Timer2B_Handler             ; Timer 2B
        DCD     Comp0_Handler               ; Comp 0
        DCD     Comp1_Handler               ; Comp 1
        DCD     Comp2_Handler               ; Comp 2
        DCD     SysCtl_Handler              ; System Control
        DCD     FlashCtl_Handler            ; Flash Control
        DCD     GPIOPortF_Handler           ; GPIO Port F
        DCD     GPIOPortG_Handler           ; GPIO Port G
        DCD     GPIOPortH_Handler           ; GPIO Port H
        DCD     UART2_Handler               ; UART2 Rx and Tx
        DCD     SSI1_Handler                ; SSI1 Rx and Tx
        DCD     Timer3A_Handler             ; Timer 3 subtimer A
        DCD     Timer3B_Handler             ; Timer 3 subtimer B
        DCD     I2C1_Handler                ; I2C1 Master and Slave
        DCD     Quadrature1_Handler         ; Quadrature Encoder 1
        DCD     CAN0_Handler                ; CAN0
        DCD     CAN1_Handler                ; CAN1
        DCD     CAN2_Handler                ; CAN2
        DCD     Ethernet_Handler            ; Ethernet
        DCD     Hibernate_Handler           ; Hibernate
        DCD     USB0_Handler                ; USB0
        DCD     PWM3_Handler                ; PWM Generator 3
        DCD     uDMA_Handler                ; uDMA Software Transfer
        DCD     uDMA_Error                  ; uDMA Error

;******************************************************************************
;
; This is the code that gets called when the processor first starts execution
; following a reset event.
;
;******************************************************************************
        EXPORT  Reset_Handler
Reset_Handler
        ;
        ; Call the 'Start' routine
        ;
        IMPORT  Start
        B       Start

;******************************************************************************
;
; This is the code that gets called when the processor receives a NMI.  This
; simply enters an infinite loop, preserving the system state for examination
; by a debugger.
;
;******************************************************************************
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP

;******************************************************************************
;
; This is the code that gets called when the processor receives a fault
; interrupt.  This simply enters an infinite loop, preserving the system state
; for examination by a debugger.
;
;******************************************************************************
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP

;******************************************************************************
;
; This is the code that gets called when the processor receives an unexpected
; interrupt.  This simply enters an infinite loop, preserving the system state
; for examination by a debugger.
;
;******************************************************************************
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP
IntDefaultHandler\
                PROC

                EXPORT  GPIOPortA_Handler         [WEAK]
                EXPORT  GPIOPortB_Handler         [WEAK]
                EXPORT  GPIOPortC_Handler         [WEAK]
                EXPORT  GPIOPortD_Handler         [WEAK]
                EXPORT  GPIOPortE_Handler         [WEAK]
                EXPORT  UART0_Handler             [WEAK]
                EXPORT  UART1_Handler             [WEAK]
                EXPORT  SSI0_Handler              [WEAK]
                EXPORT  I2C0_Handler              [WEAK]
                EXPORT  PWMFault_Handler          [WEAK]
                EXPORT  PWM0_Handler              [WEAK]
                EXPORT  PWM1_Handler              [WEAK]
                EXPORT  PWM2_Handler              [WEAK]
                EXPORT  Quadrature0_Handler       [WEAK]
                EXPORT  ADC0_Handler              [WEAK]
                EXPORT  ADC1_Handler              [WEAK]
                EXPORT  ADC2_Handler              [WEAK]
                EXPORT  ADC3_Handler              [WEAK]
                EXPORT  WDT_Handler               [WEAK]
                EXPORT  Timer0A_Handler           [WEAK]
                EXPORT  Timer0B_Handler           [WEAK]
                EXPORT  Timer1A_Handler           [WEAK]
                EXPORT  Timer1B_Handler           [WEAK]
                EXPORT  Timer2A_Handler           [WEAK]
                EXPORT  Timer2B_Handler           [WEAK]
                EXPORT  Comp0_Handler             [WEAK]
                EXPORT  Comp1_Handler             [WEAK]
                EXPORT  Comp2_Handler             [WEAK]
                EXPORT  SysCtl_Handler            [WEAK]
                EXPORT  FlashCtl_Handler          [WEAK]
                EXPORT  GPIOPortF_Handler         [WEAK]
                EXPORT  GPIOPortG_Handler         [WEAK]
                EXPORT  GPIOPortH_Handler         [WEAK]
                EXPORT  UART2_Handler             [WEAK]
                EXPORT  SSI1_Handler              [WEAK]
                EXPORT  Timer3A_Handler           [WEAK]
                EXPORT  Timer3B_Handler           [WEAK]
                EXPORT  I2C1_Handler              [WEAK]
                EXPORT  Quadrature1_Handler       [WEAK]
                EXPORT  CAN0_Handler              [WEAK]
                EXPORT  CAN1_Handler              [WEAK]
                EXPORT  CAN2_Handler              [WEAK]
                EXPORT  Ethernet_Handler          [WEAK]
                EXPORT  Hibernate_Handler         [WEAK]
                EXPORT  USB0_Handler              [WEAK]
                EXPORT  PWM3_Handler              [WEAK]
                EXPORT  uDMA_Handler              [WEAK]
                EXPORT  uDMA_Error                [WEAK]

GPIOPortA_Handler
GPIOPortB_Handler
GPIOPortC_Handler
GPIOPortD_Handler
GPIOPortE_Handler
UART0_Handler
UART1_Handler
SSI0_Handler
I2C0_Handler
PWMFault_Handler
PWM0_Handler
PWM1_Handler
PWM2_Handler
Quadrature0_Handler
ADC0_Handler
ADC1_Handler
ADC2_Handler
ADC3_Handler
WDT_Handler
Timer0A_Handler
Timer0B_Handler
Timer1A_Handler
Timer1B_Handler
Timer2A_Handler
Timer2B_Handler
Comp0_Handler
Comp1_Handler
Comp2_Handler
SysCtl_Handler
FlashCtl_Handler
GPIOPortF_Handler
GPIOPortG_Handler
GPIOPortH_Handler
UART2_Handler
SSI1_Handler
Timer3A_Handler
Timer3B_Handler
I2C1_Handler
Quadrature1_Handler
CAN0_Handler
CAN1_Handler
CAN2_Handler
Ethernet_Handler
Hibernate_Handler
USB0_Handler
PWM3_Handler
uDMA_Handler
uDMA_Error
                B       .

                ENDP

;******************************************************************************
;
; Make sure the end of this section is aligned.
;
;******************************************************************************
        ALIGN

;******************************************************************************
;
; Tell the assembler that we're done.
;
;******************************************************************************
        END
