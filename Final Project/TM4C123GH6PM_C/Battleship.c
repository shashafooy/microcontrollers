#include "Battleship.h"


squareInfo p1Ships[XSIZE][YSIZE], p1Map[XSIZE][YSIZE], p2Ships[XSIZE][YSIZE], p2Map[XSIZE][YSIZE];
int borderWidth = 2; //border width
int waterSqSize, //water square size
	shipSize;  
bool p1Turn;
int p1ShipsAlive, p2ShipsAlive;
xycoor p1Select, p2Select;

int gridbound_xBegin[XSIZE] = {
	0x17b,0x352,0x567,0x7F2,0x9f6,0xc2e
};
int gridbound_xEnd[XSIZE] = {
	0x351,0x566,0x7f1,0x9f5,0xc2d,0xe54
};
int gridbound_yBegin[YSIZE] = {
	0x17b,
	0x332,
	0x4A5,
	0x615,
	0x7A8,
	0x94D,
	0xAfd
};
int gridbound_yEnd[YSIZE] = {
	0x331,
	0x4a4,
	0x614,
	0x7a7,
	0x94c,
	0xafc,
	0xcaf
};

/*******************************************************
This function delays the game for given seconds so the user can see if their selection was a hit or miss
*******************************************************/
void delay(int sec) {
	WTIMER0->CTL = 0x0; //disable timer
	WTIMER0->TAILR = 80000000*sec; // (load+1)/80MHz= 2 secs
	WTIMER0->CTL |= 0x1; //enable timer
	while(WTIMER0->TAV > 10); // Wait for timer to finish
}

/****************************************************
This function draws a Dynamic grid of size
XSIZE by YSIZE
*****************************************************/
void drawGrid(int lcdNum) {
	int i,j;
	BtnData grid;
	
	grid.x_begin = 5; grid.x_end = 235;
	grid.y_begin = 5; grid.y_end = YSIZE*waterSqSize + (YSIZE+1)*(borderWidth)+ 5;
	grid.color = black;
	
	for(i = 0; i < XSIZE+1; i++) {												//draw all of the horizontal gridlines
		grid.x_end = grid.x_begin + (borderWidth);
		draw_rectangle(grid, lcdNum);
		grid.x_begin = grid.x_end + waterSqSize;
	}
	
	grid.x_begin = 5; grid.x_end = 235;
	
	for(j = 0; j < YSIZE+1; j++) {												//draw all of the vertical gridlines
		grid.y_end = grid.y_begin + (borderWidth);
		draw_rectangle(grid, lcdNum);
		grid.y_begin = grid.y_end + waterSqSize;
	}
	
	
}

