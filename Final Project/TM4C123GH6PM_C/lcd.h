#ifndef LCD_C 
#define LCD_C

//definitions of useful colors. 
// page 238 of the data sheet.
#define white          0xFFFF
#define black          0x0001
#define grey           0xF7DE
#define blue           0x001F
#define red            0xF800
#define magenta        0xF81F
#define green          0x07E0
#define cyan           0x7FFF
#define yellow         0xFFE0






//This function configures the LCD screen for using. 
//
//It assumes that there exist write_dat and write_cmd functions that function properly and that peripherals are configured and enabled.  
void lcd_init(void);

//This function writes data to the LCD screen
//
//It assumes that peripherals are configured configured and enabled
void write_dat(unsigned char data);
void write_dat2(unsigned short data);

//This function writes a command to the LCD screen
//
//It assumes that peripherals are configured configured and enabled
void write_cmd(unsigned char command);

unsigned int get_touch_x(void);
unsigned int get_touch_y(void);

void waitBusy(void);

#endif
