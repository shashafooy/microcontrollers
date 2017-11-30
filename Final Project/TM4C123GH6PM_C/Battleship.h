#ifndef BTLSHIP_H
#define BTLSHIP_H
#include "Shapes.h"

#define XSIZE 6
#define YSIZE 7
typedef enum{water, ship, hit, miss, sunk} squareType;
typedef struct{
	int x, y;
}xycoor;
extern int p1xval, p1yval, p2xval, p2yval;

void initBoard(void);
BtnData getSquare(int i, int j, squareType a);
void draw_p1Ships_p2Map(void);
void draw_p2Ships_p1Map(void);
void drawGrid(int lcdNum);

//BtnData shipSquare(int, int, squareType); //gets the beginning and ending point of x and y for a ship
void highlightBorder(int xsquare, int ysquare, bool on, int lcdNum);
void start_game(void);
void next_turn(void);
void end_game(void);
int getXsquarePressed(int xval);
int getYsquarePressed(int yval);
bool tryCreateShip(int tempx, int tempy, int shipSize, int player);
void draw_ship_to_be_placed(int size, int ledNum);


#endif
