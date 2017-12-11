#include "lcd.h"
#include "Battleship.h"
#include <stdbool.h>

#define RCGC2_PA 0x00000001
#define RCGC2_PB 0x00000002
#define RCGC2_PC 0x00000004
#define UNLOCK 0x4c4f434b
#define MAXTOUCH 500

void toggleBtn( BtnData *btn);
void init_Ports(void);
void init_SSI(void);
void computeTouch(unsigned int, unsigned int);

BtnData btn1, btn2, btn3;
int i, tail;
int doneTouch;
unsigned int xcord[MAXTOUCH], ycord[MAXTOUCH];
unsigned int current, touchItems;
int p1xval, p1yval, p2xval, p2yval;

void init_SysClk() {
	
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
}

void init_WTimer(){
	
	SYSCTL->RCGCWTIMER = 0x1; //use GPWTimer 0
	
	//Wtimer0 for delay() function
	WTIMER0->CTL = 0x0; //disable timer
	WTIMER0->CFG = 0x4; //32 bit wide
	WTIMER0->TAMR = 0x2; //Periodic
	
}

void init_Ports(void){
	
	SYSCTL->RCGCSSI = 0x1; //RCGCSSI enable SSI0 clock
	SYSCTL->RCGC2 |= RCGC2_PA | RCGC2_PB | RCGC2_PC;
	
	GPIOA->AFSEL = 0x3C; //enable alt function for pins 2-5
	GPIOA->PCTL = 0x222200; //SSI function pin 2-5
	GPIOA->DEN = 0x3C; //Digital enable pin 2-5
	
	GPIOB->AFSEL = 0x00; //disable AF
	GPIOB->DIR = 0xF1; //pin 0,4,5,6,7 output, pin 2,3 input
	GPIOB->DEN = 0xFD; //enable pin 0,2,3,4,5,6,7
	
	GPIOB->DATA |= 0xF0; //pin 4,5,6,7 high 
	
	GPIOC->AFSEL = 0x00; //disable at functions for pc
	GPIOC->DIR = 0x70; //pin 4,5,6 output
	GPIOC->DEN = 0x70; //enable pins 4,5,6
	
	NVIC_EnableIRQ(GPIOB_IRQn); //port B interrupt
	GPIOB->IS= 0x0; //interrupt on edge
	GPIOB->IBE = 0x0; //disable both edges
	GPIOB->IEV = 0x0; //falling edge
	GPIOB->IM = 0xC; //enable interrupt for pin 2,3
}


void GPIOB_Handler(void){
	current = touchItems = 0;
	p1xval = p1yval = p2xval = p2yval= 0;
	if(GPIOB->MIS & 0x4){				//LCD0 interrupt
		GPIOB->IM = 0x0; //mask pin 2,3
		while((GPIOB->DATA &0x4) >> 2 == 0){ //wait until touch is done
			xcord[current] = get_touch_x(0);
			ycord[current] = get_touch_y(0);
			current++;
			current %= MAXTOUCH;
			if(touchItems<MAXTOUCH) touchItems++;
		}
		for(i = 0; i<touchItems; i++){
			p1xval += xcord[i];
			p1yval += ycord[i];
		}
		p1xval /= touchItems;
		p1yval /= touchItems;
		GPIOB->ICR = 0x4; //acknowlege pin 2 interrupt
	}else{										//LCD1 Interrupt
		GPIOB->IM = 0x0; //mask pin 2,3
		while((GPIOB->DATA & 0x8) >> 3 == 0){ //wait until touch is done
			xcord[current] = get_touch_x(1);
			ycord[current] = get_touch_y(1);
			current++;
			current %= MAXTOUCH;
			if(touchItems<MAXTOUCH) touchItems++;
		}
		for(i = 0; i<touchItems; i++){
			p2xval += xcord[i];
			p2yval += ycord[i];
		}
		p2xval /= touchItems;
		p2yval /= touchItems;
		GPIOB->ICR = 0x8; //acknowlege pin 3 interrupt	
	}
	doneTouch = true;
	GPIOB->IM |= 0xC; //endable pin 2,3
}

void init_SSI(void){
	
	SSI0->CR1 &= ~0x2; //disable SSIO, SSICR1 disable SSE bit (2nd)
	SSI0->CR1 = 0x00000000; //master mode
	SSI0->CC = 0x0; //use sysclock clock
	SSI0->CPSR = 0x2; //set the prescale value to 2 CPSDVSR
	//SSIClk = SysClk / (CPSDVSR * (1 + SCR))
	//5.7Mhz = 80MHz/ (CPSDVSR *(1+SCR))   CPSDVSR= 2
	//6.35MHz max SSIClk=5.7MHz  CPSDVSR=2  SCR=6
	//SSI0->CPSR = 0x4;
	SSI0->CR0 = 0x6C7; //SCR = 6, SPH = SPO = 1, DSS= 8 bits
	SSI0->CR1 |= 0x2; //enable SSIO
}

int main(void)
{
	current = touchItems = p1xval = p1yval = p2xval = p2yval= 0;
	
	init_SysClk();
	init_WTimer();
	init_Ports();
	init_SSI();
	lcd_init();
	draw_rectangle(BtnData_new(0,240,0,4,white),0); //warm up the lcd
	draw_rectangle(BtnData_new(0,240,0,4,white),1); //these functions are needed to "warm up?" the lcd

	run();

	while(1);
}
