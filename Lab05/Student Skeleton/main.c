#include "lcd.h"
#include "lcd_configuration_test.h" //You can remove this line after you've removed the call to test_configuartion(unsigned int, unsigned char*, unsigned int*)

unsigned int *PA = (unsigned int *) 0x40004000;
unsigned int *PB = (unsigned int *) 0x40005000;
unsigned int *SSI0 = (unsigned int *) 0x40008000;
unsigned int *SYSCTL = (unsigned int *) 0x400FE000;
unsigned int *CORE = (unsigned int *) 0xE000E000;

#define RCGC2_PA 0x00000001
#define RCGC2_PB 0x00000002
#define UNLOCK 0x4c4f434b

void initPorts(void){
	
	SYSCTL[0x61C/4] = 0x1; //RCGCSSI enable SSI0 clock
	SYSCTL[0x108/4] |= RCGC2_PA | RCGC2_PB;
	
	PA[0x420/4] = 0x3C; //enable alt function for pins 2-5
	PA[0x52c/4] = 0x222200; //SSI function pin 2-5
	PA[0x51c/4] = 0x3C; //Digital enable pin 2-5
	
	PB[0x420/4] = 0x0; //disable AF
	PB[0x400/4] = 0x1; //pin 0 output
	PB[0x51c/4] = 0x1; //enable pin 0
	
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



int main(void)
{
	//values for configuration. Assign these to the values you chose for your peripherals.
	//
	//ssiModuleBaseAddress should be assigned to the base address of your SSI/SPI module. 
	//cmdDataSelectGPIOBaseAddress should be assigned to the base address of the GPIO port you are using as your command/data Select. ***PB0***
	//cmdDataSelectGPIOPinNumber should be assigned the the pin number (0-7) of the GPIO pin you are using as your command/data select pin. 
	//
	unsigned int* ssiModuleBaseAddress             = (unsigned int*) 0x40008000;    //default value for ssi0
	unsigned char* cmdDataSelectGPIOBaseAddress    = (unsigned char*) 0x40005000;   //default value for GPIO Port B
	unsigned int cmdDataSelectGPIOPinNumber        = 0x0;                           //default value for PB_0
	
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//Call your initialization functions here
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	initPorts();
	initSSI();
	
	
	//This function tests the configuration of your GPIO port and your SSI/SPI module. 
	//
	//Pass it the following parameters in the following order
	//	The pin number (0-7) of the gpio port you are using. Pass as an unsigned integer.
	//	The base address for the gpio port you are using. Pass as an unsigned character pointer
	//	The base address for the SSI/SPI module you are using. Pass as an unsigned integer pointer. 
	test_configuration(cmdDataSelectGPIOPinNumber, cmdDataSelectGPIOBaseAddress, ssiModuleBaseAddress);
	
	
	
	//After you have made sure that your ssi configuration and your gpio configuration are correct, 
	// you can delete the call to test_configuration and write your own code to draw the buttons. 
	// It is recommended that you write your LCD screen interfacing code in lcd.c and provide 
	// corresponding function declarations in lcd.h. That will allow you to copy your lcd.c and lcd.h 
	// files into any other project that requires the LCD screen. 
	while(1);
}
