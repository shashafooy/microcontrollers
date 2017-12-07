#ifndef FONT_H
#define FONT_H
#include "lcd.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
extern unsigned char font[96][6];


void DrawPixel(unsigned char startx, unsigned char starty, unsigned short color, int lcdNum);

void DrawChar(char c, unsigned char x, unsigned char y, int lcdNum, int scale);

void DrawString(const char* str, unsigned char x, unsigned char y, int lcdNum, int scale);


#endif
