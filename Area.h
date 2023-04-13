#ifndef __SUDOKU_AREA
#define __SUDOKU_AREA

#include "CellData.h"
#include<vector>
#include<random>
#include<ctime>
#include<map>

enum AreaType 
{
	AREA_ROW,
	AREA_COLUMN,
	AREA_BOX
};

struct SudokuBoard { 
	int board[9][9];
};






class Area 
{
	private:
		AreaType type;   // identifies the type: row, column or box type
		int idx;         // identifies which row, column or box
		int counter [9]; // counts occurences of a number in this area
		
	public:
		Area ();
		~Area ();
		
		void init (AreaType type, int idx);
		
		void DigitRemoved (int &digit, CellData (&data) [9][9], int &x, int &y);
		void DigitAdded (int &digit, CellData (&data) [9][9], int &x, int &y);
};

#endif