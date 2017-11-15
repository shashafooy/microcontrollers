#include "Init.h"
#include "REG.h"

#include "RTE_Components.h"             // Component selection


unsigned char *CORE_INIT = (unsigned char *) 0xE000E000;



void ADCInit(void){
	//set sysclk to 80MHz
	SYSCTL->RCC2 |= 0x1 << 31; //use RCC2	
	SYSCTL->RCC2 |= 0x1 << 11; //use bypass pll
	SYSCTL->RCC = (SYSCTL->RCC & ~0x7c0) + 0x540; //set XTAL to 0x15 16MHz
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
	NVIC->ICPR[0x0/4] |= ADC0SS0_IRQn; //use ADC0 sequencer0 interrupt

	/*Wide Timer 0 init*/
	SYSCTL->RCGCWTIMER = 0x1; //use GPTimer 0
	WTIMER0->CTL = 0x0; //disable timer
	WTIMER0->CTL |= 0x1 << 5; //enable trigger for ADC
	WTIMER0->CFG = 0x4; //32 bit wide
	WTIMER0->TAMR = 0x2; //periodic
	WTIMER0->TAILR = 0x270ff; // (load+1)/80MHz=0.002
	WTIMER0->CTL |= 0x1; //enable timer
}

void I2CInit(void){
	
	SYSCTL->RCGCI2C = 0x1;
	SYSCTL->RCGCGPIO |= 0x2;
	GPIOB->AFSEL = 0xC;
	GPIOB->ODR = 0xC; //i2c open drain
	GPIOB->PCTL = 0x3300; //AF i2c
	I2C0->MCR = 0x10; //initialize master
	I2C0->MCR = 0x3; //TPR = (system clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1 TPR = 3;
	
	
	
}
