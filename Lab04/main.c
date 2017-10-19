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
unsigned char *PE = (unsigned char *) 0x40024000;
unsigned int PE_DATA __attribute__((at(0x400243FC)));




void init(){}

	
void portFInterrupt(void){
	sendData();
}	
	
	
void portEInterrupt(void){ //***********Falling Clock edge*************
	if(charSize < 11){ //keyboard sends 11 bits at a time(maybe)
		if(charSize > 0 && charSize < 9){	//recieved start bit
			keystroke = keystroke >> 1;
			char temp = PE[0x004] << 7; //shift data left by 7, now 7th bit
			keystroke |= temp; //store shifted 7th bit of keyboard data
		}	
		charSize++;
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
