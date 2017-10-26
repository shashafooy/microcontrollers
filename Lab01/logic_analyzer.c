unsigned char *PA = (unsigned char *) 0x40004000;
unsigned char *PC = (unsigned char *) 0x40006000;
unsigned char *PE = (unsigned char *) 0x40024000;

unsigned int *SYSCTL_int = (unsigned int *) 0x400FE000;

#define RCC_13Mhz  	0x7400540;


void SYSCLK()
{
	// 0. set sysclk to main osc
  SYSCTL_int[0x060/4] = RCC_13Mhz;
	SYSCTL_int[0x608/4] |= 0x15;
}

void setup_DH_pins()
{
	//Unlock
	PC[0x520]=0x4B;
	PC[0x521]=0x43;
	PC[0x522]=0x4F;
	PC[0x523]=0x4C;
	//PC[4:7] as output
	PC[0x400] |= 0xF0;
	//PC[4:7] as GPOI
	PC[0x420] &= 0x0F;
	
	//PC[4:7] enable
	PC[0x51C] |= 0xF0;
}

void setup_DL_pins()
{
	//Unlock
	PE[0x520]=0x4B;
	PE[0x521]=0x43;
	PE[0x522]=0x4F;
	PE[0x523]=0x4C;
	//PE[0:3] as output
	PE[0x400] |= 0x0F;
	//PE[0:3] as GPOI
	PE[0x420] &= 0xF0;
	
	//PE[0:3] enable
	PE[0x51C] |= 0x0F;
}

void writeChar(unsigned char CHR)
{
	PA[0x3FC] &= 0x7F;
	
	PE[0x3FC] = CHR;	//DATABUS = DL
	PC[0x3FC] = CHR;	//DATABUS = DH
	
	PA[0x3FC] |= 0x80;
	
}
void setup_wr_rd_pins()
{
	//Unlock
	PA[0x520]=0x4B;
	PA[0x521]=0x43;
	PA[0x522]=0x4F;
	PA[0x523]=0x4C;
	//PA[7] as output
	PA[0x400] |= 0x80;
	//PA[7] as GPOI
	PA[0x420] &= 0x7F;
	
	//PA[7] enable
	PA[0x51C] |= 0x80;
}

void write()
{
	while(1)
	{
		writeChar(0x48);
		writeChar(0x65);
		writeChar(0x6C);
		writeChar(0x6C);
		writeChar(0x6F);
		writeChar(0x21);
		writeChar(0x0A);
	}
}


int main(void)
{
	SYSCLK();
	setup_wr_rd_pins();
	setup_DL_pins();
	setup_DH_pins();
	write();
	
	return 0;
}
