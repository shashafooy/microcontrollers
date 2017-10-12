#include "ps2_to_ascii.h"

//Partytime 

//GREEN - KBD Clock PE1
//BROWN - KDB Data PE0
//YELLOW - +5 V/DC
//ORANGE - GND 

//not 5V tolerant PD4, PD5, PB0, PB1
void init(){}

void clockfall(void){
	queue.push(databit)
}


int main(void)
{
	init();
	//your code here.
	while(1){
		while(queue.size() < 8);
		char* x;
		for (int i=0; i<8; i++){
			x[i] = queue.pop();
		}
		queue.reset();
		x = ps2_to_ascii[x - 0x15];
		store(x);
		
		
	}
}
