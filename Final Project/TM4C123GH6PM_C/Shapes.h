#ifndef SHAPES_H
#define SHAPES_H
#include <stdbool.h>

#include "lcd.h"

typedef struct{
	
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
	unsigned short color;
	bool on;
	
}BtnData;

BtnData BtnData_new(int xb, int xe, int yb, int ye, unsigned short color);

void clear_lcd(unsigned short color, int lcdNum);
//void draw_rectangle(int x_begin, int y_begin, int x_end, int y_end, unsigned short color);
void draw_rectangle(BtnData btn1, int ledNum);
void draw_circle(BtnData startCircle, int lcdNum);
void draw_L(int ledNum);
void draw_W(int ledNum);



#endif
