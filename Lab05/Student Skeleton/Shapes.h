#ifndef SHAPES_H
#define SHAPES_H
#include <stdbool.h>

#include "lcd.h"

struct BtnData{
	
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
	unsigned short color;
	bool on;
	
};


void LCD_SetPage(unsigned short Start, unsigned short End);
void LCD_setColumn(unsigned short Start, unsigned short End);

void clear_lcd(unsigned short color);
//void draw_rectangle(int x_begin, int y_begin, int x_end, int y_end, unsigned short color);
void draw_rectangle(struct BtnData btn1);


#endif
