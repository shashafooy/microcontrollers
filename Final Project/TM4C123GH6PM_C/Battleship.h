#ifndef BTLSHIP_H
#define BTLSHIP_H
#include "Shapes.h"
#include "Font.h"

#define XSIZE 6
#define YSIZE 7
typedef enum{water, ship, hit, miss, sunk} squareType;
typedef enum{edge, center} shipSection;
typedef enum{vertical, horizontal, up, down, left, right} orientation;

typedef struct{
	squareType type;
	shipSection s_section;
	orientation dir;
	int shipHP;
	int shipSize;
	bool beenHit;
}squareInfo;

typedef struct{
	int x, y;
}xycoor;
extern int p1xval, p1yval, p2xval, p2yval;

void initBoard(void);
BtnData getSquare(int i, int j, squareType a);
void draw_p1Ships_p2Map(void);
void draw_p2Ships_p1Map(void);
void drawGrid(int lcdNum);
void highlightBorder(int xsquare, int ysquare, bool on, int lcdNum);
void draw_ship_to_be_placed(int size, int ledNum);
void updateSquare(int x, int y, squareType type, int player, bool ships);

//BtnData shipSquare(int, int, squareType); //gets the beginning and ending point of x and y for a ship

void run(void);
void start_game(void);
void next_turn(void);
void end_game(void);
int getXsquarePressed(int xval);
int getYsquarePressed(int yval);
bool tryCreateShip(int tempx, int tempy, int shipSize, int player, xycoor selection);
void initShip(bool is_edge, int row, int col, orientation dir, int shipLength, int player);
void hitShip(xycoor selection,int player);
void trySinkShip(xycoor selection, int player);





#endif
