/*
	board.cpp
	Functions for the board class
	Authors: Andrew Litteken and Tanner Leighton
	Date: 2016-12-15
*/

#include <iostream>
#include <string>
#include <fstream>
#include "gfxnew.h"
using namespace std;
#include "board.h"

Board::Board(){ // Create the Class itself
	dots = 0;
	ifstream ifs; // Open an ifs stream
	ifs.open("pacBoard.txt"); // Open up the file containing the game board
	char c = ifs.peek(); // Check the next character
	int row = 0, col =0;
	while(c!=EOF){ // While not at the end of file
		col = 0;
		string boardLine;
		getline(ifs, boardLine, '\n'); // Get the line of the board
		for (auto it = boardLine.begin();it<boardLine.end();it++){ // Cycle through each character in line
			board[row][col] = *it; // Place into corresponding column and row
			boardOrig[row][col] = *it; // Store in memory for the reset
			col++; // Move to next column
			if(*it == '.' || *it == 'O'){
				dots++; // Add a dot if the space is designated as such
			}
		}
		row++; // move to next row
		c = ifs.peek(); // check the next character
	}
	dots--; // One space will be held by pacman
	dotsOrig = dots; // Set the number of dots
}

Board::~Board() {};

void Board::changeSpace(int row, int col, char c){
	boardPrev[row][col] = board[row][col];
	board[row][col] = c; // Set the character in this space
}

char Board::getSpace(int row,int col){
	return board[row][col]; // Get the character in this location
}

void Board::print(){
	for (int row=0;row<rowSize;row++){ // Display each row
		for (int col=0;col<colSize;col++){ // Display each column
			cout<<board[row][col];
		}
		cout<<endl;
	}
}

int Board::getDots(){
	return dots; // return the number of dots
}

void Board::reset(){ // Reset the board after the game ends
	for (int i=0;i<rowSize;i++){ // Copy the original board to the current
		for (int j = 0;j<colSize;j++){
			board[i][j]=boardOrig[i][j];
		}
	}
	dots = dotsOrig;
}

void Board::drawInitialBoard(int winw,int winh){ // Draw the initial board structure
	for(int i=0;i<colSize;i++){ // Look through every element
		for(int j=0;j<rowSize;j++){
			if(board[j][i]=='#'){ // Where there is a "#" draw a blue square
				gfx_color(0,0,255);
				gfx_fill_rectangle((double)(winw*i)/colSize,50+(double)(winh*j)/rowSize,(double)winw/colSize,(double)winh/rowSize);
			}
			else if(board[j][i]=='.'){ // Draw a small centered white dot for a '.'
				gfx_color(255,255,255);
				gfx_fill_arc((double)(winw*i+winw*0.25)/colSize,50+(double)(winh*j+winh*0.25)/rowSize,(double)winw/colSize*0.25,(double)winw/colSize*0.2,0,360);
			}
			else if(board[j][i]=='P'){ // Draw a yellow arc for a 'P'
				gfx_color(255,255,0);
				gfx_fill_arc((double)((winw*i+winw*0.25)/colSize),50+(double)((winh*j+0.25*winh)/rowSize),(double)winw/(2*colSize),(double)winh/(2*rowSize),20,340);
			}
			else if(board[j][i]=='G'){ // Draw a red cirlce for a 'G'
				gfx_color(255,0,0);
				gfx_fill_arc((double)(winw*i+winw*0.25)/colSize,50+(double)(winh*j+winh*0.25)/rowSize,(double)winw/(2*colSize),(double)winw/(2*colSize)*0.75,0,360);
			}
			else if(board[j][i]=='O'){ // Draw a large centered white cirlce for a 'O'
				gfx_color(255,255,255);
				gfx_fill_arc((double)(winw*i+winw*0.25)/colSize,50+(double)(winh*j+winh*0.25)/rowSize,(double)winw/colSize*0.5,(double)winw/colSize*0.5,0,360);
			}
		}
	}
}