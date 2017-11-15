#include "TM4C123GH6PM.h"
#include "Init.h"

volatile unsigned int Voltage;
#define SINUSOID_MAX 40;

void I2C_12bit(unsigned short data);
void ADC0SS0_Handler(void);
void WTIMER1A_Handler(void);




unsigned short SINUSOID[] = {
	0x800,0x940,0xa78,0xba1,0xcb3,0xda7,0xe78,0xf20,
0xf9b,0xfe6,0xfff,0xfe6,0xf9b,0xf20,0xe78,0xda7,
0xcb3,0xba1,0xa78,0x940,0x800,0x6bf,0x587,0x45e,
0x34c,0x258,0x187,0xdf,0x64,0x19,0x0,0x19,
0x64,0xdf,0x187,0x258,0x34c,0x45e,0x587,0x6bf	
	};
unsigned int currSine, numVoltage;

//Average voltage to set Timer 2
void WTIMER1A_Handler(void){
	unsigned int load;
	WTIMER1->ICR = 0x1; //ack interrupt
	Voltage /= numVoltage;
	numVoltage = 0;
	load = (int)(80000000.0/(100.0 + (double)Voltage*900.0/4095.0) - 1.0);
	WTIMER2->CTL = 0x0; //disable timer2
	WTIMER2->TAILR = load;//80000000*(1/freq) - 1;
	WTIMER2->CTL = 0x1;
}

//Send sinusoid value to i2c
void WTIMER2A_Handler(void){
	WTIMER2->ICR = 0x1; //ack interrupt
	I2C_12bit(SINUSOID[currSine++]);
	currSine%=SINUSOID_MAX;
}

void ADC0SS0_Handler(void){
	ADC0->ISC = 0x1; //acknowledge interrupt
	Voltage += ADC0->SSFIFO0;	
	numVoltage++;
}


void I2C_12bit(unsigned short data){
	//DAC address is 0x62
	while (I2C0->MCS & 0x1); //poll busy
	I2C0->MSA = (0x62 << 1) + 0x0; //slave address and tx
	I2C0->MDR = (char)(data >> 8); //8-11
	I2C0->MCS = 0x3; //set start enable bits
	while (I2C0->MCS & 0x1); //poll busy
	I2C0->MDR = (char) data; //0-7
	I2C0->MCS = 0x5; //set stop enable bits
	while ((I2C0->MCS & 0x1) == 0x1); 
}

int main(void)
{
	currSine=numVoltage=0;
	ADCInit();
	I2CInit();
	//your code here.
	while(1);
}
