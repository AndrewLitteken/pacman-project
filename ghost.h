/*
	ghost.h
	Prototype for the Ghost class for a game of PacMan
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include "pacman.h"

class Ghost{
	public:
		Ghost(Board&,int,int,int,char);
		~Ghost();
		bool checkSurround(Board&);
		bool checkDir(Board&,int,int);
		void moveDecision(Board& bo);
		void move(Board&,Pacman&);
		int getRow();
		int getCol();
		int getDir();
		void setRow(int);
		void setCol(int);
		void setEat(bool);
		bool getEat();
		void setPrev(char);
		char getPrev();
		void reset(Board&);
		void redraw(Board&, int, int);
		void setSwitch(bool);
		void switchCol();
	private:
		int row, col, dir, rowOrig, colOrig, dirOrig;
		bool canEat, switchColor;
		char prev;
		char name;
};