#ifndef INIT_H
#define INIT_H

#include "TM4C123GH6PM.h"

#define RCGC2E 0x40024000
#define RCGC2B 


void ADCInit(void);
void timerInit(void);
void I2CInit(void);

#endif
