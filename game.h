/*
	game.h
	Prototype for the game class
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include "ghost.h"
#include <vector>
using namespace std;

class Game{
	public:
		Game();
		void play();
		void reset();
		void drawBoard(int,int);
		void pacPrey(Ghost&);
		void ghostPrey(Ghost&);
		void updateScore();
		char endScreen(char*);
	private:
		Board bo;
		Board prevBo;
		Pacman p;
		vector<Ghost> ghosts;
		int lives, numDots, score, winw, winh, sw, sh;
		bool gameRun, lifeLost;
};