#include "Init.h"


void GPIOInit(void){
	
	
	
}

void ADCInit(void){
	//module enable
	SYSCTL->DC1 = 0x13331FFF; //set MINSYSDIV to 80MHZ
	SYSCTL->RCC = 0x004E10D1; //use PLL clk at 16MHz
	SYSCTL->RCC2 = 
	//RCC2 for 80MHz?
	SYSCTL->RCGCADC = 0x1;
	SYSCTL->RCGCGPIO |= 0x10; //set clock
	GPIOE->AFSEL = 0xC;
	GPIOE->DEN = 0x0;
	GPIOE->AMSEL = 0xC;
	
	//sample sequencer init
	ADC0->ACTSS = 0x0;
	//*****Continue on step 2*******
	

}

void I2CInit(void){
	
	SYSCTL->RCGCI2C = 0x1;
	SYSCTL->RCGCGPIO |= 0x2;
	GPIOB->AFSEL = 0xC;
	GPIOB->ODR = 0xC; //i2c open drain
	GPIOB->PCTL = 0x3300; //AF i2c
	I2C0->MCR = 0x3; //TPR = (system clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1 TPR = 3;
	
	
	
}
