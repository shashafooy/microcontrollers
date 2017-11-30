#ifndef LCD_C 
#define LCD_C

#include "TM4C123.h"                    // Device header

//definitions of useful colors. 
// page 238 of the data sheet.
#define white          0xFFFF
#define black          0x0001
#define grey           0x630C//0xF7DE
#define blue           0x001F
#define red            0xF800
#define magenta        0xF81F
#define green          0x07E0
#define cyan           0x7FFF
#define yellow         0xFFE0






//This function configures the LCD screen for using. 
void lcd_init(void);

//This function writes data to the LCD screen
void write_dat(unsigned char data, int lcdNum);
void write_dat2(unsigned short data, int lcdNum);

//This function writes a command to the LCD screen
void write_cmd(unsigned char command, int lcdNum);

unsigned int get_touch_x(int lcdNum);
unsigned int get_touch_y(int lcdNum);

void waitBusy(void);

#endif
