#ifndef BTLSHIP_H
#define BTLSHIP_H
#include "Shapes.h"

#define XSIZE 6
#define YSIZE 7 
typedef enum{water, ship, hit, miss} squareType;
typedef struct{
	int x, y;
}xycoor;
extern int p1xval, p1yval, p2xval, p2yval;

void initBoard(void);
BtnData getSquare(int i, int j, squareType a);
void draw_p1Ships_p2Map(void);
void draw_p2Ships_p1Map(void);

void start_game(void);
void next_turn(void);
void end_game(void);
int getXsquarePressed(int xval);
int getYsquarePressed(int yval);
void highlightBorder(int xsquare, int ysquare, bool on, int ledNum);
bool tryCreateShip(int tempx, int tempy, int shipSize, int player);








#endif