/*************************************************
This function highlights the selected square. It will change
the borders to magenta or black depending on parameter on.
*************************************************/
void highlightBorder(int i, int j, bool on, int lcdNum) {   //Function to highlight the selected square for ship placement
	
	BtnData border, currRect;
	
	border.x_begin = 5+borderWidth*i+waterSqSize*i;
	border.x_end = 5+(borderWidth*(i+2))+((waterSqSize)*(i+1));
	border.y_begin = 5+borderWidth*j+waterSqSize*j;
	border.y_end = 5+(borderWidth*(j+2))+((waterSqSize)*(j+1));
	
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



/**********************************************
Initalize the board to a blue background with 
a X by Y grid. Initialize Ships and Maps to water.
**********************************************/
void initBoard(void){	
	int i,j;
	waterSqSize	= (240 - 10 - borderWidth*(XSIZE+1)) / XSIZE;
	shipSize = waterSqSize - 10;
	clear_lcd(blue,0);
	clear_lcd(blue,1);
	drawGrid(0);
	drawGrid(1);
	
	for(i=0;i<XSIZE;i++){
		for(j=0;j<YSIZE;j++){
			p1Ships[i][j].type = water;
			p1Map[i][j].type = water;
			p2Ships[i][j].type = water;
			p2Map[i][j].type = water;
		}
	}
}



/***************************************************
Creates a button at a given location of the map given array index
and square type. Dynamic button sizes are used depending on map size.
Creates buttons for: water, ship, hit, miss, sunk.
***************************************************/
BtnData getSquare(int i, int j, squareType a){
	/****************TODO**********************
	*create button location for hit and miss(round button?)
	******************************************/
	BtnData temp;
	switch(a){
			case water: 
				return BtnData_new(5+borderWidth*(i+1)+waterSqSize*i
					,5+(borderWidth+waterSqSize)*(i+1)
					,5+borderWidth*(j+1)+waterSqSize*j
					,5+(borderWidth + waterSqSize)*(j+1)
					,blue);	
			case ship:
				{
					//blue border of 5 pixels
					//functions for x and y location used to allow different number of squares
					int sizeDiff = (waterSqSize - shipSize)/2;
					//external border + water between ship and border + border + ship size
					
					return BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+1)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*j+1)+borderWidth*(j+1)+shipSize*j
						,5+sizeDiff*(2*j+1)+(borderWidth + shipSize)*(j+1)
						, grey);
					
				}
			case hit:
				{
					//blue border of 5 pixels
					//functions for x and y location used to allow different number of squares
					int sizeDiff = (waterSqSize - shipSize)/2;
					//external border + water between ship and border + border + ship size
					
					return BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+1)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*j+1)+borderWidth*(j+1)+shipSize*j
						,5+sizeDiff*(2*j+1)+(borderWidth + shipSize)*(j+1)
						, red);
					
				}
			case miss:
				{
					//blue border of 5 pixels
					
					int sizeDiff = (waterSqSize - shipSize)/2;
					//external border + water between ship and border + border + ship size
					
					return BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+1)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*j+1)+borderWidth*(j+1)+shipSize*j
						,5+sizeDiff*(2*j+1)+(borderWidth + shipSize)*(j+1)
						, white);
				}
			case sunk:
			{
				int sizeDiff = (waterSqSize - shipSize)/2;
					//external border + water between ship and border + border + ship size
					
					return BtnData_new(5+sizeDiff*(2*i+1)+(borderWidth)*(i+1)+shipSize*i
						,5+sizeDiff*(2*i+1)+(borderWidth+shipSize)*(i+1)
						,5+sizeDiff*(2*j+1)+borderWidth*(j+1)+shipSize*j
						,5+sizeDiff*(2*j+1)+(borderWidth + shipSize)*(j+1)
						, red);
			}
			default: break;
		}
	return temp;
}


