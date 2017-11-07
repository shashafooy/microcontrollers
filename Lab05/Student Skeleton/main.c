#include "lcd.h"
#include "lcd_configuration_test.h" //You can remove this line after you've removed the call to test_configuartion(unsigned int, unsigned char*, unsigned int*)
#include "Shapes.h"
#include <stdbool.h>



unsigned int *PA = (unsigned int *) 0x40004000;
unsigned int *PB = (unsigned int *) 0x40005000;
unsigned int *SSI0 = (unsigned int *) 0x40008000;
unsigned int *SYSCTL = (unsigned int *) 0x400FE000;
unsigned int *CORE = (unsigned int *) 0xE000E000;

#define RCGC2_PA 0x00000001
#define RCGC2_PB 0x00000002
#define UNLOCK 0x4c4f434b

void run(void);
void toggleBtn(struct BtnData *btn);
void initPorts(void);
void initSSI(void);
void computeTouch(unsigned int, unsigned int);



struct BtnData btn1, btn2, btn3;
int i, current, touchItems;
bool startTouch;
unsigned int xcord[1000], ycord[1000];


void initBtn(void){
	btn1.x_begin = 40; btn1.x_end = 200; btn1.y_begin = 20; 
	btn1.y_end = 100;	btn1.color = red; btn1.on = true;
	
	btn2.x_begin = 40; btn2.x_end = 200; btn2.y_begin = 120; 
	btn2.y_end = 200;	btn2.color = yellow; btn2.on = true;
	
	btn3.x_begin = 40; btn3.x_end = 200; btn3.y_begin = 220; 
	btn3.y_end = 300;	btn3.color = green; btn3.on = true;
	
	draw_rectangle(btn1);
	draw_rectangle(btn2);
	draw_rectangle(btn3);
	
	startTouch = false;
}

void toggleBtn(struct BtnData *btn){
	if(btn->on){ //if button is currently on, only show border of button
		struct BtnData temp = *btn;
		temp.x_begin += 10; temp.x_end -= 10; temp.y_begin += 10; 
		temp.y_end -= 10;	temp.color = black;
		
		btn->on=false;
		draw_rectangle(temp);
	}else{ //if button is currently off, fill in
		draw_rectangle(*btn);
		btn->on = true;
	}
	
	//******toggle led too *******
}



void initPorts(void){
	
	SYSCTL[0x61C/4] = 0x1; //RCGCSSI enable SSI0 clock
	SYSCTL[0x108/4] |= RCGC2_PA | RCGC2_PB;
	
	PA[0x420/4] = 0x3C; //enable alt function for pins 2-5
	PA[0x52c/4] = 0x222200; //SSI function pin 2-5
	PA[0x51c/4] = 0x3C; //Digital enable pin 2-5
	
	PB[0x420/4] = 0x00; //disable AF
	PB[0x400/4] = 0x61; //pin 0,5,6 output, pin 2 input
	PB[0x51c/4] = 0x65; //enable pin 0,2,5,6
	
	PB[0x180/4] = 0x60; //pin 5,6 high 
	
	CORE[0x100/4] = 0x2; //enable interrupt for port B
	PB[0x404/4] = 0x0; //interrupt on edge
	PB[0x408/4] = 0x0; //disable both edges
	PB[0x40c/4] = 0x0; //falling edge
	PB[0x410/4] = 0x4; //enable interrupt for pin 2
}


void GPIOB_Handler(void){
	PB[0x410/4] = 0x0; //mask pin 2
	PB[0x41c/4] = 0x4; //acknowlege pin 2 interrupt
	xcord[current] = get_touch_x();
	ycord[current] = get_touch_y();
	current++;
	if(touchItems<1000) touchItems++;
	if(current >= 1000) current = 0;
	startTouch = true;
	PB[0x410/4] = 0x4; //enable pin 2
}

void initSSI(void){
	
	SSI0[0x4/4] &= 0xD; //disable SSIO, SSICR1 disable SSE bit (2nd)
	SSI0[0x4/4] = 0x00000000; //master mode
	SSI0[0xFC8/4] = 0x0; //use sysclock clock
	//SSI clock needs to be 2MHz
	//SSIClk = SysClk / (CPSDVSR * (1 + SCR))
	//2Mhz = 16MHz/ (CPSDVSR *(1+1))   CPSDVSR= 4
	SSI0[0x10/4] = 0x4; //CPSDVSR = 4 
	SSI0[0x0] = 0x1C7; //SCR = 1, SPH = SPO = 1, DSS= 8 bits
	SSI0[0x4/4] |= 0x2; //enable SSIO
	
	
	
	
}



void computeTouch(unsigned int xval, unsigned int yval){
	if(xval > 0 && xval < 1 && yval > 0 && yval < 1){
		toggleBtn(&btn1);
	}
	if(xval > 0 && xval < 1 && yval > 0 && yval < 1){
		toggleBtn(&btn2);
	}
	if(xval > 0 && xval < 1 && yval > 0 && yval < 1){
		toggleBtn(&btn3);
	}
}



void run(void){
	unsigned xval, yval;
	while(1){
		while(!startTouch); //wait until a touch has started
		while(PB[0x10/4] >> 2 == 1); //wait until touch is done
		startTouch= false;
		current = touchItems = 0;
		for(i = 0; i<touchItems; i++){
			xval += xcord[i];
			yval += ycord[i];
		}
		xval /= touchItems;
		yval /= touchItems;
		computeTouch(xval, yval);
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
	
	initPorts();
	initSSI();
	lcd_init();
	clear_lcd(black);
	initBtn();
	run();
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
