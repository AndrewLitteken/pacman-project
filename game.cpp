/*
	game.cpp
	Functions for the game class
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include "gfxnew.h"
#include <string>
using namespace std;

#include "game.h"

Game::Game(){ // Set up the game
	numDots = bo.getDots(); // set the number of dots based on the board
	bo.changeSpace(p.getRow(),p.getCol(),'P');
	Ghost g1(bo, rowSize-2, colSize-2, 2, 'G'); // make pacman and ghosts
	Ghost g2(bo, 1, 1, 2, 'G');
	Ghost g3(bo, 1, colSize-2, 2, 'G');
	Ghost g4(bo, rowSize-2, 1, 2, 'G');
	ghosts = {g1, g2, g3, g4};
	lives = 3; // Set initial lives and score
	score = 0;
	bool lifeLost = false; // flags for later
	bool gameRun = true;
	winh = 600, winw = 600; // set the game width and height
	sw=600, sh = 700; // Set the game and width heigh of the window
	gfx_open(sw,sh,"PACMAN"); // open gfx window
}

void Game::play(){ // This is the game itself
	char font1[] = "12x24";
	gfx_changefont(font1);
	gfx_text(sw/2-40,40,"PAC-MAN"); // Write Pacman at tope
	char font2[] = "7x13eurobold";
	gfx_changefont(font2);
	string base = "Lives: ";
	string lifetext = base+to_string(lives);
	gfx_text(10,winh+60,lifetext.c_str()); // Write the lives in bottom left corner
	string base2 = "Score: ";
	string scoreText = base2+to_string(score);
	gfx_text(sw-100,winh+60,scoreText.c_str()); // Write the lives in the bottom right corner
	char c; // character initializing
	gameRun = true; // flag for running the game
	bo.drawInitialBoard(winw,winh);
	gfx_color(255,255,255);
	gfx_text(sw/2-13,sh/2+65,"READY"); // Instructions in the center
	gfx_text(sw/2-63,sh/2-50,"Use the arrow keys");
	gfx_text(sw/2-25,sh/2-35,"to move");
	gfx_text(sw/2-58,sh/2,"Press 'q' to quit");
	gfx_text(sw/2-63,sh/2+15,"Press 'p' to pause");
	c =' ';
	int turnCount = 0; // To be used for ghost eating
	gfx_color(0,0,0);
	usleep(3000000);
	gfx_fill_rectangle(13*winw/colSize,50+18*winh/rowSize,3*winw/colSize,winh/rowSize); // Cover up the ready text
	while(gameRun){ // while the conditions to run the game are true
		if (lifeLost){ // If life was lost on previous turn
			usleep(1500000); // wait for 1.5 seconds
			gfx_color(0,0,0);
			gfx_fill_rectangle((double)(winw*p.getCol())/colSize,50+(double)(winh*p.getRow())/rowSize,(double)winw/colSize,(double)winh/rowSize);
			p.reset(bo); // reset the pacman location
			for (auto it = ghosts.begin();it<ghosts.end();it++){ // cycle throught the ghosts
				int row = (*it).getRow();
				int col = (*it).getCol();
				char prev = (*it).getPrev(); // set last location to previous character
				gfx_color(0,0,0); // cover up last location as black
				gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*row)/rowSize,(double)winw/colSize,(double)winh/rowSize);
				if (prev == '.'){ // readraw what used to be there
					gfx_color(255,255,255);
					gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*(row)+0.25*winh)/rowSize),(double)winw/colSize*0.2,(double)winw/colSize*0.2,0,360);
				}
				if (prev == 'O'){
					gfx_color(255,255,255);
					gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*(row)+0.25*winh)/rowSize),(double)winw/colSize*0.5,(double)winw/colSize*0.5,0,360);
				}
				bo.changeSpace(row,col,prev); // Make sure array is up to date
				(*it).reset(bo); // reset ghost locations
				(*it).redraw(bo,winw,winh); // Draw the ghost
			}
			lifeLost = false; // reset flag
			p.redraw(bo,winw,winh); // redraw pacman
			gfx_color(0,0,0);
			gfx_fill_rectangle(10,winh+50,200,50); // cover up lives counter and redraw
			gfx_color(255,255,255);
			lifetext = base+to_string(lives);
			gfx_text(10,winh+60,lifetext.c_str());
			gfx_text(sw/2-13,sh/2+65,"READY"); // Draw ready
			gfx_color(0,0,0);
			usleep(3000000); // wait three seconds, cover up READY
			gfx_fill_rectangle(13*winw/colSize,50+18*winh/rowSize,3*winw/colSize,winh/rowSize);
		}
		usleep(200000); // Wait 0.2s between turns
		turnCount++; // increase the turns
		for(auto it=ghosts.begin();it<ghosts.end();it++){ // cycle through ghosts
			if ((*it).getEat()){ // If allowed to be eaten
				if(turnCount<40); // Check turn count
				else if(turnCount>=40 && turnCount<50){
					(*it).switchCol(); // switch the colors back and forth for last 10 moves
				}
				else{
					(*it).setEat(false); // make ghosts not edible
					(*it).setSwitch(false); // make sure color doesn't switch
				}
			}
		}	
		char move = p.move(bo,c); // move pacman
		if(move=='.'){
			numDots--; // move the pacman, and if dot eaten, decrease number by 1
			score+=100; // increase and show score
			updateScore();
		}
		else if(move=='O'){ // If land on big dot, make ghosts edible
			numDots--;
			score+=100; // Increase and show score
			updateScore();
			turnCount = 0;
			for (auto it = ghosts.begin();it<ghosts.end();it++){ // set ghosts to edible
				(*it).setEat(true);
			}
		}
		bool movesMade = false;
		for(auto it = ghosts.begin();it<ghosts.end();it++){ // cycle through ghosts and pick appropriate
			if((*it).getEat()){ // analyzation of move
				ghostPrey((*it));
				movesMade = true;
			}
			else{
				pacPrey((*it));
			}
		}
		p.redraw(bo,winw,winh); // draw new location of pacman
 		for(auto it = ghosts.begin();it<ghosts.end();it++){
			(*it).redraw(bo,winw,winh); // draw new location of ghost
		}

		if(gfx_event_waiting()==1 || gfx_event_waiting()==2){ // If a key has been pressed
			c = gfx_wait(); // get the character
			gfx_flush();
			while(gfx_event_waiting()){ // flush out the buffer
				gfx_wait();
			}
			if (c=='q') gameRun = false; // if 'q' exit the game
			else if (c=='p'){ // for 'p'
				usleep(100000);
				while(gfx_event_waiting()){ // clear buffer
					gfx_wait();
				}
				gfx_color(255,255,255);
				gfx_text(sw/2-45,sh/2-15,"S TO UNPAUSE"); // display instuction
				while(true){ // infinite loop
					if(gfx_wait()=='s') break; // break out when 's' is pressed
					else;
				}
				gfx_color(0,0,0);
				gfx_fill_rectangle(sw/2-45,sh/2-30,100,20); // cover instruction
			}
			else;
			while(gfx_event_waiting()){ // cleer buffer
				gfx_wait();
			}
		}
		else{ // If no key pressed
			while(gfx_event_waiting()){ // clear event bugger
				gfx_wait();
			}
			c = ' '; // set the character for movement
		}
		if (lives<0 || numDots<=0){ // check for end of game conditions
			gameRun = false; // Set game run to false
			if (lives <0) lives = 0; 
		}
	}
	char end;
	gfx_changefont(font2);
	if(c=='q'){ // Set the message according to the exit condition
		char message[] = "GAME QUIT";
		end = endScreen(message);
	}
	else if (numDots == 0){
		char message[] = "YOU WIN!!";
		end = endScreen(message);
	}
	else if (lives==0){
		char message[] = "GAME OVER";
		end = endScreen(message);
	}
	if(end=='q'); // End game if q was pressed
	else{ // otherwise, clear screen and reset game
		gfx_clear();
		reset();
	}
}

void Game::reset(){ // Reset the whole game
	lives = 3; // Set lives to three
	bo.reset(); // reset the board
	numDots = bo.getDots(); // reset dots
	p.reset(bo); // reset pacman
	for (auto it = ghosts.begin();it<ghosts.end();it++){ // reset ghosts
		(*it).reset(bo);
	}
	gameRun = true; // fix flags
	lifeLost = false;
	score = 0;
	play(); // Play the game again
}

void Game::pacPrey(Ghost &g){ // When ghosts can eat pacman
	if (p.getRow()==(g).getRow() && p.getCol()==(g).getCol()){ // check pacman and ghost locations
		lives--; // If occupying same space
		lifeLost = true; // set flag
	}
	(g).move(bo,p);
	if (p.getRow()==(g).getRow() && p.getCol()==(g).getCol()) {
		lives--;
		lifeLost = true;
	}
}

void Game::ghostPrey(Ghost &g){// When pacman can eat ghosts
	if (p.getRow()==(g).getRow() && p.getCol()==(g).getCol()){ // check pacman and ghost locations
		int row = (g).getRow();
		int col = (g).getCol();
		char prev = (g).getPrev(); // set last location to previous character
		gfx_color(0,0,0); // fill old location with black
		gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*row)/rowSize,(double)winw/colSize,(double)winh/rowSize);
		if (prev == '.'){ // Add small dot if needed
			numDots--;
			score+=300; // update the score
			updateScore();
			gfx_color(255,255,255);
			gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*(row)+0.25*winh)/rowSize),(double)winw/colSize*0.2,(double)winw/colSize*0.2,0,360);
		}
		if (prev == 'O'){ // Add large dot if need
			numDots--;
			score+=300; // update score
			updateScore();
			gfx_color(255,255,255);
			gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*(row)+0.25*winh)/rowSize),(double)winw/colSize*0.5,(double)winw/colSize*0.5,0,360);
		}
		(g).reset(bo);
	}
	(g).move(bo,p); // move pacman and perform the same checks
	if (p.getRow()==(g).getRow() && p.getCol()==(g).getCol()) {
		int row = (g).getRow();
		int col = (g).getCol();
		char prev = (g).getPrev(); // set last location to previous character
		gfx_color(0,0,0);
		gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*row)/rowSize,(double)winw/colSize,(double)winh/rowSize);
		if (prev == '.'){
			numDots--;
			score+=300;
			updateScore();
			gfx_color(255,255,255);
			gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*(row)+0.25*winh)/rowSize),(double)winw/colSize*0.2,(double)winw/colSize*0.2,0,360);
		}
		if (prev == 'O'){
			numDots--;
			score+=300;
			updateScore();
			gfx_color(255,255,255);
			gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*(row)+0.25*winh)/rowSize),(double)winw/colSize*0.5,(double)winw/colSize*0.5,0,360);
		}
		(g).reset(bo);
	}

}

char Game::endScreen(char* message){
	char c='q';
	gfx_color(128,128,128); // set colo to grey
	gfx_fill_rectangle(sw/2-sw/4,sh/2-sh/4,sw/2,sh/2);
	gfx_color(255,255,255);
	gfx_text(sw/2-30,sh/2-30,message); // Show message on grey box
	gfx_text(sw/2-70,sh/2,"To restart, press a key");
	gfx_text(sw/2-50,sh/2+30,"To close, press q");
	usleep(500000); // wait for 0.5s
	while(true){
		if(gfx_event_waiting()==1 || gfx_event_waiting()==2){ // if key pressed
			c = gfx_wait(); // get character
			if(c=='q') break;
			else{
				break;
			}
		}
		else{ // While no key pressed
			while(gfx_event_waiting()){
				gfx_wait();
			}
		}
	}
	return c; // restin character
}

void Game::updateScore(){
	gfx_color(0,0,0);
	gfx_fill_rectangle(winw-100,winh+50,100,50); // cover score text
	gfx_color(255,255,255);
	string base2 = "Score: "; // redraw score text
	string scoreText = base2+to_string(score);
	gfx_text(winw-100,winh+60,scoreText.c_str());
}