/*************************************************

*************************************************/
void draw_p1Ships_p2Map(void){
	int i,j;
	//draw blank blue squares
	for(i=0;i<XSIZE;i++){
		for(j=0;j<YSIZE;j++){
			//draw water square 
			BtnData btn = getSquare(i,j,water);
			draw_rectangle(btn, 0);
			draw_rectangle(btn, 1);
			
			if(p1Ships[i][j].type==ship){ 														//if player1 square is a ship that has not been hit, display the ship
				draw_rectangle(getSquare(i,j,p1Ships[i][j].type),0);
				}
			else if(p1Ships[i][j].type == hit) {											//if player1 square is a ship that has been hit, display ship with a red circle overlay
				draw_rectangle(getSquare(i,j,ship),0);
				draw_circle(getSquare(i,j,p1Ships[i][j].type), 0);
			}
			else if(p1Ships[i][j].type == miss) {										//if player2 has missed a shot, display a miss on player1 screen
				draw_circle(getSquare(i,j,p1Ships[i][j].type),0);
			}
			else if(p1Ships[i][j].type == sunk) {
				draw_rectangle(getSquare(i,j,p1Ships[i][j].type), 0);
			}
			if(p2Map[i][j].type== miss) 
				draw_circle(getSquare(i, j, miss), 1);						//if player2 has missed the current square, display water with a white circle
			else if(p2Map[i][j].type == hit)
				draw_circle(getSquare(i, j, p1Ships[i][j].type), 1);						//if player 2 has hit the current square, display water with a red circle
			else if(p2Map[i][j].type == sunk) 
				draw_rectangle(getSquare(i, j, p1Ships[i][j].type), 1);			//if player 2 has sunk a ship, the ship squares will be solid red
		}
	}
	draw_rectangle(BtnData_new(0,240,300,320,black),0); //black border on bottom, p2 turn
	draw_rectangle(BtnData_new(0,240,300,320,green),1); //p2 turn
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
			
			
			if(p2Ships[i][j].type==ship){ 														//if player1 square is a ship that has not been hit, display the ship
				draw_rectangle(getSquare(i,j,p2Ships[i][j].type),1);
				}
			else if(p2Ships[i][j].type == hit) {											//if player1 square is a ship that has been hit, display ship with a red circle overlay
				draw_rectangle(getSquare(i,j,ship),1);
				draw_circle(getSquare(i,j,p2Ships[i][j].type), 1);
			}
			else if(p2Ships[i][j].type == miss) {										//if player2 has missed a shot, display a miss on player1 screen
				draw_circle(getSquare(i,j,p2Ships[i][j].type),1);
			}
			else if(p2Ships[i][j].type == sunk) {
				draw_rectangle(getSquare(i,j,p2Ships[i][j].type), 1);
			}
			if(p1Map[i][j].type == miss) 
				draw_circle(getSquare(i, j, miss), 0);						//if player2 has missed the current square, display water with a white circle
			else if(p1Map[i][j].type == hit)
				draw_circle(getSquare(i, j, hit), 0);						//if player 2 has hit the current square, display water with a red circle
			else if(p1Map[i][j].type == sunk) 
				draw_rectangle(getSquare(i, j, sunk), 0);			//if player 2 has sunk a ship, the ship squares will be solid red
			/*
			if(p2Ships[i][j].type!=water)
				draw_rectangle(getSquare(i,j,p2Ships[i][j].type),0);
			if(p1Map[i][j].type!=water)
				draw_rectangle(getSquare(i,j,p1Map[i][j].type),1);
			*/
		}
	}
	draw_rectangle(BtnData_new(0,240,300,320,black),1); //black border on bottom, p1 turn
	draw_rectangle(BtnData_new(0,240,300,320,green),0); //p1 turn
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
	bool p1HasSelection = false, p2HasSelection = false;
	int tempx,tempy;
	bool p1ready = false, p2ready=false;
	BtnData btn;
	
	clear_lcd(grey,0);
	clear_lcd(grey,1);
	btn = BtnData_new(30,210,100,148,grey);
	DrawString("Start",30,100,0,6);
	DrawString("Start",30,100,1,6);


	while(!p1ready || !p2ready){
		while(p1xval <= 0 && p2xval <= 0); //wait until a player presses the screen
			if(p1xval>0){
				if(p1xval>0x2a0 && p1xval<0xccd && p1yval>0x520 && p1yval<0x72b){
					draw_rectangle(btn,0);
					p1ready=true;
				}
			}else{
				if(p2xval>0x2a0 && p2xval<0xccd && p2yval>0x520 && p2yval<0x72b){
					draw_rectangle(btn,1);
					p2ready=true;
				}
			}
			p1xval = p1yval = p2xval = p2yval = 0;
	}
		
	initBoard();
	
	draw_ship_to_be_placed(p1ShipSize,0);
	draw_ship_to_be_placed(p2ShipSize,1);
	
	while(p1_ships_not_placed > 0 || p2_ships_not_placed > 0){
		while(p1xval <= 0 && p2xval <= 0); //wait until a player presses the screen
		if(p1xval>0 && p1_ships_not_placed > 0){//p1 pressed screen
			tempx=getXsquarePressed(p1xval);
			tempy=getYsquarePressed(p1yval);
			p1xval = p1yval = 0;
			if(tempy < 0 || tempx < 0) continue; //player pressed outside of square
			
			if(p1HasSelection){ //currently a selection
				if(p1Select.x == tempx && p1Select.y == tempy){ //undo selection
					highlightBorder(p1Select.x,p1Select.y, false,0); //un highlight selected square
					p1Select.x = -1; p1Select.y = -1;							
					p1HasSelection = false;
				}
				else if(tryCreateShip(tempx,tempy,p1ShipSize,0,p1Select)){ //try to create a ship
					highlightBorder(p1Select.x,p1Select.y, false,0); //un highlight selected square
					p1HasSelection = false;
					p1_ships_not_placed--;
					
					if(p1_ships_not_placed == 2){ 
						p1ShipSize = 2;
						draw_ship_to_be_placed(p1ShipSize,0); //draw 2 squares at bottom of screen
					}else if(p1_ships_not_placed == 0) draw_ship_to_be_placed(0,0);
				}				
			}
			else{ //currently no selection
				p1Select.x=tempx;
				p1Select.y=tempy;
				if(p1Ships[tempx][tempy].type!=ship){
					highlightBorder(p1Select.x,p1Select.y, true,0); //highlight selected square
					p1HasSelection = true;
				}
			}
		}else if(p2xval>0 && p2_ships_not_placed > 0){ //p2 pressed screen
			tempx=getXsquarePressed(p2xval);
			tempy=getYsquarePressed(p2yval);
			p2xval = p2yval = 0;
			if(tempy < 0 || tempx < 0) continue; //player pressed outside of square
			
			if(p2HasSelection){ //currently a selection
				if(p2Select.x == tempx && p2Select.y == tempy){ //undo selection
					highlightBorder(p2Select.x,p2Select.y, false,1); //un highlight selected square
					p2Select.x = -1; p2Select.y = -1;							
					p2HasSelection = false;
				}
				else if(tryCreateShip(tempx,tempy,p2ShipSize,1,p2Select)){
					highlightBorder(p2Select.x,p2Select.y, false,1); //un highlight selected square
					p2HasSelection = false;
					p2_ships_not_placed--;
					if(p2_ships_not_placed == 2){ 
						p2ShipSize = 2;
						draw_ship_to_be_placed(p2ShipSize,1); //draw 2 squares at bottom of screen
					}else if(p2_ships_not_placed == 0) draw_ship_to_be_placed(0,1);
				}				
			}else{ //currently no selection				
				p2Select.x=tempx;
				p2Select.y=tempy;
				if(p2Ships[tempx][tempy].type!=ship){
					highlightBorder(p2Select.x,p2Select.y, true,1); //highlight selected square
					p2HasSelection = true;
				}
			}
		}
	}
	drawGrid(0); //redraw in case something doesn't un highlight
	drawGrid(1);
}

