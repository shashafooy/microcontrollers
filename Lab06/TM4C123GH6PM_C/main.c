#include "TM4C123GH6PM.h"
#include "Init.h"




int main(void)
{
	GPIOInit();
	ADCInit();
	I2CInit();
	//your code here.
	while(1);
}
