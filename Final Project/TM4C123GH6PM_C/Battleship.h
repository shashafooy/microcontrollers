#ifndef BTLSHIP_H
#define BTLSHIP_H
#include "Shapes.h"

#define XSIZE 6
#define YSIZE 7
typedef enum{water, ship, hit, miss, sunk} squareType;

void initBoard(void);
BtnData getSquare(int i, int j, squareType a);
void draw_p1Ships_p2Map(void);
void draw_p2Ships_p1Map(void);

//BtnData shipSquare(int, int, squareType); //gets the beginning and ending point of x and y for a ship
void highlightBorder(int xsquare, int ysquare, bool on, int lcdNum);







#endif
