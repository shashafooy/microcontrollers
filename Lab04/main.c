#include "ps2_to_ascii.h"


//Partytime 

//GREEN - KBD Clock PE1
//BROWN - KDB Data PE0
//YELLOW - +5 V/DC
//ORANGE - GND 

//not 5V tolerant PD4, PD5, PB0, PB1

void init(void);
void portFInterrupt(void);
void portEInterrupt(void);
void run(void);
void sendData(void);

volatile unsigned char keystroke = 0x00;
volatile unsigned int stop = 1;
volatile int numChars=0;

int charSize = 0;
int startbit = 0;
char dataStore[500];
int skipStore = 0;

volatile unsigned int RCGC2 __attribute__((at(0x400FE108)));
volatile unsigned int PE_DATA __attribute__((at(0x400243FC)));
volatile unsigned int PF_DATA __attribute__((at(0x400253FC)));
volatile unsigned int PF_LOCK __attribute__((at(0x40025520)));
volatile unsigned int INTERUPT_CONTROL __attribute__((at(0xE000E100)));
volatile unsigned int RCGCUART0 __attribute__((at(0x400FE618)));


unsigned char *PA = (unsigned char *) 0x40004000;
unsigned char *PE = (unsigned char *) 0x40024000;
unsigned char *PF = (unsigned char *) 0x40025000;
unsigned int *UART0 = (unsigned int *) 0x4000c000;
unsigned char *SYSCTL = (unsigned char *) 0x400FE000;
unsigned char *CORE = (unsigned char *) 0xE000E000;
volatile char temp;

#define RCGC2_PE 0x00000010
#define RCGC2_PF 0x00000020
#define RCGC2_PA 0x00000001
#define UNLOCK 0x4C4F434B



void init(){
	RCGC2 = RCGC2 | RCGC2_PE | RCGC2_PF; //enable port E,F

	
	PE[0x420] &= 0x00; //turn off alt function
	PE[0x400] = 0x00; //pin 1,2 input
	PE[0x51c] |= 0x03; //enable pin 1,2
	
	
	PF_LOCK = UNLOCK;
	PF[0x524] = 0x11; //unlock pin 0,4
	PF[0x420] &= 0x00; //turn off alt function
	PF[0x510] = 0x11; //pull up
	PF[0x400] = 0x00; //PF0 input
	PF[0x51c] = 0x11; //activate pin 0,4
	
	SYSCTL[0x104] = 0x1;
	//RCGCUART0 = 0x1; // enable & provide clock to Module 0 UART pin PA1
	RCGC2 = RCGC2 | RCGC2_PA; //clock for port A
	__nop();
	__nop();
	PA[0x420] = 0x3; //alt function: UART on pin 1,0
	PA[0x51c] = 0x3; //pin 1,0 enable
	PA[0x52c] = 0x11; //setup UART to rx and tx
	
	INTERUPT_CONTROL = 0x40000010; //enable interrupts for port E, F
	
	//interrupt for port E clock pin 1
	
	PE[0x404] = 0x0; //interrupt on edge
	PE[0x408] = 0x0; //disable both edges
	PE[0x40c] = 0x0; //falling edge
	//PE[0x410] = 0x2; //enable interrupt pin 1
	//
	//PE[0x414] interrupt status
	
	PF[0x404] = 0x0; //interrupt on edge
	PF[0x408] = 0x0; //disable both edges
	PF[0x40c] = 0x0; //falling edge
	PF[0x410] = 0x11; //enable interrupt pin 0,4	
	
	//UART 0
	UART0[0x30/4] = 0x0; //disable UART0
	UART0[0x24/4] = 0x68; //baud rate 9600, 104.166667=(16Mhz/(16*9600)	
	UART0[0x28/4] = 0xb; //decimal remainder int (0.1667*2^6 + 0.5) = 11
	UART0[0x2c/4] = 0x70; //0b0111 0000 8 bit start/stop bit, no parity
	UART0[0xFC8/4] = 0x0; //use system clock
	UART0[0x30/4] = 0x101; //enable uart, needs to be 0x101, do in two chunks


}

	
void GPIOF_Handler(void){
	if(PF[0x414] & 0x10){ //start button SW1
		PF[0x41c] = 0x10; //acknowledge interrupt
		PE[0x410] |= 0x2; //enable clock interrupt
		PE[0x41c] = 0x2; //acknowledge pin 1 infterrupt

		stop = 0;
		return;
	}
	if(PF[0x414] & 0x01) { //stop button SW2
		PF[0x41c] = 0x01; //acknowledge interrupt
		PE[0x410] &= 0xD; //disable clock interrupt
		stop = 1;
		return;
	}
}	
	
	
void GPIOE_Handler(void){ //***********Falling Clock edge*************
	if(PE[0x418] & 0x2){ 
	//clock interrupt, pin 1
		PE[0x41c] = 0x2; //acknowledge pin 1 infterrupt
		if(charSize < 11){ //keyboard sends 11 bits at a time(maybe)
			if(charSize > 0 && charSize < 9){	//recieved start bit
				keystroke = keystroke >> 1;
				temp = PE[0x004] << 7; //shift data left by 7, now 7th bit
				keystroke |= temp; //store shifted 7th bit of keyboard data
			}	
			charSize++;
		}
	}
}	
	
	
void run(void) {
	int skipStore =0; //bool
	while(1){
		while(charSize < 9) if(stop) sendData(); //loop until we obtain a full character of data
		if(keystroke == (unsigned char)0xF0) skipStore = 2; //skip storing char into array for next 2 values
		if(skipStore) skipStore --;
		else{
			dataStore[numChars++] = ps2_to_ascii[keystroke - 0x15]; //store the character into the next array index, convered to ascii
		}
		while(charSize < 11); //loop waiting for stop and parody bit. Continue when full package(11) is sent
		charSize =0;
	}
}

void sendData(void){
	int i;
	int j;
	for(i=0; i<numChars; i++){
		
		UART0[0x0] = dataStore[i]; 
		for(j=0; j<5000; j++) __nop();
	//	temp = UART0[0x18] & 0x80;
//		while((UART0[0x18] & 0x80) != 0x80);
	}
	while(stop);
	numChars = 0;
	run();
}


int main(void){
	init();
	//your code here.
	while(stop);
	run();
}
