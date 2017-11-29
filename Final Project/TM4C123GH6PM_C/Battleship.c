#include "Battleship.h"

squareType p1Ships[XSIZE][YSIZE], p1Map[XSIZE][YSIZE], p2Ships[XSIZE][YSIZE], p2Map[XSIZE][YSIZE];
int borderWidth = 2; //border width
int waterSqSize, //water square size
	shipSize;  

/*
BtnData shipSquare(int i, int j, squareType status) { 
	int sizeDiff = waterSqSize - shipSize;
	BtnData ship;
	
	ship.x_begin= 5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i; //gets the beginning x value to draw ship
	ship.x_end = 5+sizeDiff*(2*i+2)+(borderWidth+shipSize)*(i+1);    //gets the ending x value to draw ship
	ship.y_begin = 5+sizeDiff*(2*i+1)+borderWidth*(j+1)+waterSqSize*j;  //gets the beginning y value to draw ship
	ship.y_end = 5+sizeDiff*(2*i+2)+(borderWidth + waterSqSize)*(j+1);  //gets the ending y value to draw ship
				
	if(status == hit) ship.color = red;						//bool: 1 is a hit (red), 0 is a ship which has not been hit (grey)
	else if(status == miss)ship.color = white;
	else ship.color = grey;
	
	return ship;
}
*/

void highlightBorder(int i, int j, bool on, int lcdNum) {   //Function to highlight the selected square for ship placement
	
	BtnData border, currRect;
	
	border.x_begin = 5+borderWidth*i+waterSqSize*i;
	border.x_end = 5+borderWidth*(i+2)+(waterSqSize)*(i+1);
	border.y_begin = 5+borderWidth*j+waterSqSize*j;
	border.x_end = 5+borderWidth*(j+2)+(waterSqSize)*(j+1);
	
	border.color = on?magenta:black; //if on, magenta, else black;
	
	currRect = border;
	currRect.y_end = border.y_begin + borderWidth;
	draw_rectangle(currRect, lcdNum); //draw the top border 
	
	currRect = border; 
	currRect.x_begin = border.x_end - borderWidth;
	draw_rectangle(currRect, lcdNum); //draw the right border
	
	currRect = border;
	currRect.y_begin = border.y_end - borderWidth;
	draw_rectangle(currRect, lcdNum); //draw the bottom border
	
	currRect = border;
	currRect.x_end = border.x_begin + borderWidth;
	draw_rectangle(currRect, lcdNum); //draw the left border 
	
}

void initBoard(void){
	int i,j;
	waterSqSize	= (240 - 10 - borderWidth*(XSIZE+1)) / XSIZE;
	shipSize = waterSqSize - 5;
	for(i=0;i<XSIZE;i++){
		for(j=0;j<YSIZE;j++){
			p1Ships[i][j] = water;
			p1Map[i][j] = water;
			p2Ships[i][j] = water;
			p2Map[i][j] = water;
		}
	}
}

BtnData getSquare(int i, int j, squareType a){
	/****************TODO**********************
	*create button location for hit and miss(round button?)
	******************************************/
	BtnData temp;
	switch(a){
			case water: 
				return *BtnData_new(5+borderWidth*(i+1)+waterSqSize*i
					,5+(borderWidth+waterSqSize)*(i+1)
					,5+borderWidth*(j+1)+waterSqSize*j
					,5+(borderWidth + waterSqSize)*(j+1)
					,blue);	
			case ship:
				{
					//blue border of 5 pixels
					//functions for x and y location used to allow different number of squares
					int sizeDiff = waterSqSize - shipSize;
					//external border + water between ship and border + border + ship size
					
					return *BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+2)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*i+1)+borderWidth*(j+1)+waterSqSize*j
						,5+sizeDiff*(2*i+2)+(borderWidth + waterSqSize)*(j+1)
						, grey);
					
				}
			case hit:
				{
					//blue border of 5 pixels
					//functions for x and y location used to allow different number of squares
					int sizeDiff = waterSqSize - shipSize;
					
					return *BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+2)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*i+1)+borderWidth*(j+1)+waterSqSize*j
						,5+sizeDiff*(2*i+2)+(borderWidth + waterSqSize)*(j+1)
						, red);
					
				}
			case miss:
				{
					//blue border of 5 pixels
					
					int sizeDiff = waterSqSize - shipSize;
					
					return *BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+2)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*i+1)+borderWidth*(j+1)+waterSqSize*j
						,5+sizeDiff*(2*i+2)+(borderWidth + waterSqSize)*(j+1)
						, white);
				}
			case sunk:
			{
				//blue border of 5 pixels with a solid red square for a sunk ship
				int sizeDiff = waterSqSize - shipSize;
					//external border + water between ship and border + border + ship size
					
					return *BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+2)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*i+1)+borderWidth*(j+1)+waterSqSize*j
						,5+sizeDiff*(2*i+2)+(borderWidth + waterSqSize)*(j+1)
						, grey);
			}
			default: break;
		}
	return temp;
}

void draw_p1Ships_p2Map(void){
	int i,j;
	//draw blank blue squares
	for(i=0;i<XSIZE;i++){
		for(j=0;j<YSIZE;j++){
			//draw water square 
			BtnData btn = getSquare(i,j,water);
			draw_rectangle(btn, 0);
			draw_rectangle(btn, 1);
			
			if(p1Ships[i][j]==ship){ 														//if player1 square is a ship that has not been hit, display the ship
				draw_rectangle(getSquare(i,j,p1Ships[i][j]),0);
				}
			else if(p1Ships[i][j] == hit) {											//if player1 square is a ship that has been hit, display ship with a red circle overlay
				draw_rectangle(getSquare(i,j,p1Ships[i][j]),0);
				draw_circle(getSquare(i,j,p1Ships[i][j]), 0);
			}
			else if(p1Ships[i][j] == miss) {										//if player2 has missed a shot, display a miss on player1 screen
				draw_circle(getSquare(i,j,p1Ships[i][j]),0);
			}
			else if(p1Ships[i][j] == sunk) {
				draw_rectangle(getSquare(i,j,p1Ships[i][j]), 0);
			}
			if(p2Map[i][j]== miss) 
				draw_circle(getSquare(i, j, miss), 1);						//if player2 has missed the current square, display water with a white circle
			else if(p2Map[i][j] == hit)
				draw_circle(getSquare(i, j, p1Ships[i][j]), 1);						//if player 2 has hit the current square, display water with a red circle
			else if(p2Map[i][j] == sunk) //****ADD A SUNK shipType****//
				draw_rectangle(getSquare(i, j, p1Ships[i][j]), 1);			//if player 2 has sunk a ship, the ship squares will be solid red
		}
	}
}

void draw_p2Ships_p1Map(void){
	int i,j;
	//draw blank blue squares
	for(i=0;i<XSIZE;i++){
		for(j=0;j<YSIZE;j++){
			//draw water square 
			BtnData btn = getSquare(i,j,water);
			draw_rectangle(btn, 0);
			draw_rectangle(btn, 1);
			
			if(p2Ships[i][j]!=water)
				draw_rectangle(getSquare(i,j,p2Ships[i][j]),0);
			if(p1Map[i][j]!=water)
				draw_rectangle(getSquare(i,j,p1Map[i][j]),1);
		}
	}
}











