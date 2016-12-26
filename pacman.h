/*
	pacman.h
	Prototype for the PacMan class for a game of PacMan
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include "board.h"

class Pacman{
	public:
		Pacman();
		Pacman(Board&);
		~Pacman();
		char move(Board&,char);
		bool checkDir(Board&,int,int);
		int getRow();
		int getCol();
		void setRow(int);
		void setCol(int);
		int getDir();
		void reset(Board&);
		void redraw(Board&,int,int);
	private:
		int row, col, dir, rowOrig, colOrig;;
};