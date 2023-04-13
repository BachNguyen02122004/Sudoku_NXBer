#ifndef __CELLDATA
#define __CELLDATA
#include<SDL.h>

struct CellData {
	int digit = 0; 	// the actual digit in this cell, 0 if nothing
	int used ;
	int vldcnt = 0; // counts number of same digits in shared areas - so must be 0 for a correct sudoku dem so giong nhau 
	
	};
#endif 