#include "TM4C123GH6PM.h"
#include "Init.h"

volatile unsigned int Voltage;
unsigned int SINUSOID_MAX = 40;
volatile int I2C_Begin;

void I2C_12bit(unsigned short data);
void ADC0SS0_Handler(void);
void WTIMER1A_Handler(void);




unsigned short SINUSOID[] = {
0x1f4,0x242,0x28f,0x2d7,0x31a,0x356,0x389,0x3b2,
0x3d0,0x3e2,0x3e8,0x3e2,0x3d0,0x3b2,0x389,0x356,
0x31a,0x2d7,0x28f,0x242,0x1f4,0x1a6,0x159,0x111,
0xce,0x92,0x5f,0x36,0x18,0x6,0x0,0x6,
0x18,0x36,0x5f,0x92,0xce,0x111,0x159,0x1a6
};
unsigned int currSine, numVoltage;

//Average voltage to set Timer 2
void WTIMER1A_Handler(void){
	unsigned int load;
	unsigned int freq;
	WTIMER1->ICR = 0x1; //ack interrupt
	Voltage /= numVoltage;
	numVoltage = 0;
	freq = (int)(100.0 + (double)Voltage*900.0/4095.0);
	load = (int)(80000000.0/(freq * SINUSOID_MAX));
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

//14 96 97
void I2C_12bit(unsigned short data){
	//DAC address is 0x62
//	unsigned char temp;
	//while (I2C0->MCS & 0x1); //poll busy
	I2C0->MDR = (unsigned char)((data >> 8) & 0x0F); //8-11
	if(!I2C_Begin){ //have not sent first bit
		I2C0->MSA = (0x62 << 1); //slave address and tx
		I2C0->MCS = 0x3; //set start enable bits
		I2C_Begin = 1;
	}else{
		I2C0->MCS = 0x1; //set enable bits
	}
	while (I2C0->MCS & 0x1); //poll busy
	I2C0->MDR = (unsigned char) data; //0-7
	I2C0->MCS = 0x1; //set enable bits
	while ((I2C0->MCS & 0x1) == 0x1); 
}

int main(void)
{
	currSine=numVoltage=I2C_Begin=0;
	ADCInit();
	I2CInit();
	timerInit();
	
	while(1);
}
