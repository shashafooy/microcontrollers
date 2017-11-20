#include "Shapes.h"


BtnData* BtnData_new(int xb, int xe, int yb, int ye, unsigned short color){
	BtnData* p;
	p->x_begin = xb; p->x_end = xe; p->y_begin=yb; p->y_end = ye;
	p->color = color;
	return p;
}



void LCD_SetPage(unsigned short Start, unsigned short End, int ledNum){
	write_cmd(0x2B,ledNum);
	write_dat2(Start,ledNum);
	write_dat2(End,ledNum);
	
}


void LCD_setColumn(unsigned short Start, unsigned short End, int ledNum){
	write_cmd(0x2A,ledNum);
	write_dat2(Start,ledNum);
	write_dat2(End,ledNum);
}




void clear_lcd(unsigned short color, int ledNum){
	BtnData btn;
	btn.x_begin = 0; btn.x_end = 240; btn.y_begin = 0; 
	btn.y_end = 320;	btn.color = color;
	draw_rectangle(btn,ledNum);
}




void draw_rectangle(BtnData btn, int ledNum){
	unsigned int area;
	unsigned int i;

	LCD_SetPage(btn.y_begin,btn.y_end, ledNum);
	LCD_setColumn(btn.x_begin,btn.x_end, ledNum);
	
	write_cmd(0x2c, ledNum);
	area	= (btn.x_end-btn.x_begin+1)*(btn.y_end-btn.y_begin+1);
	for(i=0; i<area; i++){
		write_dat2(btn.color, ledNum);
	}
	
}
