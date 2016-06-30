#include <vector>
#include "Square.h"
#include "Minecounter.h"

using namespace std;

class Field {



public:


	short width, height;
	bool mine[MAX_WIDTH][MAX_HEIGHT];
	bool supermine[MAX_WIDTH][MAX_HEIGHT];
	Square allSquares[MAX_WIDTH][MAX_HEIGHT];

	int val3BV;
	unsigned int effectiveClicks;
	unsigned int ineffectiveClicks;

    vector<Square*> surroundingSquares(int squareX, int squareY, int range);
    vector<Square*> allSquaresV;

    Minecounter mc;

    Square isAroundThisSupermine(int squareX, int squareY);

	int calculate3BV();
	void floodFillMark(int [MAX_WIDTH][MAX_HEIGHT],int,int);
	void ffmProc(int [MAX_WIDTH][MAX_HEIGHT],int,int);

	bool isMine(int x, int y);
	bool isSupermine(int x, int y);

	void setMine(int x, int y);
	void setSupermine(int x, int y);

	void placeMines(int firstClickX, int firstClickY);
	void placeSupermines(int firstClickX, int firstClickY);

	bool isAroundSupermine(int squareX, int squareY);

	void init();

	void revealAround(int squareX, int squareY);

	void revealSquare(int squareX, int squareY);

	void squareClicked(int squareX, int squareY);

	bool adjacentMinesFlagged(int squareX,int squareY);

	int calculateRemainingMines();
	int calculateRemainingSupermines();

	void checkValues();

	void click(int, int, int);

	int get3BV();

	void viewClicks();

	float getGameProgress();
};