/*********************************************
Check to see if the selected squares make the 
right sized ship. Draw the ship on the screen.
returns true if successfully created a ship
returns false if a ship was not created.
**********************************************/
bool tryCreateShip(int tempx, int tempy, int shipLength, int player, xycoor selection){
	int i;
	shipLength--;
	if(selection.x ==tempx && selection.y == tempy-(shipLength)){ //vertical
		for(i=tempy-shipLength; i<=tempy; i++){
			orientation dir;
			if(i==tempy-shipLength) dir = down;
			else if(i==tempy) dir = up;
			else dir = vertical;
			initShip((i==tempy-shipLength||i==tempy),tempx,i,dir,shipLength+1,player);
			draw_rectangle(getSquare(tempx,i,ship),player);
		}
		return true;
	}
	else if(selection.x ==tempx && selection.y == tempy+(shipLength)&& tempy+(shipLength)< 7){ //vertical
		for(i=tempy; i<=tempy+shipLength; i++){
			orientation dir;
			if(i==tempy+shipLength) dir = up;
			else if(i==tempy) dir = down;
			else dir = vertical;
			initShip((i==tempy+shipLength || i== tempy),tempx,i,dir,shipLength+1,player);
			draw_rectangle(getSquare(tempx,i,ship),player);
		}
		return true;	
	}
	else if(selection.x ==tempx-(shipLength) && selection.y == tempy){ //horizontal
		for(i=tempx-shipLength; i<=tempx; i++){
			orientation dir;
			if(i==tempx-shipLength) dir = right;
			else if(i==tempx) dir = left;
			else dir = horizontal;
			initShip((i==tempx-shipLength||i==tempx),i,tempy,dir,shipLength+1,player);
			draw_rectangle(getSquare(i,tempy,ship),player);
		}
		return true;	
	}
	else if(selection.x ==tempx+(shipLength)&& selection.y == tempy&& tempx+(shipLength) < 6){ //horizontal
		for(i=tempx; i<=tempx+shipLength; i++){
			orientation dir;
			if(i==tempx+shipLength) dir = left;
			else if(i==tempx) dir = right;
			else dir = horizontal;
			initShip((i==tempx||i==tempx+shipLength),i,tempy,dir,shipLength+1,player);
			draw_rectangle(getSquare(i,tempy,ship),player); //update square
		}
		return true;	
	}
	return false;
}

