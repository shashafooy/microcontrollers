#include "Init.h"
#include "REG.h"
unsigned int *CORE_INIT = (unsigned int *) 0xE000E000;

void ADCInit(void){
	//set sysclk to 80MHz
	SYSCTL->RCC = (SYSCTL->RCC & ~0x7c0) + 0x540; //set XTAL to 0x15 16MHz
	SYSCTL->RCC2 |= 0x80000000; //use RCC2
	SYSCTL->RCC2 |= 0x1 << 11; //use bypass pll
	SYSCTL->RCC2 &= ~0x70; //clear OSCSRC2, use main oscillator
	SYSCTL->RCC2 &= ~0x2000; //clear	PWRDN2
	SYSCTL->RCC2 |= 0x40000000; //DIV400 = 1
	SYSCTL->RCC2 = (SYSCTL->RCC2 & ~0x1fc00000) + (0x4<<22);
	while((SYSCTL->RIS & 0x40) == 0); //wait for PLLRIS bit
	SYSCTL->RCC2 &= ~(0x1 << 11); //clear bypass pll
	

	//GPIOE P3 init
	SYSCTL->RCGCADC = 0x1;
	SYSCTL->RCGCGPIO |= 0x10; //set clock
	GPIOE->AFSEL = 0x8; // AIN0 PE3
	GPIOE->DEN = 0x0;
	GPIOE->AMSEL = 0x8; //analog enable
	
	//ADC init
	ADC0->ACTSS = 0x0; //disable ADC0
	ADC0->EMUX = 0x5; //trigger with timer
	ADC0->SSMUX0 = 0x0; //use sequencer0
	ADC0->SSCTL0 = 0x6; //0b1110, set interrupt for sample sequence
	ADC0->IM = 0x1; //enable interrupt for sequencer0
	ADC0->ACTSS = 0x1; //enable ADC0   use
	NVIC_EnableIRQ(ADC0SS0_IRQn);
	CORE_INIT[0x40c/4] = 0x0 << 21; //set intpriority to 3
	
	
}

void timerInit(void){
	SYSCTL->RCGCWTIMER = 0x7; //use GPWTimer 0,1,2
	//Wtimer0 for adc
	WTIMER0->CTL = 0x0; //disable timer
	WTIMER0->CTL |= 0x1 << 5; //enable trigger for ADC
	WTIMER0->CFG = 0x4; //32 bit wide
	WTIMER0->TAMR = 0x2; //periodic
	WTIMER0->TAILR = 0x270ff; // (load+1)/80MHz=0.002
	WTIMER0->CTL |= 0x1; //enable timer
	
	//WTimer1 to update DAC
	WTIMER1->CTL = 0x0;
	WTIMER1->IMR |= 0x1; //enable interrupt
	NVIC_EnableIRQ(WTIMER1A_IRQn);
	CORE_INIT[0x960/4] |= (0x2 << 5) + (0x1 << 13); //interrupt priority for timers 1(p2) and 2(p1)
	WTIMER1->CFG = 0x4; //32bit wide
	WTIMER1->TAMR = 0x2; //periodic
	WTIMER1->TAILR = 0x26259FF; //(load+1)/80MHz=0.5
	WTIMER1->CTL |= 0x1; //enable timer
	
	//WTimer2 for frequency
	WTIMER2->CTL = 0x0; //disable
	WTIMER2->IMR |= 0x1; //enable interrupt
	NVIC_EnableIRQ(WTIMER2A_IRQn);
	WTIMER2->CFG = 0x4; //32bit wide
	WTIMER2->TAMR = 0x2; //periodic
}

void I2CInit(void){
	//PB2 clk PB3 data
	SYSCTL->RCGCI2C = 0x1;
	SYSCTL->RCGCGPIO |= 0x2;
	GPIOB->AFSEL = 0xC; //enable alt func
	GPIOB->ODR = 0x8; //i2c open drain, only set data line
	GPIOB->PCTL = 0x3300; //AF i2c
	GPIOB->DEN = 0xC;
	I2C0->MCR = 0x10; //initialize master
	I2C0->MTPR = 0x1; //TPR = (system clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1 TPR = 1;
}
