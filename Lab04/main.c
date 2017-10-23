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
int charSize = 0;
int startbit = 0;
char dataStore[1000];
int skipStore = 0;

volatile unsigned int RCGC2 __attribute__((at(0x400FE108)));
volatile unsigned int PE_DATA __attribute__((at(0x400243FC)));
volatile unsigned int PF_DATA __attribute__((at(0x400253FC)));
volatile unsigned int PF_LOCK __attribute__((at(0x40025520)));

unsigned char *PE = (unsigned char *) 0x40024000;
unsigned char *PF = (unsigned char *) 0x40025000;
volatile char temp;

#define RCGC2_PE 0x00000010
#define RCGC2_PF 0x00000020
#define UNLOCK 0x4C4F434B



void init(){
	RCGC2 = RCGC2 | RCGC2_PE | RCGC2_PF; //enable port E
	
	PE[0x420] &= 0x00; //turn off alt function
	PE[0x400] = 0x00; //pin 1,2 input
	PE[0x51c] |= 0x03; //enable pin 1,2
	
	PF_LOCK = UNLOCK;
	PF[0x524] = 1; //unlock pin 0
	PF[0x420] &= 0x00; //turn off alt function
	PF[0x514] = 0x1; //pull down
	PF[0x400] = 0x0; //PF0 input
	PF[0x51c] = 0x1; //activate pin 0
	
	
	
	
	
	//interrupt for port E clock pin 1
	PE[0x404] = 0x0; //interrupt on edge
	PE[0x408] = 0x0; //disable both edges
	PE[0x40c] = 0x0; //falling edge
	PE[0x410] = 0x2; //enable interrupt
	//PE[0x414] interrupt status
	
	PF[0x404] = 0x0;
	PF[0x408] = 0x0;
	PF[0x40c] = 0x1; //rising edge
	PE[0x410] = 0x1; //enable interrupt
	
	
	//UART Hell
}

	
void GPIOF_Handler(void){
	PF[0x41c] = 0x1; //acknowledge pin 0, SW2
	sendData();
}	
	
	
void GPIOE_Handler(void){ //***********Falling Clock edge*************
	switch(PE[0x414]){ //change to interrupt pin
		case 0x2: //clock interrupt, pin 1
			PE[0x41c] = 0x2; //acknowledge pin 1 interrupt
			if(charSize < 11){ //keyboard sends 11 bits at a time(maybe)
				if(charSize > 0 && charSize < 9){	//recieved start bit
					keystroke = keystroke >> 1;
					temp = PE[0x004] << 7; //shift data left by 7, now 7th bit
					keystroke |= temp; //store shifted 7th bit of keyboard data
				}	
				charSize++;
			}
			return;
		default:
			return;
	}
}	
	
	
void run(void) {
	int i=0;
	int skipStore =0; //bool
	while(1){
		while(charSize < 9); //loop until we obtain a full character of data
		if(keystroke == (unsigned char)0xF0) skipStore = 2; //skip storing char into array for next 2 values
		if(skipStore) skipStore --;
		else{
			dataStore[i++] = ps2_to_ascii[keystroke - 0x15]; //store the character into the next array index, convered to ascii
		}
		while(charSize < 11); //loop waiting for stop and parody bit. Continue when full package(11) is sent
		charSize =0;
	}
}	
	
void sendData(void){}


int main(void){
	init();
	//your code here.
	run();
}
