#include "lcd.h"
#include "Shapes.h"
#include "Battleship.h"
#include <stdbool.h>

#define RCGC2_PA 0x00000001
#define RCGC2_PB 0x00000002
#define RCGC2_PC 0x00000004
#define UNLOCK 0x4c4f434b
#define MAXTOUCH 50

void run(void);
void toggleBtn( BtnData *btn);
void initPorts(void);
void initSSI(void);
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
	WTIMER0->TAMR = 0x1; //One-Shot
	WTIMER0->CTL |= 0x1; //enable timer
	
}


void initBtn(void){
	btn1.x_begin = 40; btn1.x_end = 200; btn1.y_begin = 20; 
	btn1.y_end = 100;	btn1.color = red; btn1.on = true;
	
	btn2.x_begin = 40; btn2.x_end = 200; btn2.y_begin = 120; 
	btn2.y_end = 200;	btn2.color = yellow; btn2.on = true;
	
	btn3.x_begin = 40; btn3.x_end = 200; btn3.y_begin = 220; 
	btn3.y_end = 300;	btn3.color = green; btn3.on = true;
	
	draw_rectangle(btn1,0);
	draw_rectangle(btn2,0);
	draw_rectangle(btn3,0);
	
	doneTouch = false;
}

void initPorts(void){
	
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
		while((GPIOB->DATA &~0x8) >> 3 == 0){ //wait until touch is done
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

void initSSI(void){
	
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



void computeTouch(unsigned int xval, unsigned int yval){
	if(xval > 750 && xval < 3250){
		if(yval > 400 && yval < 1400){
			toggleBtn(&btn1);
		}
		else if(yval > 1400 && yval < 2500){
			toggleBtn(&btn2);
		}
		else if(yval > 2500 && yval < 3550){
			toggleBtn(&btn3);
		}
	}
}

void toggleBtn( BtnData *btn){
	if(btn->on){ //if button is currently on, only show border of button, want to turn off
		BtnData temp = *btn;
		temp.x_begin += 10; temp.x_end -= 10; temp.y_begin += 10; 
		temp.y_end -= 10;	temp.color = black;
		btn->on=false;
		draw_rectangle(temp,0);
	}else{ //if button is currently off, turn on, fill in
		draw_rectangle(*btn,0);
		btn->on = true;
	}
}

void run(void){
	while(1){
		while(!doneTouch); //wait until a touch has started
		doneTouch= false;
		//computeTouch(xval, yval);
	}
	//toggleBtn(&btn1);
	//toggleBtn(&btn1);
}

int main(void)
{
	//values for configuration. Assign these to the values you chose for your peripherals.
	//
	//ssiModuleBaseAddress should be assigned to the base address of your SSI/SPI module. 
	//cmdDataSelectGPIOBaseAddress should be assigned to the base address of the GPIO port you are using as your command/data Select. ***PB0***
	//cmdDataSelectGPIOPinNumber should be assigned the the pin number (0-7) of the GPIO pin you are using as your command/data select pin. 
	//
//	unsigned int* ssiModuleBaseAddress             = (unsigned int*) 0x40008000;    //default value for ssi0
//	unsigned char* cmdDataSelectGPIOBaseAddress    = (unsigned char*) 0x40005000;   //default value for GPIO Port B
//	unsigned int cmdDataSelectGPIOPinNumber        = 0x0;                           //default value for PB_0
	
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//Call your initialization functions here
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	current = touchItems = p1xval = p1yval = p2xval = p2yval= 0;
	
	initPorts();
	initSSI();
	lcd_init();
	
	start_game();
	
	//clear_lcd(black,0);
	//initBtn();
	//run();
	//This function tests the configuration of your GPIO port and your SSI/SPI module. 
	//
	//Pass it the following parameters in the following order
	//	The pin number (0-7) of the gpio port you are using. Pass as an unsigned integer.
	//	The base address for the gpio port you are using. Pass as an unsigned character pointer
	//	The base address for the SSI/SPI module you are using. Pass as an unsigned integer pointer. 
	//test_configuration(cmdDataSelectGPIOPinNumber, cmdDataSelectGPIOBaseAddress, ssiModuleBaseAddress);
	
	

	
	
	
	//After you have made sure that your ssi configuration and your gpio configuration are correct, 
	// you can delete the call to test_configuration and write your own code to draw the buttons. 
	// It is recommended that you write your LCD screen interfacing code in lcd.c and provide 
	// corresponding function declarations in lcd.h. That will allow you to copy your lcd.c and lcd.h 
	// files into any other project that requires the LCD screen. 
	while(1);
}
