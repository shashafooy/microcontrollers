#include "Shapes.h"


BtnData BtnData_new(int xb, int xe, int yb, int ye, unsigned short color){
	BtnData p;
	p.x_begin = xb; 
	p.x_end = xe;
	p.y_begin=yb;
	p.y_end = ye;
	p.color = color;
	return p;
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

void draw_circle(BtnData startCircle, int ledNum) {
	unsigned int diff, midx, midy;
	unsigned int i, j, rs;
	int xs, ys;
	unsigned int radius;
	bool readyWrite;
	//startCircle.x_begin = startCircle.x_begin ^ startCircle.y_begin;
	
	
	//area	= (startCircle.x_end-startCircle.x_begin+1)*(startCircle.y_end-startCircle.y_begin+1); //area of the square region the circle will be drawn in
	
	diff = startCircle.x_end - startCircle.x_begin; //Difference between end point and beginning point
	radius = (diff / 2.0) - 1; //get value of half of the difference
	midx = startCircle.x_begin + radius; //the center point
	midy = startCircle.y_begin + radius;
	
	
	rs = radius * radius;
	readyWrite=false;
	/*
	for(i = startCircle.x_begin+1; i< startCircle.x_end; i++){
			xs = (i - midx) * (i - midx);
				for(j = startCircle.y_begin+1; j< startCircle.y_end; j++){
					ys = (j-midy) * (j-midy);
					if(xs + ys <= rs && !readyWrite) 
					{ 
						LCD_SetPage(i, startCircle.x_end, ledNum);
						LCD_setColumn(j,startCircle.y_end,ledNum);
						write_cmd(0x2c, ledNum);
						readyWrite=true;
					}
					else if(xs+ys>rs && readyWrite){
						readyWrite=false;
						break;
					}
					
					if(readyWrite){
						write_dat2(startCircle.color, ledNum); 
					}
				}
			}
		*/
	
	for(j = startCircle.y_begin+1; j< startCircle.y_end; j++){
		ys = (j-midy) * (j-midy);
		for(i = startCircle.x_begin+1; i< startCircle.x_end; i++){
			xs = (i - midx) * (i - midx);

			if(xs + ys <= rs && !readyWrite) 
			{ 
				LCD_SetPage(j, startCircle.y_end, ledNum);
				LCD_setColumn(i,startCircle.x_end,ledNum);
				write_cmd(0x2c, ledNum);
				readyWrite=true;
			}
			else if(xs+ys>rs && readyWrite){
				readyWrite=false;
				break;
			}
			
			if(readyWrite){
				write_dat2(startCircle.color, ledNum); 
			}
		}
	}		
}

void draw_L(int ledNum){
	BtnData loser;
	
	loser.y_begin = 60; loser.y_end = 300;
	loser.x_begin = 80; loser.x_end = 100;
	loser.color = yellow;
	
	draw_rectangle(loser, ledNum); //draw left part of 'L'
	
	loser.y_begin = 280; loser.y_end = 300;
	loser.x_begin = 80; loser.x_end = 200;
	
	draw_rectangle(loser, ledNum); //draw bottom part of 'L'
}

void draw_W(int ledNum) {
	BtnData winner;
	
	winner.x_begin = 40; winner.x_end = 60;
	winner.y_begin = 60; winner.y_end = 300;
	winner.color = green;
	
	draw_rectangle(winner, ledNum); //draw left part of 'w'
	
	winner.x_begin = 60; winner.x_end = 180;
	winner.y_begin = 280; winner.y_end = 300;
	
	draw_rectangle(winner, ledNum); //draw base of 'w'
	
	winner.x_begin = 110; winner.x_end = 130;
	winner.y_begin = 200; winner.y_end = 280;
	
	draw_rectangle(winner, ledNum); //draw middle of 'w'
	
	winner.x_begin = 180; winner.x_end = 200;
	winner.y_begin = 60; winner.y_end = 300;
	
	draw_rectangle(winner, ledNum); //draw right part of 'w'
	
}
