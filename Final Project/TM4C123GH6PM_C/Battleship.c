#include "Battleship.h"

squareType p1Ships[XSIZE][YSIZE], p1Map[XSIZE][YSIZE], p2Ships[XSIZE][YSIZE], p2Map[XSIZE][YSIZE];
int borderWidth = 2; //border width
int waterSqSize, //water square size
	shipSize;  

	

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
						,grey);
				}
			case hit:
				return temp;
			case miss:
				return temp;
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
			
			if(p1Ships[i][j]!=water)
				draw_rectangle(getSquare(i,j,p1Ships[i][j]),0);
			if(p2Map[i][j]!=water)
				draw_rectangle(getSquare(i,j,p2Map[i][j]),1);
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










