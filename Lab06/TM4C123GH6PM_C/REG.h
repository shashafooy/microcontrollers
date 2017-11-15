#ifndef REG_H
#define REG_H

typedef struct{
	volatile unsigned int _0:1,_1:1,_2:1,_3:1,_4:1,_5:1,_6:1,_7:1,_8:1,_9:1,_10:1,_11:1,_12:1,_13:1,_14:1,_15:1,_16:1,_17:1,_18:1,_19:1,_20:1,_21:1,_22:1,_23:1,_24:1,_25:1,_26:1,_27:1,_28:1,_29:1,_30:1,_31:1;	
}bitReg;

typedef struct {
	volatile unsigned short _0,_1;
}halfwordReg;

typedef struct{
	volatile unsigned char _0,_1,_2,_3;
}byteReg;

typedef union{
	bitReg b;
	byteReg B;
	halfwordReg H;
	volatile unsigned int W;
}REG;
#endif
