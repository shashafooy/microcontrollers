#include "lcd.h"
#include "lcd_configuration_test.h" //You can remove this line after you've removed the call to test_configuartion(unsigned int, unsigned char*, unsigned int*)





int main(void)
{
	//values for configuration. Assign these to the values you chose for your peripherals.
	//
	//ssiModuleBaseAddress should be assigned to the base address of your SSI/SPI module. 
	//cmdDataSelectGPIOBaseAddress should be assigned to the base address of the GPIO port you are using as your command/data Select. ***PB0***
	//cmdDataSelectGPIOPinNumber should be assigned the the pin number (0-7) of the GPIO pin you are using as your command/data select pin. 
	//
	unsigned int* ssiModuleBaseAddress;          //   = (unsigned int*) 0x40008000;    //default value for ssi0
	unsigned char* cmdDataSelectGPIOBaseAddress; //   = (unsigned char*) 0x40005000;   //default value for GPIO Port B
	unsigned int cmdDataSelectGPIOPinNumber;     //   = 0x0;                           //default value for PB_0
	
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//Call your initialization functions here
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	
	
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