/**************
creates a ship with given attributes
***************/
void initShip(bool is_edge, int row, int col, orientation dir, int shipLength, int player){
	if(player == 0){
		p1Ships[row][col].s_section = (is_edge)?edge:center;
		p1Ships[row][col].shipHP=shipLength;
		p1Ships[row][col].shipSize = shipLength;
		p1Ships[row][col].dir = dir;
		p1Ships[row][col].type = ship;
	}else{
		p2Ships[row][col].s_section = (is_edge)?edge:center;
		p2Ships[row][col].shipHP=shipLength;
		p2Ships[row][col].shipSize=shipLength;
		p2Ships[row][col].dir = dir;
		p2Ships[row][col].type = ship; //set ships
	}
}

void next_turn(void){
	int x, y;
	if(p1Turn){
		draw_p2Ships_p1Map();
	}else{
		draw_p1Ships_p2Map();
	}	
	while(1){
		while((p1Turn)?p1xval<= 0:p2xval <= 0); //wait until correct player presses the screen
		x=getXsquarePressed((p1Turn)?p1xval:p2xval);
		y=getYsquarePressed((p1Turn)?p1yval:p2yval);
		p1xval = p1yval = p2xval = p2yval = 0;
		if(x < 0 || y < 0) continue; //player pressed outside of square
		if(((p1Turn)?p1Map[x][y].type:p2Map[x][y].type) != water) continue;
		//player has now selected a valid empty square
		if(((p1Turn)?p2Ships[x][y].type : p1Ships[x][y].type) != ship){ // miss
			updateSquare(x,y,miss,!p1Turn,false); //update pMap
			updateSquare(x,y,miss,p1Turn,true); //update pShip
		}
		else{ //hit			
			xycoor selection;
			selection.x=x;
			selection.y=y;
			hitShip(selection, p1Turn);
			trySinkShip(selection, p1Turn);
		}
		return;
	}
}

void end_game(void){
	if(p1ShipsAlive==0){
		draw_L(0);
		draw_W(1);
	}else{
		draw_W(0);
		draw_L(1);
	}
	draw_rectangle(BtnData_new(0,240,300,320,blue),0); //clear borders
	draw_rectangle(BtnData_new(0,240,300,320,blue),1); 
	
	delay(5);
	
	DrawString("Restart",15,200,1,5);
	DrawString("Restart",15,200,0,5);

	while(1){
		while(p1xval <= 0 && p2xval <= 0); //wait until a player presses the screen
			if(p1xval>0){
				if(p1xval>0x1d0 && p1xval<0xdc3 && p1yval>0x964 && p1yval<0xb2f){
					run();
				}
			}else{
				if(p2xval>0x1d0 && p2xval<0xdc3 && p2yval>0x964 && p2yval<0xb2f){
					run();
				}
			}
			p1xval = p1yval = p2xval = p2yval = 0;
	
	}
	
}

void draw_ship_to_be_placed(int size, int ledNum){
	int i;
	draw_rectangle(BtnData_new(50,50+shipSize*(2+1) + 3*2,
			YSIZE*waterSqSize + (YSIZE+1)*borderWidth + 5 +10,YSIZE*waterSqSize + (YSIZE+1)*borderWidth + 5 +10+shipSize, blue)
		,ledNum); //draw water squares
	for(i=0; i<size; i++){ //draw ship to be placed
	BtnData t = BtnData_new(50+shipSize*i + 3*i,50+shipSize*(i+1) + 3*i,
		YSIZE*waterSqSize + (YSIZE+1)*borderWidth + 5 +10,YSIZE*waterSqSize + (YSIZE+1)*borderWidth + 5 +10+shipSize, grey);
	draw_rectangle(t,ledNum);
	}
}

void run(void){
	bool winner;
	winner=false;
	p1Turn = true;
	p1ShipsAlive = p2ShipsAlive = 4;
	
	start_game();
	while(!winner){
		next_turn();
		p1Turn = !p1Turn; //switch player turn
		 //how long to delay in seconds
		if(p1ShipsAlive == 0 || p2ShipsAlive==0) winner=true;
		else delay(3);
	}
	end_game();
	while(1);
}

