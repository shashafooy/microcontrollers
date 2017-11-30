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

void drawGrid(int lcdNum) {
	
	BtnData grid;
	
	grid.x_begin = 5; grid.x_end = 235;
	grid.y_begin = 5; grid.y_end = 315;
	grid.color = black;
	
	for(int i = 0; i < XSIZE+1; i++) {												//draw all of the horizontal gridlines
		grid.x_end = grid.x_begin + borderWidth;
		draw_rectangle(grid, lcdNum);
		grid.x_begin = grid.x_end + shipSize;
	}
	
	grid.x_begin = 5; grid.x_end = 235;
	
	for(int j = 0; j < YSIZE+1; j++) {												//draw all of the vertical gridlines
		grid.y_end = grid.y_begin + borderWidth;
		draw_rectangle(grid, lcdNum);
		grid.y_begin = grid.y_end + shipSize;
	}
	
	
}

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

xycoor p1Select, p2Select;

int gridbound_xBegin[XSIZE] = {
	-1,-1,-1,-1,-1,-1
};
int gridbound_xEnd[XSIZE] = {
	-1,-1,-1,-1,-1,-1
};
int gridbound_yBegin[YSIZE] = {
	-1,
	-1,
	-1,
	-1,
	-1,
	-1
};
int gridbound_yEnd[YSIZE] = {
	-1,
	-1,
	-1,
	-1,
	-1,
	-1
};


void initBoard(void){
	int i,j;
	waterSqSize	= (240 - 10 - borderWidth*(XSIZE+1)) / XSIZE;
	shipSize = waterSqSize - 5;
	clear_lcd(blue,0);
	clear_lcd(blue,1);
	/*draw grid*/
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
int getXsquarePressed(int xval){
	if(xval > gridbound_xBegin[0] && xval < gridbound_xEnd[0]) return 0;
	if(xval > gridbound_xBegin[1] && xval < gridbound_xEnd[1]) return 1;
	if(xval > gridbound_xBegin[2] && xval < gridbound_xEnd[2]) return 2;
	if(xval > gridbound_xBegin[3] && xval < gridbound_xEnd[3]) return 3;
	if(xval > gridbound_xBegin[4] && xval < gridbound_xEnd[4]) return 4;
	if(xval > gridbound_xBegin[5] && xval < gridbound_xEnd[5]) return 5;
	return -1; 
}
int getYsquarePressed(int yval){
	if(yval > gridbound_yBegin[0] && yval < gridbound_yEnd[0]) return 0;
	if(yval > gridbound_yBegin[1] && yval < gridbound_yEnd[1]) return 1;
	if(yval > gridbound_yBegin[2] && yval < gridbound_yEnd[2]) return 2;
	if(yval > gridbound_yBegin[3] && yval < gridbound_yEnd[3]) return 3;
	if(yval > gridbound_yBegin[4] && yval < gridbound_yEnd[4]) return 4;
	if(yval > gridbound_yBegin[5] && yval < gridbound_yEnd[5]) return 5;
	if(yval > gridbound_yBegin[6] && yval < gridbound_yEnd[6]) return 6;
	return -1; 
}


void start_game(void){
	int p1_ships_not_placed = 4;
	int p2_ships_not_placed = 4;
	int p1ShipSize = 3;
	int p2ShipSize = 3;
	bool p1HasSelection, p2HasSelection = false;
	int tempx,tempy;
	initBoard();
	
	/*draw example ship size below grid here*/
	
	
	while(1){
		if(p1_ships_not_placed == 2) p1ShipSize = 2;
		if(p2_ships_not_placed == 2) p2ShipSize = 2;
		while(p1xval <= 0 || p2xval <= 0); //wait until a player presses the screen
		if(p1xval>0){//p1 pressed screen
			tempx=getXsquarePressed(p1xval);
			tempy=getYsquarePressed(p1yval);
			if(tempy < 0 || tempx < 0) continue; //player pressed outside of square
			
			if(p1HasSelection){ //currently a selection
				if(p1Select.x == tempx && p1Select.y == tempy){ //undo selection
					p1Select.x = -1; p1Select.y = -1;							
					highlightBorder(p1Select.x,p1Select.y, false,0); //un highlight selected square
					p1HasSelection = false;
				}
				else if(tryCreateShip(tempx,tempy,p1ShipSize,0)){ //try to create a ship
					highlightBorder(p1Select.x,p1Select.y, false,0); //un highlight selected square
					p1HasSelection = false;
					p1_ships_not_placed--;
					}				
			}
			else{ //currently no selection
				p1Select.x=tempx;
				p1Select.y=tempy;
				highlightBorder(p1Select.x,p1Select.y, true,0); //highlight selected square
				p1HasSelection = true;
			}
		}else{ //p2 pressed screen
			tempx=getXsquarePressed(p2xval);
			tempy=getYsquarePressed(p2yval);
			if(tempy < 0 || tempx < 0) continue; //player pressed outside of square
			
			if(p2HasSelection){ //currently a selection
				if(p2Select.x == tempx && p2Select.y == tempy){ //undo selection
					p2Select.x = -1; p2Select.y = -1;							
					highlightBorder(p2Select.x,p2Select.y, false,1); //un highlight selected square
					p2HasSelection = false;
				}
				else if(tryCreateShip(tempx,tempy,p2ShipSize,1)){
					highlightBorder(p2Select.x,p2Select.y, false,1); //un highlight selected square
					p2HasSelection = false;
					p2_ships_not_placed--;
					}				
			}else{ //currently no selection
				p2Select.x=tempx;
				p2Select.y=tempy;
				highlightBorder(p2Select.x,p2Select.y, true,0); //highlight selected square
				p2HasSelection = true;
			}
		}
	}
}

/*********************************************
Check to see if the selected squares make the 
right sized ship. Draw the ship on the screen.
returns true if successfully created a ship
returns false if a ship was not created.
**********************************************/
bool tryCreateShip(int tempx, int tempy, int shipSize, int player){
	int i;
	if(p1Select.x ==tempx && p1Select.y == tempy-shipSize){
		for(i=tempy-shipSize; i<tempy; i++){
			p1Ships[tempx][i] = ship; //set ships
			draw_rectangle(getSquare(tempx,i,ship),player);
		}
		return true;
	}
	else if(p1Select.x ==tempx && p1Select.y == tempy+shipSize&& tempy+shipSize< 7){
		for(i=tempy; i<tempy+shipSize; i++){
			p1Ships[tempx][i] = ship; //set ships
			draw_rectangle(getSquare(tempx,i,ship),player);
		}
		return true;	
	}
	else if(p1Select.x ==tempx-shipSize && p1Select.y == tempy){
		for(i=tempx-shipSize; i<tempx; i++){
			p1Ships[i][tempy] = ship; //set ships
			draw_rectangle(getSquare(i,tempy,ship),player);
		}
		return true;	
	}
	else if(p1Select.x ==tempx+shipSize&& p1Select.y == tempy+shipSize&& tempx+3 < 6){
		for(i=tempx; i<tempx+shipSize; i++){
			p1Ships[i][tempy] = ship; //set ships
			draw_rectangle(getSquare(i,tempy,ship),player); //update square
		}
		return true;	
	}
	return false;
}


void next_turn(void){
}

void end_game(void){
}







