

#include "lcd.h"
#include "lcd_configuration_test.h"

unsigned int *PB_lcd = (unsigned int *) 0x40005000;
unsigned int *SSI0_lcd = (unsigned int *) 0x40008000;



//This function configures the LCD screen for using. 
//
//It assumes that there exist write_dat and write_cmd functions that function properly and that peripherals are configured and enabled. 
void lcd_init(void)
{
	int i;
		write_cmd(0x28); //display off
	
    write_cmd(0xCB);
    write_dat(0x39);
    write_dat(0x2C);
    write_dat(0x00);
    write_dat(0x34);
    write_dat(0x02);
            
    write_cmd(0xCF);
    write_dat(0x00);
    write_dat(0XC1);
    write_dat(0X30);
            
    write_cmd(0xE8);
    write_dat(0x85);
    write_dat(0x00);
    write_dat(0x78);
            
    write_cmd(0xEA);
    write_dat(0x00);
    write_dat(0x00);
            
    write_cmd(0xED);
    write_dat(0x64);
    write_dat(0x03);
    write_dat(0X12);
    write_dat(0X81);
            
    write_cmd(0xF7);
    write_dat(0x20);
            
    write_cmd(0xC0);    //Power control
    write_dat(0x23);   //VRH[5:0]
            
    write_cmd(0xC1);    //Power control
    write_dat(0x10);   //SAP[2:0];BT[3:0]
            
    write_cmd(0xC5);    //VCM control
    write_dat(0x3e); //对比度调节
    write_dat(0x28);
            
    write_cmd(0xC7);    //VCM control2
    write_dat(0x86);  //--
            
    write_cmd(0x36);    // Memory Access Control
    write_dat(0x48); //C8       //48 68竖屏//28 E8 横屏
            
    write_cmd(0x3A);
    write_dat(0x55);
            
    write_cmd(0xB1);
    write_dat(0x00);
    write_dat(0x18);
            
    write_cmd(0xB6);    // Display Function Control
    write_dat(0x08);
    write_dat(0x82);
    write_dat(0x27);
            
    write_cmd(0xF2);    // 3Gamma Function Disable
    write_dat(0x00);
            
    write_cmd(0x26);    //Gamma curve selected
    write_dat(0x01);
            
    write_cmd(0xE0);    //Set Gamma
    write_dat(0x0F);
    write_dat(0x31);
    write_dat(0x2B);
    write_dat(0x0C);
    write_dat(0x0E);
    write_dat(0x08);
    write_dat(0x4E);
    write_dat(0xF1);
    write_dat(0x37);
    write_dat(0x07);
    write_dat(0x10);
    write_dat(0x03);
    write_dat(0x0E);
    write_dat(0x09);
    write_dat(0x00);
            
    write_cmd(0XE1);    //Set Gamma
    write_dat(0x00);
    write_dat(0x0E);
    write_dat(0x14);
    write_dat(0x03);
    write_dat(0x11);
    write_dat(0x07);
    write_dat(0x31);
    write_dat(0xC1);
    write_dat(0x48);
    write_dat(0x08);
    write_dat(0x0F);
    write_dat(0x0C);
    write_dat(0x31);
    write_dat(0x36);
    write_dat(0x0F);
            
    write_cmd(0x11);    //Exit Sleep
		for( i = 0; i < 20000; i++) { i++;}//Wait a long time. 

    write_cmd(0x29);    //Display on
    //writeCmd(0x2c);
}



//The following two functions call functions defined in lcd_configuration_test.o 
//They're included to help you get the LCD screen functioning. However, 
//eventually, you should write your own code to do what the following 
//two functions do. Then you can remove the lcd_configuration_test.o file
//from the project and comment out these two lines.
//extern void	lcd_configuration_write_data(unsigned char data);
//extern void	lcd_configuration_write_command(unsigned char command);

//This function writes data to the LCD screen
void write_dat(unsigned char data)
{
	//Note that this function merely calls a function that is defined in the lcd_configuration_test.o file
	//
	//Use this function to make sure you can get your LCD screen to function, 
	// but then try your hand at writing the code in lcd_configuration_test.o
	//lcd_configuration_write_data(data);
	
	PB_lcd[0x4/4] |= 0x1; //cx data;
	PB_lcd[0x180/4] = 0x20;  // 010 0000 LCD chip 0, touch chip 1 (active low) 0x20
	SSI0_lcd[0x8/4] = data;
	while(SSI0_lcd[0xc/4] >> 4 == 1); //check busy bit
	PB_lcd[0x180/4] = 0x60; //LCD chip 1 (off) 0x60
	
}

void write_dat2(unsigned short data){
	write_dat((unsigned char)(data >> 8));	
	write_dat((unsigned char)data);
}

//This function writes a command to the LCD screen
void write_cmd(unsigned char command)
{
	//Note that this function merely calls a function that is defined in the lcd_configuration_test.o file
	//
	//Use this function to make sure you can get your LCD screen to function, 
	// but then try your hand at writing the code in lcd_configuration_test.o
	//lcd_configuration_write_command(command);
	PB_lcd[0x4/4] = 0x0; //cx data;
	PB_lcd[0x180/4] = 0x20;  // 010 0000 LCD chip 0, touch chip 1 (active low)
	SSI0_lcd[0x8/4] = command;
	while(SSI0_lcd[0xc/4] >> 4 == 1); //check busy bit
	PB_lcd[0x180/4] = 0x60; //LCD chip 1 (off)

}