//player passed is the player getting hit
void hitShip(xycoor select,int player){
	//decrement ship HP for ships next to selection
		int x,y,i,shipLength;
		bool p;
		x=select.x;
		y=select.y;
		p = player;
		shipLength=(player==0)?p1Ships[x][y].shipSize : p2Ships[x][y].shipSize;
		
		updateSquare(x,y,hit,p,true);
		updateSquare(x,y,hit,!p,false);
	
		switch((player==0)?p1Ships[x][y].dir : p2Ships[x][y].dir){
		case up:
			for(i=y; i>y-shipLength; i--){
				if(p) p2Ships[x][i].shipHP--;
				else p1Ships[x][i].shipHP--;
			}
			break;
		case down:
			for(i=y; i<y+shipLength; i++){
				if(p) p2Ships[x][i].shipHP--;
				else p1Ships[x][i].shipHP--;
			}
			break;
		case left:
			for(i=x; i>x-shipLength; i--){
				if(p) p2Ships[i][y].shipHP--;
				else p1Ships[i][y].shipHP--;
			}
			break;
		case right:
			for(i=x; i<x+shipLength; i++){
				if(p) p2Ships[i][y].shipHP--;
				else p1Ships[i][y].shipHP--;
			}
			break;
		case horizontal:
			for(i=x-1; i<=x+1; i++){
				if(p) p2Ships[i][y].shipHP--;
				else p1Ships[i][y].shipHP--;
			}
			break;
		case vertical:
			for(i=y-1; i<=y+1; i++){
				if(p) p2Ships[x][i].shipHP--;
				else p1Ships[x][i].shipHP--;
			}
			break;
		default: break;
		
		}
}

void trySinkShip(xycoor select, int player){
	//check if 0 HP-> change ship status to sunk, update squares, decrement ships alive
	int i,x,y,shipLength;
	bool p;
	x=select.x;
	y=select.y;
	p = player;
	shipLength=(player==0)?p1Ships[x][y].shipSize:p2Ships[x][y].shipSize;
	if(player ==0){ //p1
		if(p1Ships[x][y].shipHP != 0) return;
		p1ShipsAlive--;
	}else{
		if(p2Ships[x][y].shipHP != 0) return;
		p2ShipsAlive--;
	}
	switch((player==0)?p1Ships[x][y].dir : p2Ships[x][y].dir){
		case up:
			for(i=y; i>y-shipLength; i--){
				updateSquare(x,i,sunk,p,true);
				updateSquare(x,i,sunk,!p,false);
			}
			break;
		case down:
			for(i=y; i<y+shipLength; i++){
				updateSquare(x,i,sunk,p,true);
				updateSquare(x,i,sunk,!p,false);
			}
			break;
		case left:
			for(i=x; i>x-shipLength; i--){
				updateSquare(i,y,sunk,p,true);
				updateSquare(i,y,sunk,!p,false);
			}
			break;
		case right:
			for(i=x; i<x+shipLength; i++){
				updateSquare(i,y,sunk,p,true);
				updateSquare(i,y,sunk,!p,false);
			}
			break;
		case horizontal:
			for(i=x-1; i<=x+1; i++){
				updateSquare(i,y,sunk,p,true);
				updateSquare(i,y,sunk,!p,false);
			}
			break;
		case vertical:
			for(i=y-1; i<=y+1; i++){
				updateSquare(x,i,sunk,p,true);
				updateSquare(x,i,sunk,!p,false);
			}
			break;
		default: break;
		
		}
}
/********************
updates a specific player's ship or hit map
ships true - update ships
ships false - update map
********************/
void updateSquare(int x, int y, squareType type, int player, bool ships){
		if(player==0&&ships)	
			p1Ships[x][y].type=type;
		else if(player==0&&!ships)	
			p1Map[x][y].type=type;
		else if(player==1&&ships) 
			p2Ships[x][y].type=type;
		else 
			p2Map[x][y].type=type;
		
		if(type == hit || type == miss) draw_circle(getSquare(x,y,type),player);
		else draw_rectangle(getSquare(x,y,type),player);
	
}
