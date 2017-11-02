#include "Shapes.h"





void LCD_SetPage(unsigned short Start, unsigned short End){
	write_cmd(0x2B);
	write_dat2(Start);
	write_dat2(End);
	
}


void LCD_setColumn(unsigned short Start, unsigned short End){
	write_cmd(0x2A);
	write_dat2(Start);
	write_dat2(End);
}




void clear_lcd(unsigned short color){
	struct BtnData btn;
	btn.x_begin = 0; btn.x_end = 240; btn.y_begin = 0; 
	btn.y_end = 320;	btn.color = color;
	draw_rectangle(btn);
}




void draw_rectangle(struct BtnData btn/*int x_begin, int y_begin, int x_end, int y_end, unsigned short color*/){
	unsigned int area;
	unsigned int i;

	LCD_SetPage(btn.y_begin,btn.y_end);
	LCD_setColumn(btn.x_begin,btn.x_end);
	
	write_cmd(0x2c);
	area	= (btn.x_end-btn.x_begin+1)*(btn.y_end-btn.y_begin+1);
	for(i=0; i<area; i++){
		write_dat2(btn.color);
	}
	
}
