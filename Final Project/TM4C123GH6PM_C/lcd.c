#include "lcd.h"

//waits for specified lcd to not be busy
void waitBusy(){
	while(SSI0->SR >> 2 == 1); 
}


//This function configures the LCD screen for using. 
//
//It assumes that there exist write_dat and write_cmd functions that function properly and that peripherals are configured and enabled. 
void lcd_init(void)
{
	int i;
	/*LCD 0*/
	{
		write_cmd(0x28,0); //display off
	
    write_cmd(0xCB,0);
    write_dat(0x39,0);
    write_dat(0x2C,0);
    write_dat(0x00,0);
    write_dat(0x34,0);
    write_dat(0x02,0);
            
    write_cmd(0xCF,0);
    write_dat(0x00,0);
    write_dat(0XC1,0);
    write_dat(0X30,0);
            
    write_cmd(0xE8,0);
    write_dat(0x85,0);
    write_dat(0x00,0);
    write_dat(0x78,0);
            
    write_cmd(0xEA,0);
    write_dat(0x00,0);
    write_dat(0x00,0);
            
    write_cmd(0xED,0);
    write_dat(0x64,0);
    write_dat(0x03,0);
    write_dat(0X12,0);
    write_dat(0X81,0);
            
    write_cmd(0xF7,0);
    write_dat(0x20,0);
            
    write_cmd(0xC0,0);    //Power control
    write_dat(0x23,0);   //VRH[5:0]
            
    write_cmd(0xC1,0);    //Power control
    write_dat(0x10,0);   //SAP[2:0];BT[3:0]
            
    write_cmd(0xC5,0);    //VCM control
    write_dat(0x3e,0); //对比度调节
    write_dat(0x28,0);
            
    write_cmd(0xC7,0);    //VCM control2
    write_dat(0x86,0);  //--
            
    write_cmd(0x36,0);    // Memory Access Control
    write_dat(0x48,0); //C8       //48 68竖屏//28 E8 横屏
            
    write_cmd(0x3A,0);
    write_dat(0x55,0);
            
    write_cmd(0xB1,0);
    write_dat(0x00,0);
    write_dat(0x18,0);
            
    write_cmd(0xB6,0);    // Display Function Control
    write_dat(0x08,0);
    write_dat(0x82,0);
    write_dat(0x27,0);
            
    write_cmd(0xF2,0);    // 3Gamma Function Disable
    write_dat(0x00,0);
            
    write_cmd(0x26,0);    //Gamma curve selected
    write_dat(0x01,0);
            
    write_cmd(0xE0,0);    //Set Gamma
    write_dat(0x0F,0);
    write_dat(0x31,0);
    write_dat(0x2B,0);
    write_dat(0x0C,0);
    write_dat(0x0E,0);
    write_dat(0x08,0);
    write_dat(0x4E,0);
    write_dat(0xF1,0);
    write_dat(0x37,0);
    write_dat(0x07,0);
    write_dat(0x10,0);
    write_dat(0x03,0);
    write_dat(0x0E,0);
    write_dat(0x09,0);
    write_dat(0x00,0);
            
    write_cmd(0XE1,0);    //Set Gamma
    write_dat(0x00,0);
    write_dat(0x0E,0);
    write_dat(0x14,0);
    write_dat(0x03,0);
    write_dat(0x11,0);
    write_dat(0x07,0);
    write_dat(0x31,0);
    write_dat(0xC1,0);
    write_dat(0x48,0);
    write_dat(0x08,0);
    write_dat(0x0F,0);
    write_dat(0x0C,0);
    write_dat(0x31,0);
    write_dat(0x36,0);
    write_dat(0x0F,0);
            
    write_cmd(0x11,0);    //Exit Sleep
	}
		/*****************
		******************
		******************/
		/*LCD 1*/
		{
		write_cmd(0x28,1); //display off
	
    write_cmd(0xCB,1);
    write_dat(0x39,1);
    write_dat(0x2C,1);
    write_dat(0x00,1);
    write_dat(0x34,1);
    write_dat(0x02,1);
            
    write_cmd(0xCF,1);
    write_dat(0x00,1);
    write_dat(0XC1,1);
    write_dat(0X30,1);
            
    write_cmd(0xE8,1);
    write_dat(0x85,1);
    write_dat(0x00,1);
    write_dat(0x78,1);
            
    write_cmd(0xEA,1);
    write_dat(0x00,1);
    write_dat(0x00,1);
            
    write_cmd(0xED,1);
    write_dat(0x64,1);
    write_dat(0x03,1);
    write_dat(0X12,1);
    write_dat(0X81,1);
            
    write_cmd(0xF7,1);
    write_dat(0x20,1);
            
    write_cmd(0xC0,1);    //Power control
    write_dat(0x23,1);   //VRH[5:0]
            
    write_cmd(0xC1,1);    //Power control
    write_dat(0x10,1);   //SAP[2:0];BT[3:0]
            
    write_cmd(0xC5,1);    //VCM control
    write_dat(0x3e,1); //对比度调节
    write_dat(0x28,1);
            
    write_cmd(0xC7,1);    //VCM control2
    write_dat(0x86,1);  //--
            
    write_cmd(0x36,1);    // Memory Access Control
    write_dat(0x48,1); //C8       //48 68竖屏//28 E8 横屏
            
    write_cmd(0x3A,1);
    write_dat(0x55,1);
            
    write_cmd(0xB1,1);
    write_dat(0x00,1);
    write_dat(0x18,1);
            
    write_cmd(0xB6,1);    // Display Function Control
    write_dat(0x08,1);
    write_dat(0x82,1);
    write_dat(0x27,1);
            
    write_cmd(0xF2,1);    // 3Gamma Function Disable
    write_dat(0x00,1);
            
    write_cmd(0x26,1);    //Gamma curve selected
    write_dat(0x01,1);
            
    write_cmd(0xE0,1);    //Set Gamma
    write_dat(0x0F,1);
    write_dat(0x31,1);
    write_dat(0x2B,1);
    write_dat(0x0C,1);
    write_dat(0x0E,1);
    write_dat(0x08,1);
    write_dat(0x4E,1);
    write_dat(0xF1,1);
    write_dat(0x37,1);
    write_dat(0x07,1);
    write_dat(0x10,1);
    write_dat(0x03,1);
    write_dat(0x0E,1);
    write_dat(0x09,1);
    write_dat(0x00,1);
            
    write_cmd(0XE1,1);    //Set Gamma
    write_dat(0x00,1);
    write_dat(0x0E,1);
    write_dat(0x14,1);
    write_dat(0x03,1);
    write_dat(0x11,1);
    write_dat(0x07,1);
    write_dat(0x31,1);
    write_dat(0xC1,1);
    write_dat(0x48,1);
    write_dat(0x08,1);
    write_dat(0x0F,1);
    write_dat(0x0C,1);
    write_dat(0x31,1);
    write_dat(0x36,1);
    write_dat(0x0F,1);
            
    write_cmd(0x11,1);    //Exit Sleep
	}
		for( i = 0; i < 20000; i++) { i++;}//Wait a long time. 

    write_cmd(0x29,0);    //Display on
		write_cmd(0x29,1);    //Display on
    //writeCmd(0x2c);
}




