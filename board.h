/*
	board.h
	Prototype for the board class for a game of PacMan
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/
#include "gfxnew.h"

const int colSize = 29;
const int rowSize = 31;

class Board{
	public:
		Board();
		~Board();
		void changeSpace(int,int,char);
		char getSpace(int,int);
		int getDots();
		void print();
		void reset();
		void drawInitialBoard(int,int);
		void drawBoard(int,int);
	private:
		char board[rowSize][colSize];
		char boardPrev[rowSize][colSize];
		char boardOrig[rowSize][colSize];
		int dots, dotsOrig;
};
