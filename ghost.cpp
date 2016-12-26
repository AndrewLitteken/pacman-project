/*
	ghost.cpp
	Functions for the ghost class
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <array>
using namespace std;
#include "gfxnew.h"
#include "ghost.h"

Ghost::Ghost(Board &bo,int r, int c, int d, char n){
	col =  c; // Set location, name, and direction
	row =  r;
	colOrig = c;
	rowOrig = r;
	dir = d;
	dirOrig = d;
	name = n;
	canEat = false;
	switchColor = false;
	prev = bo.getSpace(row,col); // Set what used to be in that location
	bo.changeSpace(row,col,name); // Place on board
}

Ghost::~Ghost(){}

bool Ghost::checkSurround(Board& bo){
	return true;
}

bool Ghost::checkDir(Board& bo,int rowDir,int colDir){ // Check the next location
	bool move = false;
	if(bo.getSpace(row+rowDir,col+colDir)=='.' || bo.getSpace(row+rowDir,col+colDir)==' ' || bo.getSpace(row+rowDir,col+colDir)=='P') move = true;;
	return move; // If not a '#' or another ghost (not 'P'), is a valid place to move
}

void Ghost::moveDecision(Board& bo){ // When at a junction, decide which direction to move
	bool moveFound = false;
	array<int,4> nums = {1,2,3,4}; //array of numbers
	unsigned seed = chrono::system_clock::now().time_since_epoch().count(); // seed from comptuer clock
	shuffle(nums.begin(), nums.end(), default_random_engine(seed)); // Shuffle the range
	for(auto i = 0;i<4;i++){ // look through array
		switch (nums[i]){ //using number in array
			case 1: // up
				if (dir==-2); // do nothing if it is the current direction
				else if(checkDir(bo,-1,0)){ // Check if valid
					moveFound = true; // Set to true if true
					dir = 2; // Set direction
				}
				break;
			case 2: // down
				if (dir==2);
				else if(checkDir(bo,1,0)){
					moveFound = true;
					dir = -2;
				}
				break;
			case 3: // left
				if (dir==1);
				else if(checkDir(bo,0,-1)){
					moveFound = true;
					dir = -1;
				}
				break;
			case 4: // right
				if (dir==-1);
				else if(checkDir(bo,0,1)){
					moveFound = true;
					dir = 1;
				}
				break;
		}
		if (moveFound) break; // exit for loop once a valid direction is found
	}
}

void Ghost::move(Board& bo,Pacman& p){ // Decide where to move the ghosts
	if(dir%2==0){ // If moving vertically
		if(bo.getSpace(row,col-1)!='#' ||  bo.getSpace(row,col+1)!='#'){ // Check if in a tunnel
			moveDecision(bo); // If not, use junction algorithm
		}
		bo.changeSpace(row,col,prev); // change the current space back to original
	}
	else{ // if moving horizontally
		if(bo.getSpace(row-1,col)!='#' ||  bo.getSpace(row+1,col)!='#'){
			moveDecision(bo);
		}
		bo.changeSpace(row,col,prev);
	}
	if(dir%2==0){ // If about to move vertically
		if(bo.getSpace(row-dir/2,col)!='.' && bo.getSpace(row-dir/2,col)!=' ' && bo.getSpace(row-dir/2,col)=='G') dir = -dir;
		// change direction if detect another ghost
		if (bo.getSpace(row-dir/2,col)!='P'){  // As long as the next spot is not pacman
			setPrev(bo.getSpace(row-dir/2,col)); // Store the value of the current space
		}
		else setPrev(' '); // If it is pacman, set it to be a space
		row = row-dir/2; // change the row
		bo.changeSpace(row,col,name);// change the space to read that is a ghost
	}
	else{ // If about to move horizontally
		if(bo.getSpace(row,col+dir)!='.' && bo.getSpace(row,col+dir)!=' ' && bo.getSpace(row,col+dir)=='G') dir = -dir;
		if (bo.getSpace(row,col+dir)!='P'){
			setPrev(bo.getSpace(row,col+dir));
		}
		else setPrev(' ');
		col+=dir;
		bo.changeSpace(row,col,name);
	}
}

int Ghost::getRow(){
	return row;
}

int Ghost::getCol(){
	return col;
}

void Ghost::setRow(int r){
	row = r;
}

void Ghost::setCol(int c){
	col = c;
}

int Ghost::getDir(){
	return dir;
}

void Ghost::setEat(bool eat){
	canEat = eat;
}

bool Ghost::getEat(){
	return canEat;
}

void Ghost::setPrev(char p){
	prev = p;
}

void Ghost::setSwitch(bool b){
	switchColor = b; // Defines whether the ghost should switch color
}

void Ghost::switchCol(){ // A function to switch the color
	if(switchColor) switchColor = false;
	else switchColor = true;
}

char Ghost::getPrev(){
	return prev;
}

void Ghost::reset(Board &bo){ // Reset the ghost to its original state
	row = rowOrig; // set original rows
	col = colOrig;
	switchColor = false;
	canEat = false;
	setPrev(bo.getSpace(row,col)); // hold onto current value of new space
	bo.changeSpace(row,col,name); // change the space to be a ghost
}

void Ghost::redraw(Board &bo, int winw, int winh){ // redrawing function for the ghosts
	int drow = row; // find the changes for horizontal movements
	int dcol = col-dir;
	if(dir%2==0){ // If vertical, redefine the changes
		drow = row+dir/2;
		dcol = col;
	}
	gfx_color(0,0,0); // Set color to black and fill the current space
	gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*row)/rowSize,(double)winw/colSize,(double)winh/rowSize);
	if(rowOrig==row && colOrig==col){ // If in the original location
		if((drow==0 || drow==rowSize-1) || (dcol==0||dcol==colSize-1)); // And the space directly behind is not an edge
		else{ // Make that square black also
			gfx_fill_rectangle((double)(winw*dcol)/colSize,50+(double)(winh*drow)/rowSize,(double)winw/colSize,(double)winh/rowSize);
		}
		}	
	else{ // If not in original locaiton, fill square behind with black
		gfx_fill_rectangle((double)(winw*dcol)/colSize,50+(double)(winh*drow)/rowSize,(double)winw/colSize,(double)winh/rowSize);
	}
	if (canEat){ // If the ghost can be eaten
		gfx_color(0,255,0); // Make it green
		(!switchColor)?gfx_color(0,255,0):gfx_color(255,255,255); // Based on a switch variable, change color between white and green
	}
	else gfx_color(255,0,0); // Set to red and draw a ghost in current space
	gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*row+0.25*winh)/rowSize),(double)winw/(2*colSize),(double)winh/(2*rowSize),0,360);
	if(bo.getSpace(drow,dcol)=='.'){ // If space behind was a '.'
		gfx_color(255,255,255); // Draw the small dot
		gfx_fill_arc((double) ((winw*dcol+winw*0.25)/colSize),50+(double)((winh*drow+0.25*winh)/rowSize),(double)winw/colSize*0.2,(double)winw/colSize*0.2,0,360);
	}
	else if(bo.getSpace(drow,dcol)=='O'){ // If space behind was a 'O'
		gfx_color(255,255,255); // Draw the large circle.
		gfx_fill_arc((double)(winw*dcol+winw*0.25)/colSize,50+(double)(winh*drow+winh*0.25)/rowSize,(double)winw/colSize*0.5,(double)winw/colSize*0.5,0,360);
	}
}