//This function writes data to the LCD screen
void write_dat(unsigned char data, int lcdNum)
{

	GPIOB->DATA |= 0x1; //cx data;
	GPIOB->DATA &= (lcdNum==0)?~0x40 : ~0x10; //LCD0/1 chip low
	SSI0->DR = data;
	waitBusy();	
	GPIOB->DATA |= (lcdNum==0)?0x40 : 0x10; //LCD0/1 chip high (off)
}

//writes 2 bytes of data to lcd
void write_dat2(unsigned short data, int lcdNum){
	write_dat((unsigned char)(data >> 8),lcdNum);	
	write_dat((unsigned char)data,lcdNum);
}

//This function writes a command to the LCD screen
void write_cmd(unsigned char command, int lcdNum)
{
	//Note that this function merely calls a function that is defined in the lcd_configuration_test.o file
	//
	//Use this function to make sure you can get your LCD screen to function, 
	// but then try your hand at writing the code in lcd_configuration_test.o
	//lcd_configuration_write_command(command);
	GPIOB->DATA &= ~0x1; //cx cmd
	GPIOB->DATA &= (lcdNum==0)?~0x40 : ~0x10; //LCD0/1 chip low
	SSI0->DR = command;
	waitBusy();
	GPIOB->DATA |= (lcdNum==0)?0x40 : 0x10; //LCD0/1 chip high (off)
}

unsigned int get_touch_x(int lcdNum){
	unsigned int retval = 0x0;
	int garbage;

	GPIOB->DATA &= (lcdNum==0)?~0x80 : ~0x20; //Touch0/1 chip low
	while(SSI0->SR >> 2 == 1) garbage = SSI0->DR;  //wait until rx empty
	
	//tx cmd
	GPIOB->DATA &= ~0x1; //cx cmd, clear pin 0
	SSI0->DR = 0xD0; //cmd: get x 101
	waitBusy();
	
	//transmit 0x0
	GPIOB->DATA |= 0x1; //cx data
	SSI0->DR = 0x00; //tx null
	waitBusy();
	SSI0->DR = 0x00; //tx null
	waitBusy();	
	
	//rx coordinate
	garbage = SSI0->DR; //garbage
	garbage++; //to get rid of warning
	retval = SSI0->DR << 5; //A11-A5
	retval |=SSI0->DR >> 3; //A4-A0
	
	GPIOB->DATA |= (lcdNum==0)?0x80 : 0x20; //Touch0/1 chip low

	return retval;
	
}
unsigned int get_touch_y(int lcdNum){
	unsigned int retval = 0x0;
	int garbage;
	
	//PB_lcd[0x180/4] = 0x40; //100 0000 LCD chip 1, touch chip 0
	GPIOB->DATA &= (lcdNum==0)?~0x80 : ~0x20; //Touch0/1 chip low
	while(SSI0->SR >> 2 == 1) garbage = SSI0->DR;  //wait until rx empty

	//tx cmd
	GPIOB->DATA &= ~0x1; //cx cmd, clear pin 0
	SSI0->DR = 0x90;
	waitBusy();
	
	//transmit 0x0
	GPIOB->DATA |= 0x1; //cx data
	SSI0->DR = 0x00; //tx null
	waitBusy();
	SSI0->DR = 0x00; //tx null
	waitBusy();	
	
	//rx coordinate
	garbage = SSI0->DR; //garbage
	garbage++; //to get rid of warning
	retval = SSI0->DR << 5; //A11-A5
	retval |=SSI0->DR >> 3; //A4-A0
	
	GPIOB->DATA |= (lcdNum==0)?0x80 : 0x20; //Touch0/1 chip low
	return retval;
}
