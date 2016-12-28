/*
	pacman.cpp
	Functions for the pacman class 
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include "pacman.h"

Pacman::Pacman(){
	col =  14; // Set the locations
	row =  23;
	colOrig =  14;
	rowOrig =  23;
	dir = -1;
}

Pacman::Pacman(Board &bo){
	col =  14;
	row =  23;
	colOrig =  14;
	rowOrig =  23;
	dir = -1;
	bo.changeSpace(row,col,'P');
}

Pacman::~Pacman(){}

char Pacman::move(Board &bo, char c){ // Interpret the user input and move pacman accordingly
	char s; // tracking character
	switch (c) { // Find the character input by the user
		case 'T': // If down
			if(checkDir(bo,1,0)){ // Check if good
				bo.changeSpace(row,col,' '); // Set current location to be a space
				row++; // Increase row
				s = bo.getSpace(row,col); // get character at location
				dir=-2; // set the direction (-2 is down, 2 is up, -1 is left, 1 is right)
				bo.changeSpace(row,col,'P'); // Set new space character as pacman
			}
			else gfx_flush();
			break;
		case 'R': // if up
			if(checkDir(bo,-1,0)){ // Same strcutre as above
				bo.changeSpace(row,col,' ');
				row--;
				s = bo.getSpace(row,col);
				dir = 2;
				bo.changeSpace(row,col,'P');
			}
			else gfx_flush();
			break;
		case 'S': // if right
			if(checkDir(bo,0,1)){
				bo.changeSpace(row,col,' ');
				col++;
				s = bo.getSpace(row,col);
				dir=1;
				bo.changeSpace(row,col,'P');
			}
			else gfx_flush();
			break;	
		case 'Q': // if left
			if(checkDir(bo,0,-1)){
				bo.changeSpace(row,col,' ');
				col--;
				s = bo.getSpace(row,col);
				dir=-1;
				bo.changeSpace(row,col,'P');
			}
			else gfx_flush();
			break;
		default: // if not WASD entry, continue in current direction if possible
			if(dir==-2){
				if(checkDir(bo,1,0)){
					bo.changeSpace(row,col,' ');
					row++;
					s = bo.getSpace(row,col);
					dir=-2;
					bo.changeSpace(row,col,'P');
				}
			}
			if(dir == 2){
				if(checkDir(bo,-1,0)){
					bo.changeSpace(row,col,' ');
					row--;
					s = bo.getSpace(row,col);
					dir = 2;
					bo.changeSpace(row,col,'P');
				}
			}
			if(dir==1){
				if(checkDir(bo,0,1)){
					bo.changeSpace(row,col,' ');
					col++;
					s = bo.getSpace(row,col);
					dir=1;
					bo.changeSpace(row,col,'P');
				}
			}	
			if(dir==-1){
				if(checkDir(bo,0,-1)){
					bo.changeSpace(row,col,' ');
					col--;
					s = bo.getSpace(row,col);
					dir=-1;
					bo.changeSpace(row,col,'P');
				}
			}
			break;
	}
	return s;
}

bool Pacman::checkDir(Board &bo,int rowDir,int colDir){ // Check the location in question
	bool move = true;
	if(bo.getSpace(row+rowDir,col+colDir)=='#') move = false; // Check location set if false
	return move;
}

int Pacman::getRow(){
	return row;
}

int Pacman::getCol(){
	return col;
}

void Pacman::setRow(int r){
	row=r;
}

void Pacman::setCol(int c){
	col=c;
}

int Pacman::getDir(){
	return dir;
}

void Pacman::reset(Board &bo){ // Reset to the original location
	row = rowOrig;
	col = colOrig;
	dir = -1;
	bo.changeSpace(row,col,'P'); // place on board
}

void Pacman::redraw(Board &bo, int winw, int winh){ // Redrawing function for pacman
	if(dir%2==0){ // If moving in a vertical direction
		gfx_color(0,0,0); // Set color to black, and draw a black square in the current location, and one behind
		gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*row)/rowSize,(double)winw/colSize,(double)winh/rowSize);
		gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*(row+dir/2))/rowSize,(double)winw/colSize,(double)winh/rowSize);
		gfx_color(255,255,0); // fill current location with a yellow arc
		gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*row+0.25*winh)/rowSize),(double)winw/(2*colSize),(double)winh/(2*rowSize),20,340);
	}
	else{ // If moving in a horizontal direction, do the same as above
		gfx_color(0,0,0);
		gfx_fill_rectangle((double)(winw*col)/colSize,50+(double)(winh*row)/rowSize,(double)winw/colSize,(double)winh/rowSize);
		gfx_fill_rectangle((double)(winw*(col-dir)/colSize),50+(double)(winh*(row)/rowSize),(double)winw/colSize,(double)winh/rowSize);
		gfx_color(255,255,0);
		gfx_fill_arc((double)((winw*col+winw*0.25)/colSize),50+(double)((winh*row+0.25*winh)/rowSize),(double)winw/(2*colSize),(double)winh/(2*rowSize),20,340);
	}
}
